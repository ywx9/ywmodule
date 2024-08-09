/// \file d3d.hpp
/// \brief defined `class DirectX` to create a DirectX 12 window

#pragma once

#include "d3d12.h"
#include "dxgi1_4.h"
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include "comptr.hpp"
#include "windows.hpp"

export namespace yw::win {

using DXGI_ADAPTER_DESC1 = ::DXGI_ADAPTER_DESC1;
using DXGI_ADAPTER_FLAG = ::DXGI_ADAPTER_FLAG;

using ID3D12Device = ::ID3D12Device;
using IDXGIAdapter1 = ::IDXGIAdapter1;
using IDXGIFactory4 = ::IDXGIFactory4;
using IUnknown = ::IUnknown;

template<typename Device>
inline sl_t D3D12CreateDevice(
  IUnknown* pAdapter, D3D_FEATURE_LEVEL fl, Device*& ppDevice) {
  return ::D3D12CreateDevice(pAdapter, fl, __uuidof(Device), (void**)&ppDevice);
}

template<typename Factory>
inline sl_t CreateDXGIFactory1(Factory*& ppFactory) {
  return ::CreateDXGIFactory1(__uuidof(Factory), (void**)&ppFactory);
}

} // namespace yw::win

export namespace yw {

/// dxgi factory
inline ComPtr<win::IDXGIFactory4> DXGI_FACTORY = []{
  IDXGIFactory4* ptr = nullptr;
  auto hr = win::CreateDXGIFactory1(ptr);
  if (FAILED(hr)) {
    auto text = cvt<cat1>(hresult_message(hr));
    text = std::format("Failed to create DXGI factory: {}", text);
    logger.error(text);
    std::cerr << text << std::endl;
    return ComPtr<win::IDXGIFactory4>();
  } else return ComPtr<win::IDXGIFactory4>(ptr);
}();

/// dxgi adapter
inline ComPtr<win::IDXGIAdapter1> DXGI_ADAPTER = []{
  win::IDXGIAdapter1* ptr = nullptr;
  for (nat4 i{}; DXGI_FACTORY->EnumAdapters1(i, &ptr) != DXGI_ERROR_NOT_FOUND; ++i) {
    DXGI_ADAPTER_DESC1 desc;
    ptr->GetDesc1(&desc);
    if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
    win::ID3D12Device* device = nullptr;
    auto hr = win::D3D12CreateDevice(ptr, D3D_FEATURE_LEVEL_11_0, device);
    break;
  }
  auto hr = DXGI_FACTORY->EnumAdapters1(0, &ptr);
  if (FAILED(hr)) {
    auto text = cvt<cat1>(hresult_message(hr));
    text = std::format("Failed to create DXGI adapter: {}", text);
    logger.error(text);
    std::cerr << text << std::endl;
    return ComPtr<win::IDXGIAdapter1>();
  } else return ComPtr<win::IDXGIAdapter1>(ptr);
}();
}
