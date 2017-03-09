#include "RayTracer.h"
#include "scene.h"
#include "../linearmath/profiler.h"
#include "rayintersection.h"
#include <algorithm>
#include "../common/defaults.h"
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

RayTracer::RayTracer() : imgBuffer_(*new Imagemap(500,500)) {
  scene_ = nullptr;
  //cameraPos = {0.0f , 0.0f , 100.0f};
  cameraPos = {0.0f , 0.0f , 100.0f};
  Initialize();
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
  initCLPlatfrom(0, imgBuffer_.getData());
  return string();

/*
  // Create camera via setting up position and direction
  Camera camera;
  camera.setResolution(500,400);
  camera.setPosition({0.0f,0.0f,100.0f}, {0.0f,0.0f,-1.0f});
  camera.setRotation(0.0f);
  camera.setCameraToScreenDistance(4.0);
  camera.setFOVDegrees(60.0f);
  camera.setAspect(1.0f);

  const auto size   = camera.getResolution();
  const auto width  = size.first;
  const auto height = size.second;
  const float cameraDist = camera.getCameraToScreenDistance();
  const auto dxdy = camera.getDxDY();

  float dX = dxdy.first;
  float dY = dxdy.second;

  //
  std::string msgResults;
  std::string msgHeader = "Rendering image of " + 
    std::to_string(width * height / (float)1'000'000) + " Mpx";
  Profiler prf(msgHeader, true);

  prf.start();
  const float  min = -1.0f * cameraDist;
  //const float  max =  1.0f * cameraDist;
  //float aspect = width / static_cast<float>(height);

  //float dX = (max - min) / static_cast<float>(width) * aspect;
  //float dY = (max - min) / static_cast<float>(height);

  float sy = min;

  auto currentPix = imgBuffer_.getData();
  float distance = 100.0f;

  Ray ray(cameraPos, vec3<float>());
  
  linearmath::vec3<float> dx_dir = {};
  linearmath::vec3<float> dy_dir = {};

  
  for (size_t y(0); y < height; ++y) {
    float sx = min;
    for (size_t x(0); x < width; ++x) {
      camera.getRay(x,y,ray);
      //vec3<float> direction = vec3<float>(sx, sy, 90.f) - cameraPos;
      //ray.resetIntersection();
      //ray.setDirection(direction);
      vec3<float> color = Trace( ray, distance) * 255;

      currentPix[2] = static_cast<unsigned char>(color.x);
      currentPix[1] = static_cast<unsigned char>(color.y);
      currentPix[0] = static_cast<unsigned char>(color.z);
      currentPix += 3;
      sx += dX;
    }
    //sy += dY;
  }
  prf.finish();
  render_time_ = prf.getFPS();
  return prf.getResults();
*/
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

void RayTracer::initCLPlatfrom(size_t platform_num, unsigned char *img) {
	
	cl_int err;

	// 1) Obtain selected platform
	std::vector<cl::Platform> platforms;
	err = cl::Platform::get(&platforms);
	checkErr( (platforms.size() != 0 && platform_num < platforms.size()) ? CL_SUCCESS : -1,
	          "Can't obtain selected platform");

	// 2) Create context in accordance with devices of obtained platform
	std::vector<cl::Device> devices;
	platforms[platform_num].getDevices(CL_DEVICE_TYPE_CPU, &devices);
	checkErr( devices.size() != 0 ? CL_SUCCESS : -1,"Can't obtain devices from selected platform");

	cl::Context context(devices);
	checkErr(err, "Can't create context");

	// 3) Allocate the host memory to be used by OpenCL kernel. Make a buffer from it
	// For now let's assume that only 1 Mpix image to be used
	const size_t width = 500;
	const size_t height = 500;
	const size_t channels = 3;

	const size_t buf_size = width*height*channels;

	unsigned char *picBuf = img;//new char[buf_size + 1];
	cl::Buffer outCL(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, buf_size + 1, picBuf, &err);
	checkErr(err, "Can't create a buffer");

	// 4) Query devices from the context
	devices = context.getInfo<CL_CONTEXT_DEVICES>();
	checkErr(devices.size() > 0 ? CL_SUCCESS : -1, "The contex doesn't provide any devices");

    auto keeper = KernelKeeper::getInstance();
    keeper.addKernelToList("hello",L"C:\\Users\\Anton\\Desktop\\tracer\\tracer\\src\\tracelib\\cl_kernels\\hellworld.cl", devices, context);
    //keeper.addKernelToList(L"E:\\sources\\tracer\\src\\tracelib\\cl_kernels\\hellworld.cl");
    auto krnl_draw_circle = keeper.buildKernel("hello");

    // Set arguments
    krnl_draw_circle.setArg(0, outCL);

   //Enqueue kernel to all workgroups and CU ( compute units )
   cl::CommandQueue queue(context, devices[0]);

   size_t max_work_dim = 0;
   devices[0].getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &max_work_dim);
   size_t work_dims = 2; // Corresponds to the CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS.
   
   cl::Event frame_is_done;
   cl::NDRange offset(0, 0), work_size(width, height);
   queue.enqueueNDRangeKernel(krnl_draw_circle, offset, work_size, cl::NullRange, nullptr, &frame_is_done);
   frame_is_done.wait();

   queue.enqueueReadBuffer(outCL, CL_TRUE, 0, buf_size, picBuf);
}
