/**
 * @file ntrip_state.hpp
 * @brief NTRIP 连接状态枚举和错误码。
 * @copyright Copyright (C) 2026, LingAo Robotics, INC.
 */

#pragma once

#include <string>

namespace lingao::ntrip {

/// 连接状态
enum class NtripState {
    Disconnected,   ///< 未连接
    WaitingLLA,     ///< 等待首次 GGA 位置数据
    Connecting,     ///< 连接中 (TCP + NTRIP 认证)
    Running,        ///< 运行中 (正在接收 RTCM 数据)
    Reconnecting,   ///< 重连中
    Error,          ///< 错误 (认证失败等，不可恢复)
};

/// 错误码
enum class NtripError {
    None = 0,           ///< 无错误
    NetworkError,       ///< 网络不可达
    ConnectTimeout,     ///< 连接超时
    BadPassword,        ///< 账号或密码错误
    RequestError,       ///< NTRIP 请求被拒绝
    RecvTimeout,        ///< 接收超时 (断连检测)
    SocketError,        ///< Socket 异常
    ReconnectFailed,    ///< 重连次数用尽
};

/**
 * @brief 将 NtripState 转为可读字符串。
 */
inline const char *NtripStateToString(NtripState state) {
    switch (state) {
        case NtripState::Disconnected: return "Disconnected";
        case NtripState::WaitingLLA:   return "WaitingLLA";
        case NtripState::Connecting:   return "Connecting";
        case NtripState::Running:      return "Running";
        case NtripState::Reconnecting: return "Reconnecting";
        case NtripState::Error:        return "Error";
        default:                        return "Unknown";
    }
}

/**
 * @brief 将 NtripError 转为可读字符串。
 */
inline const char *NtripErrorToString(NtripError error) {
    switch (error) {
        case NtripError::None:            return "None";
        case NtripError::NetworkError:    return "NetworkError";
        case NtripError::ConnectTimeout:  return "ConnectTimeout";
        case NtripError::BadPassword:     return "BadPassword";
        case NtripError::RequestError:    return "RequestError";
        case NtripError::RecvTimeout:     return "RecvTimeout";
        case NtripError::SocketError:     return "SocketError";
        case NtripError::ReconnectFailed: return "ReconnectFailed";
        default:                           return "Unknown";
    }
}

}  // namespace lingao::ntrip
