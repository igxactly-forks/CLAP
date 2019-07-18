#pragma once

#include <string_view>

#ifdef _MSC_VER
#define CLA_LIB_EXPORT __declspec(dllexport)
#else
#define CLA_LIB_EXPORT
#endif

namespace cla
{
enum
{
    WINDOWS = 1,
    UNIX = 2
};

CLA_LIB_EXPORT void parse(int argc, char** argv, int opt = UNIX) noexcept;
CLA_LIB_EXPORT bool has(std::string_view flag) noexcept;
CLA_LIB_EXPORT std::string_view get(std::string_view flag) noexcept;
}