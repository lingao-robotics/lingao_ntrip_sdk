/**
 * @file ntrip_receiver_manager.hpp
 * @brief NTRIP 接收器管理器 — 主公开 API (PIMPL 模式)。
 * @copyright Copyright (C) 2026, LingAo Robotics, INC.
 *
 * 用法示例:
 * @code
 *   lingao::ntrip::NtripConfig cfg;
 *   cfg.host       = "120.253.239.161";
 *   cfg.port       = 8002;
 *   cfg.user       = "user123";
 *   cfg.password   = "pass456";
 *   cfg.mountpoint = "RTCM33_GRCEJ";
 *
 *   lingao::ntrip::NtripReceiverManager ntrip(cfg);
 *   ntrip.SetStateCallback([](NtripState from, NtripState to) { ... });
 *   ntrip.SetRtcmCallback([](const uint8_t* data, size_t len) { ... });
 *   ntrip.Connect();
 *   ntrip.SetLocation(22.547, 114.086);  // 触发 WaitingLLA → Connecting
 * @endcode
 */

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

#include "lingao_ntrip_sdk/ntrip_config.hpp"
#include "lingao_ntrip_sdk/ntrip_state.hpp"

namespace lingao::ntrip {

/**
 * @brief NTRIP 接收器管理器。
 *
 * 管理完整的 NTRIP 生命周期:
 *   Connect() → WaitingLLA → Connecting → Running
 *
 * 所有内部实现 (TCP 连接, NTRIP 协议, 重连逻辑) 编译进库,
 * 本头文件仅暴露公开 API。
 */
class NtripReceiverManager {
public:
    explicit NtripReceiverManager(const NtripConfig &config);
    ~NtripReceiverManager();

    // 不可拷贝, 可移动
    NtripReceiverManager(const NtripReceiverManager &) = delete;
    NtripReceiverManager &operator=(const NtripReceiverManager &) = delete;
    NtripReceiverManager(NtripReceiverManager &&) noexcept;
    NtripReceiverManager &operator=(NtripReceiverManager &&) noexcept;

    // ==========================================================================
    // 生命周期
    // ==========================================================================

    /**
     * @brief 连接 NTRIP 服务 (异步)。
     *
     * 若配置中已有有效初始坐标, 直接进入 Connecting;
     * 否则进入 WaitingLLA, 等待 SetLocation() / SetGgaString() 调用。
     *
     * @return true 启动成功 (不代表已连接, 通过 StateCallback 通知)。
     */
    bool Connect();

    /**
     * @brief 断开连接, 停止工作线程。
     */
    void Disconnect();

    /**
     * @brief 从 Error 状态重置为 Disconnected。
     */
    void Reset();

    // ==========================================================================
    // 状态查询
    // ==========================================================================

    /// 获取当前连接状态
    NtripState GetState() const;

    /// 是否正在接收 RTCM 数据
    bool IsRunning() const;

    /// 获取最近一次错误
    NtripError GetLastError() const;

    // ==========================================================================
    // GGA 更新
    // ==========================================================================

    /**
     * @brief 设置位置坐标, 自动生成 GGA 语句。
     *
     * 在 WaitingLLA 状态下调用将触发进入 Connecting。
     * 在 Running 状态下调用将更新 GGA 用于下次定时上报。
     *
     * @param latitude  纬度 (WGS84, 度)
     * @param longitude 经度 (WGS84, 度)
     */
    void SetLocation(double latitude, double longitude);

    /**
     * @brief 直接设置 GGA 字符串 (用户自行生成)。
     *
     * 行为与 SetLocation 相同, 但跳过内部 GGA 生成。
     *
     * @param gga NMEA GPGGA 语句
     */
    void SetGgaString(const std::string &gga);

    // ==========================================================================
    // RTCM 数据
    // ==========================================================================

    /// RTCM 数据回调签名
    using RtcmCallback = std::function<void(const uint8_t *data, size_t len)>;

    /**
     * @brief 设置 RTCM 数据接收回调。
     *
     * 当从 NTRIP CORS 接收到 RTCM 数据时触发。
     * 回调在 SDK 内部线程中执行, 用户需自行保证线程安全。
     */
    void SetRtcmCallback(RtcmCallback cb);

    /**
     * @brief 从外部来源注入 RTCM 数据。
     *
     * 注入的数据也会触发 RtcmCallback, 用于非 NTRIP 来源的 RTCM 输入。
     *
     * @param data 原始 RTCM 数据指针
     * @param len  数据长度 (字节)
     */
    void WriteRtcmData(const uint8_t *data, size_t len);

    // ==========================================================================
    // 回调
    // ==========================================================================

    /// 状态变化回调签名
    using StateCallback = std::function<void(NtripState old_state, NtripState new_state)>;

    /**
     * @brief 设置状态变化回调。
     *
     * 每次状态转换时触发, 包括重连过程中的状态切换。
     */
    void SetStateCallback(StateCallback cb);

    /// 错误回调签名
    using ErrorCallback = std::function<void(NtripError error, const std::string &detail)>;

    /**
     * @brief 设置错误回调。
     *
     * 当发生错误时触发, 提供错误码和详细描述。
     */
    void SetErrorCallback(ErrorCallback cb);

    /// 日志级别
    enum class LogLevel { Debug, Info, Warn, Error };

    /// 日志回调签名
    using LogCallback = std::function<void(LogLevel level, const std::string &msg)>;

    /**
     * @brief 设置日志回调 (替代内部 printf)。
     */
    void SetLogCallback(LogCallback cb);

    // ==========================================================================
    // 运行时配置
    // ==========================================================================

    /**
     * @brief 运行时更新配置。
     *
     * 更新后会自动断开并重连 (如果当前已连接)。
     */
    void UpdateConfig(const NtripConfig &config);

    // ==========================================================================
    // SDK 信息
    // ==========================================================================

    /**
     * @brief 获取 SDK 版本字符串 (e.g. "1.0.0")。
     */
    static std::string SdkVersion();

    /**
     * @brief 获取 SDK 内部 Build 流水号。
     */
    static std::string SdkBuildNo();

    /**
     * @brief 获取 SDK Git 短 SHA。
     */
    static std::string SdkGitSha();

    /**
     * @brief 获取完整 SDK 版本字符串。
     *
     * 格式: "<version> [Build <build>] (<git_sha>)"
     */
    static std::string SdkVersionFull();

private:
    class Impl;                      ///< 前向声明, 实现编译进库
    std::unique_ptr<Impl> m_impl;    ///< PIMPL 指针
};

}  // namespace lingao::ntrip
