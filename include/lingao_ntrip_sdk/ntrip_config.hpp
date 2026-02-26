/**
 * @file ntrip_config.hpp
 * @brief NTRIP CORS 连接配置结构体。
 * @copyright Copyright (C) 2026, LingAo Robotics, INC.
 */

#pragma once

#include <cstdint>
#include <string>

namespace lingao::ntrip {

/// NTRIP CORS 服务配置
struct NtripConfig {
    // ==================== CORS 服务器 ====================

    std::string host;                ///< CORS 服务器 IP/域名
    uint16_t port = 8002;            ///< 端口
    std::string user;                ///< 账号
    std::string password;            ///< 密码
    std::string mountpoint;          ///< 挂载点

    // ==================== GGA 设置 ====================

    double latitude = 0.0;           ///< 初始纬度 (WGS84)
    double longitude = 0.0;          ///< 初始经度 (WGS84)
    int gga_report_interval_s = 1;   ///< GGA 上报间隔 (秒)

    // ==================== 重连设置 ====================

    bool auto_reconnect = true;                ///< 网络断开时自动重连
    uint32_t reconnect_interval_ms = 3000;     ///< 初始重连间隔 (ms)
    uint32_t max_reconnect_interval_ms = 5000;  ///< 最大重连间隔 (指数退避上限, 5s)
    uint32_t max_reconnect_attempts = 0;       ///< 最大重连次数, 0 = 无限重试

    // ==================== 超时设置 ====================

    uint32_t connect_timeout_ms = 5000;   ///< TCP 连接超时 (ms)
    uint32_t recv_timeout_ms = 10000;     ///< 数据接收超时, 无数据判定断连 (ms)
};

}  // namespace lingao::ntrip
