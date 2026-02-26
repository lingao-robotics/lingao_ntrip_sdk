# LingAo NTRIP Receiver SDK

灵遨科技 NTRIP CORS 差分数据接收器 SDK，用于从 NTRIP 服务获取 RTCM 差分数据。

## 特性

- **NTRIP CORS 连接**: 支持配置账号、密码、挂载点等
- **连接状态反馈**: 完整状态机，通过回调通知连接/断连/重连/错误
- **自动重连**: 指数退避策略，网络断开时自动重连
- **GGA 上报**: 自动生成和定时上报 GGA 语句
- **RTCM 数据注入**: 预留外部 RTCM 数据输入接口
- **闭源交付**: PIMPL 模式隐藏实现，仅暴露公开头文件
- **独立运行**: 不绑定 GNSS SDK，可选择配合使用

## 快速开始

```cpp
#include <lingao_ntrip_sdk/ntrip_receiver_manager.hpp>

lingao::ntrip::NtripConfig cfg;
cfg.host       = "120.253.239.161";
cfg.port       = 8002;
cfg.user       = "your_user";
cfg.password   = "your_pass";
cfg.mountpoint = "RTCM33_GRCEJ";

lingao::ntrip::NtripReceiverManager ntrip(cfg);

// 设置回调
ntrip.SetRtcmCallback([](const uint8_t* data, size_t len) {
    // 处理 RTCM 数据
});
ntrip.SetStateCallback([](auto from, auto to) {
    printf("状态: %s → %s\n", NtripStateToString(from), NtripStateToString(to));
});

// 启动 (进入 WaitingLLA 状态)
ntrip.Connect();

// 设置位置后自动连接 CORS
ntrip.SetLocation(22.547, 114.086);
```

## 与 XRTK SDK 配合使用

```cpp
#include <lingao_ntrip_sdk/ntrip_receiver_manager.hpp>
#include <lingao_xrtk_sdk/xrtk_device_manager.hpp>

// NTRIP 接收到 RTCM → 写入 GNSS 设备
ntrip.SetRtcmCallback([&gnss](const uint8_t* data, size_t len) {
    gnss.WriteRawData(data, len);
});

// GNSS 定位结果 → 更新 NTRIP GGA
nav->SetCallback([&ntrip](const BestNavData& d) {
    ntrip.SetLocation(d.latitude, d.longitude);
});
```

## 客户集成方式

### 1. 解压 SDK

```bash
tar -xzf lingao_ntrip_sdk.tar.gz
cd lingao_ntrip_sdk
```

### 2. CMake 集成（推荐）

客户工程 `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.14)
project(my_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(lingao_ntrip_sdk REQUIRED)

add_executable(my_app src/main.cpp)
target_link_libraries(my_app PRIVATE lingao::lingao_ntrip_sdk)
```

构建时指定 SDK 前缀目录:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/lingao_ntrip_sdk
cmake --build build -j$(nproc)
```

### 3. 运行时库路径

如果程序运行时报错找不到 `liblingao_ntrip_sdk.so.*`，请设置:

```bash
export LD_LIBRARY_PATH=/path/to/lingao_ntrip_sdk/lib/amd64:$LD_LIBRARY_PATH
```

`amd64` 请替换为实际架构目录（如 `arm64v8`）。

### 4. 手动链接（可选）

```cmake
target_include_directories(my_app PRIVATE /path/to/lingao_ntrip_sdk/include)
target_link_libraries(my_app PRIVATE /path/to/lingao_ntrip_sdk/lib/amd64/liblingao_ntrip_sdk.so)
```

## 版本

- SDK: 1.0.0
- 编译器: C++17 (GCC 8+)
- 系统: Linux (Ubuntu 20.04+)
