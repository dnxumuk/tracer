#include "RenderOptions.h"
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
  auto &platform = platforms[id];

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
  platforms[platform_id].getDevices(CL_DEVICE_TYPE_GPU, &dev_gpu);

  spec.reserve(dev_cpu.size() + dev_gpu.size());
  
  // Fill out the platform spec
  
  size_t dev_idx = 0;
  for (const auto &cpu : dev_cpu) {
    spec.emplace_back();
    GetDeviceCPUInfo(cpu, spec[dev_idx++]);
  }

  for (const auto &gpu : dev_gpu) {
    spec.emplace_back();
    GetDeviceGPUInfo(gpu, spec[dev_idx++]);
  }
}


RenderOptions::~RenderOptions() {}

std::string RenderOptions::GetPlatformItemName(const std::string & name, const std::string & vendor) {
  return name + "(" + vendor + ")";
}

void RenderOptions::GetDeviceCommonInfo(const cl::Device & dev, DeviceSpec & dic) {
  cl_int bits;
  dev.getInfo(CL_DEVICE_ADDRESS_BITS, &bits);
  dic[L"Bit"] = std::to_wstring(bits);

  cl_int units;
  dev.getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &units);
  dic[L"Cumpute units"] = std::to_wstring(units);

  size_t streams;
  dev.getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &streams);
  dic[L"Cumpute streams"] = std::to_wstring(streams);

  std::string name;
  dev.getInfo(CL_DEVICE_NAME, &name);
  dic[L"Name"] = std::wstring(name.cbegin(), name.cend());

  size_t frequency;
  dev.getInfo(CL_DEVICE_MAX_CLOCK_FREQUENCY, &frequency);
  dic[L"Frequency"] = std::to_wstring(frequency);


  size_t simd_length;
  dev.getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, &simd_length);
  dic[L"SIMD float dimensions"] = std::to_wstring(simd_length);

}

void RenderOptions::GetDeviceCPUInfo(const cl::Device &dev, DeviceSpec &dic) {
  GetDeviceCommonInfo(dev,dic);
  dic[L"Type"] = L"CPU";
}

void RenderOptions::GetDeviceGPUInfo(const cl::Device &dev, DeviceSpec &dic) {
  GetDeviceCommonInfo(dev, dic);
  dic[L"Type"] = L"GPU";
}
