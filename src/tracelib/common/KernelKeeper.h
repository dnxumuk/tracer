#pragma once
#include <string>
#include <vector>
#include <CL/cl.hpp>
//
//namespace cl {
//  class Device;
//  class Context;
//  class Kernel;
//  class Context;
//  class Program;
//}

class KernelKeeper {
public:
  static KernelKeeper & getInstance();
  
  static void setPath(const std::string &path);
  static void setDevices(const std::vector<cl::Device> &devices);
  static void setContext(const cl::Context &context);
  static cl::Kernel buildKernel();
  static cl::Kernel getKernel();
  ~KernelKeeper();

private:
  static KernelKeeper _instance;
  KernelKeeper();

  static std::string kernel_path_;
  static std::vector<cl::Device> devices_;
  static cl::Context context_;
  static cl::Program prg_;

  static bool is_rebuild_needed_;
};

