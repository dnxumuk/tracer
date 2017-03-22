#pragma once
#include <string>
#include <vector>
#include <map>
#include <CL/cl.hpp>

struct KernelContext {
public:
    KernelContext();
    KernelContext(const std::wstring &path, const std::vector<cl::Device> &devices, const cl::Context &context);

    cl::Kernel getKernel() const;
    cl::Context getContext() const;
    std::wstring getPath() const;
    std::vector<cl::Device> getDevices() const;

    void setPath(const std::wstring &path);
    void setRelatedDevices(const std::vector<cl::Device> &devices);
    void setRelatedContext(const cl::Context &context);

    cl::Kernel build(const std::string &krnl_name);
private:
    cl::Kernel built_kernel_;

    std::wstring krnl_src_path_;
    std::vector<cl::Device> krnl_devices_;
    cl::Context krnl_context_;
    cl::Program prg_;
    bool is_rebuild_needed;
};

class KernelKeeper {
public:
  static KernelKeeper & getInstance();
  void addKernelToList(const std::string &kernel_name, const std::wstring &path, const std::vector<cl::Device> &devices, const cl::Context &context);
  cl::Kernel getKernel(const std::string &name);
  cl::Context getContext(const std::string &name);
  std::vector<cl::Device> getDevices(const std::string &name);
  cl::Kernel buildKernel(const std::string &name);
  ~KernelKeeper();

private:
  static KernelKeeper _instance;
  KernelKeeper();
  std::map<std::string, KernelContext> kernels_;
  cl::Kernel buildKernel();
};

