#include "clap.h"
#include "clap.hpp"

#include <unordered_map>

namespace cla::internal
{
std::unordered_map<std::string_view, std::string_view> args;
}

void cla_parse(int argc, char** argv)
{
    cla::parse(argc, argv);
}

void cla_parse_opt(int argc, char** argv, int opt)
{
    cla::parse(argc, argv, opt);
}

bool cla_has(char const* flag)
{
    return cla::has(flag);
}

char const* cla_get(char const* flag)
{
    return cla::get(flag).data(); // the data is null terminated
}

void cla::parse(int argc, char** argv, int opt) noexcept
{
    internal::args.clear();

    char data_sep = opt == WINDOWS ? ':' : '=';
    for (int i = 0; i < argc; ++i)
    {
        std::string_view main_arg(argv[i]);
        if (auto sep_pos = main_arg.find(data_sep); sep_pos != main_arg.npos)
        {
            auto flag = main_arg.substr(0, sep_pos);
            auto data = main_arg.substr(sep_pos);
            internal::args.emplace(flag, data);
        }
        internal::args.emplace(main_arg, std::string_view{});
    }
}

bool cla::has(std::string_view flag) noexcept
{
    return internal::args.find(flag) != internal::args.end();
}

std::string_view cla::get(std::string_view flag) noexcept
{
    if (auto search = internal::args.find(flag); search != internal::args.end())
        return search->second;

    return {};
}
