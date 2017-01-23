#include "RayTracer.h"
#include "scene.h"
#include "../linearmath/profiler.h"
#include "rayintersection.h"
#include <algorithm>
#include "../common/defaults.h"

#include "camera.h"
#include <CL/opencl.h>

using namespace linearmath;

enum class CLPlatformParams : short {
  PROFILE = CL_PLATFORM_PROFILE,
  VERSION = CL_PLATFORM_VERSION,
  NAME = CL_PLATFORM_NAME,
  VENDOR = CL_PLATFORM_VENDOR,
  EXTENTION = CL_PLATFORM_EXTENSIONS
};

enum class CLDeviceType : unsigned int {
  CPU = CL_DEVICE_TYPE_CPU,
  GPU = CL_DEVICE_TYPE_GPU,
  DEFAULT = CL_DEVICE_TYPE_DEFAULT,
  ACCELERATOR = CL_DEVICE_TYPE_ACCELERATOR,
  CUSTOM = CL_DEVICE_TYPE_CUSTOM,
  ALL = CL_DEVICE_TYPE_ALL
};

enum class CLDeviceParam : unsigned int {
  TYPE = CL_DEVICE_TYPE,
  VENDOR_ID = CL_DEVICE_VENDOR_ID,
  MAX_COMPUTE_UNITS = CL_DEVICE_MAX_COMPUTE_UNITS,
  MAX_WORK_ITEM_DIMENSIONS = CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
  MAX_WORK_GROUP_SIZE = CL_DEVICE_MAX_WORK_GROUP_SIZE,
  MAX_WORK_ITEM_SIZES = CL_DEVICE_MAX_WORK_ITEM_SIZES,
  PREFERRED_VECTOR_WIDTH_CHAR = CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR,
  PREFERRED_VECTOR_WIDTH_SHORT = CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT,
  PREFERRED_VECTOR_WIDTH_INT = CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,
  PREFERRED_VECTOR_WIDTH_LONG = CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG,
  PREFERRED_VECTOR_WIDTH_FLOAT = CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,
  PREFERRED_VECTOR_WIDTH_DOUBLE = CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,
  MAX_CLOCK_FREQUENCY = CL_DEVICE_MAX_CLOCK_FREQUENCY,
  ADDRESS_BITS = CL_DEVICE_ADDRESS_BITS,
  MAX_READ_IMAGE_ARGS = CL_DEVICE_MAX_READ_IMAGE_ARGS,
  MAX_WRITE_IMAGE_ARGS = CL_DEVICE_MAX_WRITE_IMAGE_ARGS,
  MAX_MEM_ALLOC_SIZE = CL_DEVICE_MAX_MEM_ALLOC_SIZE,
  IMAGE2D_MAX_WIDTH = CL_DEVICE_IMAGE2D_MAX_WIDTH,
  IMAGE2D_MAX_HEIGHT = CL_DEVICE_IMAGE2D_MAX_HEIGHT,
  IMAGE3D_MAX_WIDTH = CL_DEVICE_IMAGE3D_MAX_WIDTH,
  IMAGE3D_MAX_HEIGHT = CL_DEVICE_IMAGE3D_MAX_HEIGHT,
  IMAGE3D_MAX_DEPTH = CL_DEVICE_IMAGE3D_MAX_DEPTH,
  IMAGE_SUPPORT = CL_DEVICE_IMAGE_SUPPORT,
  MAX_PARAMETER_SIZE = CL_DEVICE_MAX_PARAMETER_SIZE,
  MAX_SAMPLERS = CL_DEVICE_MAX_SAMPLERS,
  MEM_BASE_ADDR_ALIGN = CL_DEVICE_MEM_BASE_ADDR_ALIGN,
  MIN_DATA_TYPE_ALIGN_SIZE = CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE,
  SINGLE_FP_CONFIG = CL_DEVICE_SINGLE_FP_CONFIG,
  GLOBAL_MEM_CACHE_TYPE = CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
  GLOBAL_MEM_CACHELINE_SIZE = CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
  GLOBAL_MEM_CACHE_SIZE = CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
  GLOBAL_MEM_SIZE = CL_DEVICE_GLOBAL_MEM_SIZE,
  MAX_CONSTANT_BUFFER_SIZE = CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
  MAX_CONSTANT_ARGS = CL_DEVICE_MAX_CONSTANT_ARGS,
  LOCAL_MEM_TYPE = CL_DEVICE_LOCAL_MEM_TYPE,
  LOCAL_MEM_SIZE = CL_DEVICE_LOCAL_MEM_SIZE,
  ERROR_CORRECTION_SUPPORT = CL_DEVICE_ERROR_CORRECTION_SUPPORT,
  PROFILING_TIMER_RESOLUTION = CL_DEVICE_PROFILING_TIMER_RESOLUTION,
  ENDIAN_LITTLE = CL_DEVICE_ENDIAN_LITTLE,
  AVAILABLE = CL_DEVICE_AVAILABLE,
  COMPILER_AVAILABLE = CL_DEVICE_COMPILER_AVAILABLE,
  EXECUTION_CAPABILITIES = CL_DEVICE_EXECUTION_CAPABILITIES,
  QUEUE_PROPERTIES = CL_DEVICE_QUEUE_PROPERTIES,
  QUEUE_ON_HOST_PROPERTIES = CL_DEVICE_QUEUE_ON_HOST_PROPERTIES,
  NAME = CL_DEVICE_NAME,
  VENDOR = CL_DEVICE_VENDOR,
  DRIVER_VERSION = CL_DRIVER_VERSION,
  PROFILE = CL_DEVICE_PROFILE,
  VERSION = CL_DEVICE_VERSION,
  EXTENSIONS = CL_DEVICE_EXTENSIONS,
  PLATFORM = CL_DEVICE_PLATFORM,
  DOUBLE_FP_CONFIG = CL_DEVICE_DOUBLE_FP_CONFIG,
  PREFERRED_VECTOR_WIDTH_HALF = CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF,
  HOST_UNIFIED_MEMORY = CL_DEVICE_HOST_UNIFIED_MEMORY,
  NATIVE_VECTOR_WIDTH_CHAR = CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR,
  NATIVE_VECTOR_WIDTH_SHORT = CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT,
  NATIVE_VECTOR_WIDTH_INT = CL_DEVICE_NATIVE_VECTOR_WIDTH_INT,
  NATIVE_VECTOR_WIDTH_LONG = CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG,
  NATIVE_VECTOR_WIDTH_FLOAT = CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT,
  NATIVE_VECTOR_WIDTH_DOUBLE = CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE,
  NATIVE_VECTOR_WIDTH_HALF = CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF,
  OPENCL_C_VERSION = CL_DEVICE_OPENCL_C_VERSION,
  LINKER_AVAILABLE = CL_DEVICE_LINKER_AVAILABLE,
  BUILT_IN_KERNELS = CL_DEVICE_BUILT_IN_KERNELS,
  IMAGE_MAX_BUFFER_SIZE = CL_DEVICE_IMAGE_MAX_BUFFER_SIZE,
  IMAGE_MAX_ARRAY_SIZE = CL_DEVICE_IMAGE_MAX_ARRAY_SIZE,
  PARENT_DEVICE = CL_DEVICE_PARENT_DEVICE,
  PARTITION_MAX_SUB_DEVICES = CL_DEVICE_PARTITION_MAX_SUB_DEVICES,
  PARTITION_PROPERTIES = CL_DEVICE_PARTITION_PROPERTIES,
  PARTITION_AFFINITY_DOMAIN = CL_DEVICE_PARTITION_AFFINITY_DOMAIN,
  PARTITION_TYPE = CL_DEVICE_PARTITION_TYPE,
  REFERENCE_COUNT = CL_DEVICE_REFERENCE_COUNT,
  PREFERRED_INTEROP_USER_SYNC = CL_DEVICE_PREFERRED_INTEROP_USER_SYNC,
  PRINTF_BUFFER_SIZE = CL_DEVICE_PRINTF_BUFFER_SIZE,
  IMAGE_PITCH_ALIGNMENT = CL_DEVICE_IMAGE_PITCH_ALIGNMENT,
  IMAGE_BASE_ADDRESS_ALIGNMENT = CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT,
  MAX_READ_WRITE_IMAGE_ARGS = CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS,
  MAX_GLOBAL_VARIABLE_SIZE = CL_DEVICE_MAX_GLOBAL_VARIABLE_SIZE,
  QUEUE_ON_DEVICE_PROPERTIES = CL_DEVICE_QUEUE_ON_DEVICE_PROPERTIES,
  QUEUE_ON_DEVICE_PREFERRED_SIZE = CL_DEVICE_QUEUE_ON_DEVICE_PREFERRED_SIZE,
  QUEUE_ON_DEVICE_MAX_SIZE = CL_DEVICE_QUEUE_ON_DEVICE_MAX_SIZE,
  MAX_ON_DEVICE_QUEUES = CL_DEVICE_MAX_ON_DEVICE_QUEUES,
  MAX_ON_DEVICE_EVENTS = CL_DEVICE_MAX_ON_DEVICE_EVENTS,
  SVM_CAPABILITIES = CL_DEVICE_SVM_CAPABILITIES,
  GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE = CL_DEVICE_GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE,
  MAX_PIPE_ARGS = CL_DEVICE_MAX_PIPE_ARGS,
  PIPE_MAX_ACTIVE_RESERVATIONS = CL_DEVICE_PIPE_MAX_ACTIVE_RESERVATIONS,
  PIPE_MAX_PACKET_SIZE = CL_DEVICE_PIPE_MAX_PACKET_SIZE,
  PREFERRED_PLATFORM_ATOMIC_ALIGNMENT = CL_DEVICE_PREFERRED_PLATFORM_ATOMIC_ALIGNMENT,
  PREFERRED_GLOBAL_ATOMIC_ALIGNMENT = CL_DEVICE_PREFERRED_GLOBAL_ATOMIC_ALIGNMENT,
  PREFERRED_LOCAL_ATOMIC_ALIGNMENT = CL_DEVICE_PREFERRED_LOCAL_ATOMIC_ALIGNMENT,
  IL_VERSION = CL_DEVICE_IL_VERSION,
  MAX_NUM_SUB_GROUPS = CL_DEVICE_MAX_NUM_SUB_GROUPS,
  SUB_GROUP_INDEPENDENT_FORWARD_PROGRESS = CL_DEVICE_SUB_GROUP_INDEPENDENT_FORWARD_PROGRESS
};

std::string SerializeParamValue(CLDeviceParam param, void *data) {
  std::string str;

  switch (param) {
    // cl_int
    case CLDeviceParam::VENDOR_ID:
    case CLDeviceParam::MAX_COMPUTE_UNITS:
    case CLDeviceParam::MAX_WORK_ITEM_DIMENSIONS:
    case CLDeviceParam::PREFERRED_VECTOR_WIDTH_CHAR:
    case CLDeviceParam::PREFERRED_VECTOR_WIDTH_SHORT:
    case CLDeviceParam::PREFERRED_VECTOR_WIDTH_INT:
    case CLDeviceParam::PREFERRED_VECTOR_WIDTH_LONG:
    case CLDeviceParam::PREFERRED_VECTOR_WIDTH_FLOAT:
    case CLDeviceParam::PREFERRED_VECTOR_WIDTH_DOUBLE:
    case CLDeviceParam::MAX_CLOCK_FREQUENCY:
    case CLDeviceParam::ADDRESS_BITS:
    case CLDeviceParam::MAX_READ_IMAGE_ARGS:
    case CLDeviceParam::MAX_WRITE_IMAGE_ARGS:
    case CLDeviceParam::MAX_SAMPLERS:
    case CLDeviceParam::MEM_BASE_ADDR_ALIGN:
    case CLDeviceParam::MIN_DATA_TYPE_ALIGN_SIZE:
    case CLDeviceParam::GLOBAL_MEM_CACHELINE_SIZE:
    case CLDeviceParam::MAX_CONSTANT_ARGS:
      str = std::to_string(*static_cast<cl_int*>(data));
      break;

    // cl_device_type
    case CLDeviceParam::TYPE:
      str = std::to_string(*static_cast<cl_int*>(data));
      break;

    //
/*
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
    case CLDeviceParam::
*/
  }
  return str;
}

std::string GetDeviceInformation(cl_device_id device, CLDeviceParam param) {
  cl_uint max_value_size = 0xFFFFFFFF;
  size_t value_size = 0; /*in bytes*/

  // The first goal is to find out quantity of these ids

  if (CL_SUCCESS != clGetDeviceInfo(device, static_cast<cl_device_info>(param),
      max_value_size, nullptr, &value_size))
  {
    return std::string();
  }

  // Then the memory should be allocated according to value size 
  auto value = std::malloc(value_size);
  if (!value) {
    return std::string();
  }

  if (CL_SUCCESS != clGetDeviceInfo(device, static_cast<cl_device_info>(param),
    max_value_size, value, &value_size)) {
    return std::string();
  }

  // Then we should serialize the value to a string :
  std::string std_value = SerializeParamValue(param, value);
  std::free(value);

  return std_value;
}

// The function writes the device ids to be allowed for current platform 
// of current type to the end of being passed vector

void GetPlatformDevices(cl_platform_id platform, CLDeviceType filter, std::vector<cl_device_id> &ids) {
   cl_uint max_entries_count = 0xFFFFFFFF;
   cl_uint num_devices = 0;
   // The first goal is to find out quantity of these ids
    if (CL_SUCCESS != clGetDeviceIDs(platform, static_cast<cl_device_type>(filter),
      max_entries_count, nullptr, &num_devices)) {
      return;
    }

    // Then the memory should be allocated according to devices count 
    std::vector<cl_device_id> device_ids(num_devices);
    if (CL_SUCCESS != clGetDeviceIDs(platform, static_cast<cl_device_type>(filter),
        max_entries_count, device_ids.data(), &num_devices)) {
      return;
    }

    // Write ids down to the end of the out-param vector
    ids.insert(ids.end(), device_ids.cbegin(), device_ids.cend());
}

std::string GetCLPlatfomParam(cl_platform_id platform, CLPlatformParams param) {
  const cl_int value_size = 256;
  size_t value_size_ret = 0;
  char value[value_size];

  if (CL_SUCCESS != clGetPlatformInfo(platform, static_cast<cl_platform_info>(param),
      value_size, value, &value_size_ret)) {
    return std::string();
  }

  value[++value_size_ret]='\0';
  std::string result(value);
  return result;
}

void RayTracer::Initialize() {
  // Initialize OpenCL library

  // 1) Get allowed platforms
  // Let's assume 10 as the max platforms count
  const cl_uint num_entries =10;
  cl_uint platforms_count = 0;
  cl_platform_id platform_ids[num_entries];

  if (CL_SUCCESS != clGetPlatformIDs(num_entries, platform_ids, &platforms_count)) {
    return;
  }

  // 2) Get allowed platforms info
  for (int i=0; i<platforms_count; ++i) {
    auto platform = platform_ids[i];
    std::string info;

    info += GetCLPlatfomParam(platform, CLPlatformParams::PROFILE);
    info += GetCLPlatfomParam(platform, CLPlatformParams::VERSION);
    info += GetCLPlatfomParam(platform, CLPlatformParams::NAME);
    info += GetCLPlatfomParam(platform, CLPlatformParams::VENDOR);
    info += GetCLPlatfomParam(platform, CLPlatformParams::EXTENTION) ;
  }

  // 3) Get devices ids of specified type for a platform
  std::vector<cl_device_id> dev_ids;
  cl_device_id *d1;
  for (int i = 0; i < platforms_count; ++i) {
    auto platform = platform_ids[i];
    GetPlatformDevices(platform, CLDeviceType::ALL, dev_ids);
    d1 = dev_ids.data();
  }
  auto d = dev_ids.data();

  // 4) Get information about certain device
  for (const auto& dev_id : dev_ids) {
    auto info = GetDeviceInformation(dev_id, CLDeviceParam::MAX_CLOCK_FREQUENCY);
    cerr << info;
  }

  auto i = dev_ids.size();
}

RayTracer::RayTracer() : imgBuffer_(*new Imagemap(500,400)) {
  scene_ = nullptr;
  //cameraPos = {0.0f , 0.0f , 100.0f};
  cameraPos = {0.0f , 0.0f , 100.0f};
  Initialize();
}

RayTracer::~RayTracer() {}


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
