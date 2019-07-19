#pragma once

#include <string_view>
#include <vector>

#ifdef _MSC_VER
#define CLA_LIB_EXPORT __declspec(dllexport)
#else
#define CLA_LIB_EXPORT
#endif

namespace cla
{
enum class opt_t
{
    WINDOWS,
    UNIX,
};
/**
 * This option is used to specify the args convention.
 * In any case, multiple data can be passed to the same flag
 * by using a ',' delimiter.
 * If duplicates are found and data is present, it will be appended
 *
 * Windows Style starts with '/' and data follows ':'
 *
 *               i.e.: /std:c++17 /W3 /W4 /Wdisable:123,124,125
 *
 *               flag           data
 *               'std'              'c++17'
 *               'W3'               <empty>
 *               'W4'               <empty>
 *               'Wdisable' {123, 124, 125}
 *
 * Unix Style starts with '-' or '--' and data follows '='
 *            multiple letters after a single '-' will be
 *            parsed as multiple single flags
 *            data for single hyphen '-' flags is ignored
 *
 *            i.e.: --include=/usr/include,/etc/include -xvf --help -h -longflag -ext='.txt'
 *
 *            flag                 data
 *            'include' {/usr/include, /etc/include}
 *            'x'                            <empty>
 *            'v'                            <empty>
 *            'f'                            <empty>
 *            'help'                         <empty>
 *            'h'                            <empty>
 *            'l'                            <empty> <-- longflag
 *            'o'                            <empty>
 *            'n'                            <empty>
 *            'g'                            <empty>
 *            'a'                            <empty>
 *            'e'                            <empty> <-- ext='.txt'
 *            'x'                            <empty>
 *            't'                            <empty>
 **/
CLA_LIB_EXPORT void parse(int argc, char const** argv, opt_t opt) noexcept;

/**
 * Checks if a flag is present.
 **/
CLA_LIB_EXPORT bool has_flag(std::string_view flag) noexcept;

/**
 * Checks if a flag has data
 **/
CLA_LIB_EXPORT bool has_data(std::string_view flag) noexcept;

/**
 * Returns the data associated with the flag.
 * If no data is present, the vector is empty.
 * If no flag is found, nullptr is returned.
 **/
CLA_LIB_EXPORT std::vector<std::string_view> const* get(std::string_view flag) noexcept;
}