#pragma once
#include <spdlog/common.h>
#include <spdlog/fmt/ranges.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// template <typename... Args>
// inline void LTRACE(fmt::format_string<Args...> fmt, const Args&... args) {
//     SPDLOG_TRACE(fmt, args...);
// }
// template <typename... Args>
// inline void LDEBUG(fmt::format_string<Args...> fmt, const Args&... args) {
//     SPDLOG_DEBUG(fmt, args...);
// }
// template <typename... Args>
// inline void LINFO(fmt::format_string<Args...> fmt, const Args&... args) {
//     SPDLOG_INFO(fmt, args...);
// }
// template <typename... Args>
// inline void LWARN(fmt::format_string<Args...> fmt, const Args&... args) {
//     SPDLOG_WARN(fmt, args...);
// }
// template <typename... Args>
// inline void LERR(fmt::format_string<Args...> fmt, const Args&... args) {
//     SPDLOG_ERROR(fmt, args...);
// }
// template <typename... Args>
// inline void LCRIT(fmt::format_string<Args...> fmt, const Args&... args) {
//     SPDLOG_CRITICAL(fmt, args...);
// }
#define LTRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LDEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LINFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LWARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LERR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LCRIT(...) SPDLOG_CRIT(__VA_ARGS__)
#define LCRITRET(ret, ...)  \
    {                       \
        LCRIT(__VA_ARGS__); \
        return ret;         \
    }

inline void _init_log() {
    spdlog::set_default_logger(spdlog::stdout_color_mt("console"));
    spdlog::set_level(spdlog::level::level_enum::trace);
    spdlog::set_pattern("%^(%L%L)%$ %v");  // (DD) Debug message
}
