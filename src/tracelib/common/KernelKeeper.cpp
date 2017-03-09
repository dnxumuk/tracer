#include "KernelKeeper.h"
#include <CL/cl.hpp>
#include <iterator>
#include <fstream>

KernelKeeper & KernelKeeper::getInstance() {
  static KernelKeeper keeper;
  return keeper;
}

cl::Kernel KernelKeeper::getKernel(const std::string &name) {
  auto result = kernels_.find(name);
  if (result != kernels_.cend())
      return result->second.getKernel();
  else
      throw "Kernel has not been found";
}

cl::Kernel KernelKeeper::buildKernel(const std::string &name) {
  auto result = kernels_.find(name);
  if (result == kernels_.cend())
      throw "Kernel has not been found";

  return result->second.build(name);
}

void KernelKeeper::addKernelToList(const std::string &kernel_name, const std::wstring &path, 
                                   const std::vector<cl::Device> &devices, const cl::Context &context) {
    kernels_.emplace(std::make_pair(kernel_name, KernelContext(path, devices, context)));
}

KernelKeeper::~KernelKeeper() {
}

KernelKeeper::KernelKeeper() {
#if 0
  _kernel_path_("C:\\Users\\Anton\\Desktop\\tracer\\tracer\\src\\tracelib\\cl_kernels\\hellworld.cl");
#else
 // _kernel_path = "E:\\sources\\tracer\\src\\tracelib\\cl_kernels\\hellworld.cl";
#endif
}

// Methods for maintaining an environment of a separate kernel

KernelContext::KernelContext() {
    is_rebuild_needed = false;
}

KernelContext::KernelContext(const std::wstring & path, const std::vector<cl::Device>& devices, 
                             const cl::Context & context) {
  setPath(path);
  setRelatedDevices(devices);
  setRelatedContext(context);
}

cl::Kernel KernelContext::getKernel() const {
    return built_kernel_;
}

void KernelContext::setPath(const std::wstring &path) {
    krnl_src_path_ = path;
    is_rebuild_needed = true;
}

std::wstring KernelContext::getPath() const {
    return krnl_src_path_;
}

void KernelContext::setRelatedDevices(const std::vector<cl::Device> &devices) {
    krnl_devices_ = devices;
    is_rebuild_needed = true;
}

void KernelContext::setRelatedContext(const cl::Context &context) {
    krnl_context_ = context;
    is_rebuild_needed = true;
}

cl::Kernel KernelContext::build(const std::string &krnl_name) {
    std::ifstream src_file(krnl_src_path_);
    if (!src_file.is_open())
        throw "Build kernel. Can not open kernel source file";
    if (krnl_devices_.size() < 1)
        throw "Build kernel. Invalid devices count";

    std::string prg_src(std::istreambuf_iterator<char>(src_file),
                       (std::istreambuf_iterator<char>()));
    cl::Program::Sources src(1, std::make_pair(prg_src.c_str(), prg_src.length() + 1));

    // Kernel has been read. Let's build it
    prg_ = cl::Program(krnl_context_, src);
    if (prg_.build(krnl_devices_) != CL_SUCCESS) {
        std::string buid_log = prg_.getBuildInfo<CL_PROGRAM_BUILD_LOG>(krnl_devices_[0]);
        throw buid_log;
    }

    cl_int err;
    cl::Kernel kernel(prg_, krnl_name.c_str(), &err);
    if (err != CL_SUCCESS)
        throw "Build kernel. Error creating kernel";

    built_kernel_ = kernel;
    return built_kernel_;
}
