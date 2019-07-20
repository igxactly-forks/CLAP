#include "clap.hpp"

#include <unordered_map>
#include <algorithm>

namespace cla::internal
{
std::unordered_map<std::string_view, std::vector<std::string_view>> args;

void parse_windows(int argc, char const** argv) noexcept;
void parse_unix(int argc, char const** argv) noexcept;
void extract_data(std::string_view flag_view, std::string_view data_view) noexcept;
}

void cla::parse(int argc, char const** argv, cla::opt_t opt) noexcept
{
    switch (opt)
    {
    case cla::opt_t::WINDOWS:
        internal::parse_windows(argc, argv);
        break;
    case cla::opt_t::UNIX:
        internal::parse_unix(argc, argv);
        break;
    default:
        break;
    }
}

bool cla::has_flag(std::string_view flag) noexcept
{
    return internal::args.find(flag) != internal::args.end();
}

bool cla::has_data(std::string_view flag) noexcept
{
    auto* data = get(flag);
    return data && !data->empty();
}

std::vector<std::string_view> const* cla::get(std::string_view flag) noexcept
{
    if (auto search = internal::args.find(flag); search != internal::args.end())
        return &search->second;

    return nullptr;
}

/*
 * Flag starts with '/' and data follows ':'
 * If found, the first '/' of a flag is removed
 * 
 * 1. For each argument, get its view
 * 2. Remove the '/' prefix if needed
 * 3. If it has data, extract it
 * 4. Otherwise store the flag
 */
void cla::internal::parse_windows(int argc, char const** argv) noexcept
{
    for (int i = 0; i < argc; ++i)
    {
        std::string_view arg_view{argv[i]}; // #1 (get view)

        if (arg_view[0] == '/') // #2
            arg_view.remove_prefix(1);

        if (auto data_sep = arg_view.find(':'); data_sep != arg_view.npos) // #3 (has data)
        {
            auto flag_view = arg_view.substr(0, data_sep);
            auto data_view = arg_view.substr(data_sep+1);

            extract_data(flag_view, data_view);
        }
        else // #4 (no data)
        {
            args.emplace(arg_view, std::vector<std::string_view>{});
        }
    }
}

/*
 * Flag starts with '-' or '--'
 * Data follows '=' but only for flags with '--'
 * If a single '-' is found, the flag is composed of multiple single flags
 * 
 * 1. For each argument, get its view
 * 2. If it starts with '--', extract data (if found) and store the flag
 * 3. If it starts with '-', extract single flags and store them
 * 4. Without prefixes, store the flag
 */
void cla::internal::parse_unix(int argc, char const** argv) noexcept
{
    auto starts_with = [](std::string_view v, std::string_view prefix)
    {
        if (v.length() < prefix.length())
            return false;

        return std::equal(prefix.cbegin(), prefix.cend(), v.cbegin());
    };

    for (int i = 0; i < argc; ++i)
    {
        std::string_view arg_view{argv[i]}; // #1
        if (starts_with(arg_view, "--")) // #2
        {
            arg_view.remove_prefix(2);
            if (auto data_sep = arg_view.find("="); data_sep != arg_view.npos) // data found
            {
                auto flag_view = arg_view.substr(0, data_sep);
                auto data_view = arg_view.substr(data_sep + 1);

                extract_data(flag_view, data_view);
            }
            else
            {
                args.emplace(arg_view, std::vector<std::string_view>{});
            }
        }
        else if (starts_with(arg_view, "-")) // #3 (extracting single flags)
        {
            auto begin = arg_view.cbegin() + 1;
            auto end = arg_view.cend();

            while (begin != end)
                args.emplace(arg_view.substr(end - begin++, 1), std::vector<std::string_view>{});
        }
        else // #4
        {
            args.emplace(arg_view, std::vector<std::string_view>{});
        }
    }
}

/*
 * 1. While it has more data (',' found)
 *      a. Extract the data piece
 * 2. Extract the last data piece
 * 3. Store all the data with the flag
 */
void cla::internal::extract_data(std::string_view flag_view, std::string_view data_view) noexcept
{
    std::vector<std::string_view> data;

    auto next_data_sep = data_view.find(',');
    while (next_data_sep != data_view.npos) // #1
    {
        data.emplace_back(data_view.substr(0, next_data_sep)); // #1.a
        data_view = data_view.substr(next_data_sep + 1);
        next_data_sep = data_view.find(',');
    }

    data.emplace_back(data_view); // #2

    args.emplace(flag_view, std::move(data)); // #3
}