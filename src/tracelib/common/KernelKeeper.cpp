#include "KernelKeeper.h"
#include <CL/cl.hpp>
#include <iterator>
#include <fstream>

cl::Program KernelKeeper::prg_ = cl::Program();
cl::Context KernelKeeper::context_ = cl::Context();
std::vector<cl::Device> KernelKeeper::devices_ = std::vector<cl::Device>();
std::string KernelKeeper::kernel_path_ = std::string();


KernelKeeper & KernelKeeper::getInstance() {
  static KernelKeeper keeper;
  return keeper;
}

void KernelKeeper::setPath(const std::string & path) {
  kernel_path_ = path;
}

void KernelKeeper::setDevices(const std::vector<cl::Device> &devices) {
  devices_ = devices;
}

void KernelKeeper::setContext(const cl::Context &context) {
  context_ = context;
}

cl::Kernel KernelKeeper::buildKernel() {
  // Read sources first
  std::ifstream src_file(kernel_path_);
  std::string prg_src(
    std::istreambuf_iterator<char>(src_file),
    (std::istreambuf_iterator<char>()));

  cl::Program::Sources src(1, std::make_pair(prg_src.c_str(), prg_src.length() + 1));
  prg_ = cl::Program(context_, src);

  if (prg_.build(devices_) != CL_SUCCESS) {
    // Error
    std::string buid_log = prg_.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices_[0]);
  }

  cl::Kernel kernel(prg_, "hello");
  return kernel;
}

cl::Kernel KernelKeeper::getKernel() {
  return cl::Kernel();
}

KernelKeeper::~KernelKeeper() {
}

KernelKeeper::KernelKeeper() {
#if 0
  _kernel_path_("C:\\Users\\Anton\\Desktop\\tracer\\tracer\\src\\tracelib\\cl_kernels\\hellworld.cl");
#else
  kernel_path_ = "E:\\sources\\tracer\\src\\tracelib\\cl_kernels\\hellworld.cl";
#endif
}


