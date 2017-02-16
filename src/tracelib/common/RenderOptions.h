#pragma once
#include <vector>
#include <string>
#include <map>

typedef std::map<std::string, std::string> DeviceSpec;
typedef std::vector<DeviceSpec> PlatformSpec;

class RenderOptions {
public:
  RenderOptions();
  void GetRenderPlatforms(std::vector<std::string> &list);
  void GetPlatfomInfo(size_t id, std::map<std::string,std::string> &dic);
  void GetDevices(int platform_id, PlatformSpec &spec);
  ~RenderOptions();
private:
  std::string GetPlatformItemName(const std::string &name, const std::string &vendor);
  void GetDeviceCPUInfo(cl::Device &dev, DeviceSpec &dic);

};

