 #pragma once

#include <spdlog/spdlog.h>

namespace PositronEngine
{

    #ifdef NDEBUG

        #define LOG_INFORMATION(...)
        #define LOG_WARNING(...)
        #define LOG_ERROR(...)
        #define LOG_CRITICAL(...)

    #else

        #define LOG_INFORMATION(...)   spdlog::info(__VA_ARGS__)
        #define LOG_WARNING(...)    spdlog::warn(__VA_ARGS__)
        #define LOG_ERROR(...)  spdlog::error(__VA_ARGS__)
        #define LOG_CRITICAL(...)   spdlog::critical(__VA_ARGS__)

    #endif
}

