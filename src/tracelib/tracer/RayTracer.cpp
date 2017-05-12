#include "RayTracer.h"
#include "scene.h"
#include "../linearmath/profiler.h"
#include "rayintersection.h"
#include <algorithm>
#include "../common/defaults.h"
#include "../common/KernelKeeper.h"

#include "../cl_kernels/logic_kernel_attendant.h"
#include "../cl_kernels/cl_ray_attendant.h"

#include "../common/KernelKeeper.h"

#ifdef _WIN64
#include "Windows.h"
#endif

//#define __NO_STD_VECTOR
#include <iostream>
#include <string>

#include "camera.h"
//#include <CL/opencl.h>
#include <CL/cl.hpp>

using namespace linearmath;

const size_t frame_width = 1920;
const size_t frame_height = 1080;

void checkErr(cl_int err, const char * name) {
  if (err != CL_SUCCESS) {
    std::cerr << "OPEN CL ERROR: " << name << " (" << err << ")" << std::endl;
    std::string msg("OPEN CL ERROR: ");
    msg += name;
    msg += " (";
    msg += err;
    msg += " )\n";
    OutputDebugString(msg.c_str());
    exit(EXIT_FAILURE);
  }
}

void RayTracer::Initialize() {
}

RayTracer::RayTracer() : imgBuffer_(*new Imagemap(frame_width, frame_height)) {
  scene_ = nullptr;
  // Use negative values as unitialized
  render_time_ = -100.0;
  cameraPos = {0.0f , 0.0f , 100.0f};
  Initialize();
  initCLPlatfrom();
}

RayTracer::~RayTracer() {
  int i = 0;
}


inline float clamp(float val, float low, float high) {
  if (val>low && val<high)
    return val;
  if (val <= low)
    return low;
  else 
    return high;
}

std::string RayTracer::Run() {
  //if (!scene_)
  //  return string();

  imgBuffer_.reset();

  // Create camera via setting up position and direction
  Camera camera;
  camera.setResolution(imgBuffer_.width(),imgBuffer_.height());
  camera.setPosition({0.0f,0.0f,100.0f}, {0.0f,0.0f,-1.0f});
  camera.setRotation(0.0f);
  camera.setCameraToScreenDistance(4.0);
  camera.setFOVDegrees(140.0f);
  camera.setAspect(1.0f);

  // Create OpenCL structure to pass camera settings to the kernel
  ScreenParameters params;
  params.frame_width = camera.getResolution().first;
  params.frame_height = camera.getResolution().second;

  params.SetWidthDir(camera.getUDir());
  params.SetHeightDir(camera.getVDir());

  params.width_ratio = camera.getDxDY().first;
  params.height_ratio = camera.getDxDY().second;

  params.SetScreenOrigin(camera.getScreenOrigin());
  params.SetCameraOrigin(camera.getPosition());


  // Obtain OpenCL kernel 
  auto kernel = KernelKeeper::getInstance().getKernel("logic");
  auto context = KernelKeeper::getInstance().getContext("logic");
  auto devices = KernelKeeper::getInstance().getDevices("logic");

  // Create 2 buffers. The first one is the rays storage and the other is the buffer made up from image
  cl_int err;

  std::vector<CLRay> rays(imgBuffer_.width()*imgBuffer_.height());
  //rays.reserve(imgBuffer_.getMegapixels());

  cl::Buffer param_buf(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(ScreenParameters), &params, &err);
  cl::Buffer rays_buffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, sizeof(CLRay) * imgBuffer_.width()*imgBuffer_.height(), rays.data(), &err);
  cl::Buffer frame_buffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, imgBuffer_.height() * imgBuffer_.width()*3*sizeof(unsigned char), imgBuffer_.getData(), &err);

  //cl::Buffer debug_buffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float) * debug.size(), debug.data(), &err);

  checkErr(err, "Can't create a buffer");

  kernel.setArg(0, frame_buffer);
  kernel.setArg(1, param_buf);
  kernel.setArg(2, rays_buffer);


  //Enqueue kernel to all workgroups
  cl::CommandQueue queue(context, devices[0]);

  size_t max_work_dim = 0;
  devices[0].getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &max_work_dim);
  size_t work_dims = 2; // Corresponds to the CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS.
  
  cl::Event frame_is_done;
  cl::NDRange offset(0, 0), work_size(imgBuffer_.width(), imgBuffer_.height());

  std::string msgHeader = "Rendering image of " + std::to_string(imgBuffer_.getMegapixels()) + " Mpx";
  Profiler prf(msgHeader, true);
  prf.start();

  queue.enqueueNDRangeKernel(kernel, offset, work_size, cl::NullRange, nullptr, &frame_is_done);
  frame_is_done.wait();

  queue.enqueueReadBuffer(frame_buffer, CL_TRUE, 0, imgBuffer_.height() * imgBuffer_.width() * 3 * sizeof(unsigned char), imgBuffer_.getData());


  prf.finish();
  render_time_ = prf.getFPS();
  return prf.getResults();
}

vec3<float> RayTracer::Trace(const Ray &ray, float distanse) {
  if (findIntersections(ray)) {
    return shader(ray);
  } else {
    // The more set bits has default_stars_perion
    // the less stars number will be rendered
    if ((rand() & default_stars_period)) {
      return default_vacuum_color;
    } else {
      return default_star_color;
    }
  }
}

void RayTracer::setScene(Scene *scene) {
  if (scene)
    scene_ = scene;
}

linearmath::vec3<float> RayTracer::shader(const Ray& ray) {
  RayIntersection intersection_point = ray.getIntersectionConst();
  vec3<float> bg_color =// {1.0f,1.0f,0.0f};
  /*vec3<float> diffuse =*/ intersection_point.inssctColor;

  //ntPoint.inssctColor;
  for (size_t i=0; i < 1; ++i) {
    //auto light = scene_->getLight(i);
    auto light = scene_->getLight(i)->getColor();

    vec3<float> dirToLight = (-scene_->getLight(i)->getCenter() + intersection_point.intsctPosition).getNormalized();
    
    float NdotL = dirToLight.dot(intersection_point.intsctNormal);
    if (NdotL >= 0.0f)
      bg_color = bg_color*light*NdotL;
    else
      bg_color = {0.0f,0.0f,0.0f};

    // lightRay(intersection_point.intsctPosition + intersection_point.intsctNormal * 0.001f, dirToLight);
    //RayIntersection lightIntsct;
    /*
    if (findIntersections(lightRay)) {
      float lambert = clamp(NdotL, 0.f, 1.f);
      if (lambert > 0.f) {
        bg_color = light->getColor()*diffuse*lambert;

        if (intersection_point.intsctSpecular > 0.f) {
          vec3<float> V = ray.getDirection();
          vec3<float> R = V - intersection_point.intsctNormal * 2 * V.dot(intersection_point.intsctNormal);
          float VdotR = V.dot(R);
          if (VdotR > 0.0f) {
            float specularShiness = 32.f;
            float spec = pow(VdotR, specularShiness) * lambert * intersection_point.intsctSpecular;
            // add specular component to ray color
            bg_color += light->getColor() * spec;
          }

        }
      }*/
    //} else {
    //  bg_color = {0.,1.,1.};
    //}
  }

  bg_color.x = clamp(bg_color.x, 0.f, 1.f);
  bg_color.y = clamp(bg_color.y, 0.f, 1.f);
  bg_color.z = clamp(bg_color.z, 0.f, 1.f);
  return bg_color;
}

bool RayTracer::findIntersections(const Ray & ray) {
  bool intersectionFound = false;
  for (size_t i(0); i < scene_->size(); ++i) {
    if ((*scene_)[i]->isIntersects(ray)) {
      intersectionFound = true;
    }
  }
  return intersectionFound;
}

void RayTracer::initCLPlatfrom() {
	cl_int err;
  size_t platform_num = 1;

	// 1) Obtain selected platform
	std::vector<cl::Platform> platforms;
	err = cl::Platform::get(&platforms);
	checkErr( (platforms.size() != 0 && platform_num < platforms.size()) ? CL_SUCCESS : -1,
	          "Can't obtain selected platform");

	// 2) Create context in accordance with devices of obtained platform
	std::vector<cl::Device> devices;
	platforms[platform_num].getDevices(CL_DEVICE_TYPE_GPU, &devices);
	checkErr( devices.size() != 0 ? CL_SUCCESS : -1,"Can't obtain devices from selected platform");

	cl::Context context(devices);
	checkErr(err, "Can't create context");

	devices = context.getInfo<CL_CONTEXT_DEVICES>();
	checkErr(devices.size() > 0 ? CL_SUCCESS : -1, "The contex doesn't provide any devices");

  KernelKeeper::getInstance().addKernelToList("logic",L"C:\\Users\\Anton\\Desktop\\tracer\\tracer\\src\\tracelib\\cl_kernels\\logic.cl", devices, context);
  //keeper.addKernelToList(L"E:\\sources\\tracer\\src\\tracelib\\cl_kernels\\hellworld.cl");
  auto krnl_draw_circle = KernelKeeper::getInstance().buildKernel("logic");
}
