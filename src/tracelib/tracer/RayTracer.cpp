#include "RayTracer.h"
#include "scene.h"
#include "../linearmath/profiler.h"
#include "rayintersection.h"
#include <algorithm>
#include "../common/defaults.h"

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
  // Initialize OpenCL library
/*
  std::string hw("Hello Antony");

  // 1) Get platforms list
  cl_int err;
  std::vector< cl::Platform > platformList;
  cl::Platform::get(&platformList);
  checkErr(platformList.size() != 0 ? CL_SUCCESS : -1, "cl::Platform::get");
  std::cerr << "Platform number is: " << platformList.size() << std::endl; std::string platformVendor;

  platformList[0].getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);
  std::cerr << "Platform is by: " << platformVendor << "\n";

  // 2) Create context
  cl_context_properties cprops[3] = { 
    CL_CONTEXT_PLATFORM, (cl_context_properties)(platformList[0])(), 0 
  }; 
  cl::Context context(CL_DEVICE_TYPE_CPU, cprops, NULL, NULL, &err);
  checkErr(err, "Context::Context()");

  // 3) Allocate the host memory to be used by OpenCL kernel
  char * outBuffer = new char[hw.length() + 1];
  cl::Buffer outCL(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, hw.length() + 1, outBuffer, &err);
  checkErr(err, "Buffer::Buffer()");

  // 4) Query devices from the context
  std::vector<cl::Device> devices;
  devices = context.getInfo<CL_CONTEXT_DEVICES>();
  std::cerr << "Selected context has " << devices.size() << " devices" << std::endl;
  checkErr(devices.size() > 0 ? CL_SUCCESS : -1, "devices.size() > 0");

  /////////////////////////////////////////////////////////////////////////////

  // 5) Time to build executing kernel from sources is come

  std::ifstream file;
  file.open("E:/sources/tracer/src/tracelib/cl_kernels/hellworld.cl", std::ifstream::in);
  checkErr(file.is_open() ? CL_SUCCESS : -1, "lesson1_kernel.cl");

  std::string prg_sources(
    (std::istreambuf_iterator<char>(file)),
    (std::istreambuf_iterator<char>()));
  cl::Program::Sources source(1, std::make_pair(prg_sources.c_str(), prg_sources.length() + 1));

  cl::Program program(context, source);
  err = program.build(devices, "");
  checkErr(err, "Program::build()");

  // 6)  Let's work with kernel
  cl::Kernel kernel(program, "hello", &err);
  checkErr(err, "Kernel::Kernel()"); 
  err = kernel.setArg(0, outCL);
  checkErr(err, "Kernel::setArg()");

  // 7) Create queue and execute our kernel
  cl::CommandQueue queue(context, devices[0], 0, &err);
  checkErr(err, "CommandQueue::CommandQueue()"); 
  
  cl::Event event;
  err = queue.enqueueNDRangeKernel(
    kernel, cl::NullRange, cl::NDRange(hw.length() + 1),
    cl::NDRange(1, 1), NULL, &event);
  checkErr(err, "ComamndQueue::enqueueNDRangeKernel()");

  // 8) Wait for program finishing
  event.wait();
  err = queue.enqueueReadBuffer(outCL, CL_TRUE, 0, hw.length() + 1, outBuffer);
  checkErr(err, "ComamndQueue::enqueueReadBuffer()");

  MessageBox(NULL, "The program is saying ", outBuffer, MB_OK);
  //return EXIT_SUCCESS;
*/

}

RayTracer::RayTracer() : imgBuffer_(*new Imagemap(500,400)) {
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
  if (!scene_)
    return string();
  imgBuffer_.reset();

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

void RayTracer::initCLPlatfrom(size_t platform_num) {
	
	cl_int err;

	// 1) Obtain selected platform
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
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
	const size_t width = 1000;
	const size_t height = 1000;
	const size_t channels = 3;

	const size_t buf_size = width*height*channels;

	char *picBuf = new char[buf_size + 1];
	cl::Buffer outCL(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, buf_size + 1, picBuf, &err);
	checkErr(err, "Can't create a buffer");

	// 4) Query devices from the context
	std::vector<cl::Device> devices;
	devices = context.getInfo<CL_CONTEXT_DEVICES>();
	checkErr(devices.size() > 0 ? CL_SUCCESS : -1, "The contex doesn't provide any devices");

	// Now read the kernel file
	std::ifstream programFile("kernels.cl");
	std::string programString(
		std::istreambuf_iterator<char>(programFile),
		(std::istreambuf_iterator<char>()));
	///////////////////////////////////////////////////////////////////////////////

	//// 5) Time to build executing kernel from sources is come

	//std::ifstream file;
	//file.open("E:/sources/tracer/src/tracelib/cl_kernels/hellworld.cl", std::ifstream::in);
	//checkErr(file.is_open() ? CL_SUCCESS : -1, "lesson1_kernel.cl");

	//std::string prg_sources(
	//	(std::istreambuf_iterator<char>(file)),
	//	(std::istreambuf_iterator<char>()));
	//cl::Program::Sources source(1, std::make_pair(prg_sources.c_str(), prg_sources.length() + 1));

	//cl::Program program(context, source);
	//err = program.build(devices, "");
	//checkErr(err, "Program::build()");

	//// 6)  Let's work with kernel
	//cl::Kernel kernel(program, "hello", &err);
	//checkErr(err, "Kernel::Kernel()");
	//err = kernel.setArg(0, outCL);
	//checkErr(err, "Kernel::setArg()");

	//// 7) Create queue and execute our kernel
	//cl::CommandQueue queue(context, devices[0], 0, &err);
	//checkErr(err, "CommandQueue::CommandQueue()");

	//cl::Event event;
	//err = queue.enqueueNDRangeKernel(
	//	kernel, cl::NullRange, cl::NDRange(hw.length() + 1),
	//	cl::NDRange(1, 1), NULL, &event);
	//checkErr(err, "ComamndQueue::enqueueNDRangeKernel()");

	//// 8) Wait for program finishing
	//event.wait();
	//err = queue.enqueueReadBuffer(outCL, CL_TRUE, 0, hw.length() + 1, outBuffer);
	//checkErr(err, "ComamndQueue::enqueueReadBuffer()");

	//MessageBox(NULL, "The program is saying ", outBuffer, MB_OK);
	//return EXIT_SUCCESS;
}
