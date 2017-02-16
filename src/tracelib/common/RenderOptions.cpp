#include "RenderOptions.h"

#include <CL/opencl.h>
#include <CL/cl.hpp>

#include <string>


RenderOptions::RenderOptions() {}

void RenderOptions::GetRenderPlatforms(std::vector<std::string>& list) {
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);
  for (auto &platform : platforms) {

    std::string name, vendor;
    platform.getInfo(CL_PLATFORM_NAME, &name);
    platform.getInfo(CL_PLATFORM_VENDOR, &vendor);

    list.push_back(GetPlatformItemName(name,vendor));
  }
}

void RenderOptions::GetPlatfomInfo(size_t id, std::map<std::string, std::string>& dic) {
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);

  if (id > platforms.size())
    return;
  auto platform = platforms[id];

  std::string value;
  platform.getInfo(CL_PLATFORM_PROFILE,&value);
  dic["Profile"] = value;

  platform.getInfo(CL_PLATFORM_VERSION, &value);
  dic["Version"] = value;

  platform.getInfo(CL_PLATFORM_NAME, &value);
  dic["Name"] = value;

  platform.getInfo(CL_PLATFORM_EXTENSIONS, &value);
  dic["Extensions"] = value;

  platform.getInfo(CL_PLATFORM_VENDOR, &value);
  dic["Vendor"] = value;

  platform.getInfo(CL_PLATFORM_ICD_SUFFIX_KHR, &value);
  dic["Suffix ICD"] = value;
  
}

void RenderOptions::GetDevices(int platform_id, PlatformSpec &spec) {
  // CPU and GPU devices only is assumed to be supported now

  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);

  if (platform_id > platforms.size())
    return;

  std::vector<cl::Device> dev_cpu;
  platforms[platform_id].getDevices(CL_DEVICE_TYPE_CPU, &dev_cpu);

  std::vector<cl::Device> dev_gpu;
  platforms[platform_id].getDevices(CL_DEVICE_TYPE_CPU, &dev_gpu);

}


RenderOptions::~RenderOptions() {}

std::string RenderOptions::GetPlatformItemName(const std::string & name, const std::string & vendor) {
  return name + "(" + vendor + ")";
}

void RenderOptions::GetDeviceCPUInfo(cl::Device &dev, DeviceSpec &dic) {
  // Header
  dic["Type"] = "CPU";
  //
  cl_int bits;
  dev.getInfo(CL_DEVICE_ADDRESS_BITS, &bits);
  dic["Bit"] = std::to_string(bits);

  cl_int units;
  dev.getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &units);
  dic["Cumpute units"] = std::to_string(units);

  cl_int streams;
  dev.getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &streams);
  dic["Cumpute streams"] = std::to_string(streams);

  std::string name;
  dev.getInfo(CL_DEVICE_NAME, &name);
  dic["Name"] = name;

  size_t frequency;
  dev.getInfo(CL_DEVICE_MAX_CLOCK_FREQUENCY, &frequency);
  dic["Frequency"] = frequency;



}
