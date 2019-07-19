#include "clap.hpp"

#include <unordered_map>

namespace cla::internal
{
std::unordered_map<std::string_view, std::vector<std::string_view>> args;

void parse_windows(int argc, char const** argv) noexcept;
void parse_unix(int argc, char const** argv) noexcept;
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

bool cla::has(std::string_view flag) noexcept
{
    return internal::args.find(flag) != internal::args.end();
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
 * 1. For each arg get it as view
 * 2. If it has data ':', go to #4
 * 3. No data found? Store the flag
 * 4. Separate Flag and Data with different views
 * 5. Extract next data by finding the first between 'end of string' and ',' (next data)
 * 6. Repeat #5 until 'end of string'
 */
void cla::internal::parse_windows(int argc, char const** argv) noexcept
{
    for (int i = 0; i < argc; ++i)
    {
        std::string_view arg_view{argv[i]}; // #1 (get view)
        if (auto data_sep = arg_view.find(':'); data_sep != arg_view.npos) // #2 (has data)
        {
            std::vector<std::string_view> data;

            int slash_offset = arg_view[0] == '/' ? 1 : 0;

            auto flag_view = arg_view.substr(slash_offset, data_sep - slash_offset); // #4
            auto data_view = arg_view.substr(data_sep+1);    // #4

            auto next_data_sep = data_view.find(',');
            while (next_data_sep != data_view.npos) // #6
            {
                data.emplace_back(data_view.substr(0, next_data_sep)); // #5 (extract data)
                data_view = data_view.substr(next_data_sep+1);
                next_data_sep = data_view.find(',');
            }

            args.emplace(flag_view, std::move(data));
        }
        else // #3 (no data)
        {
            int slash_offset = arg_view[0] == '/' ? 1 : 0;
            auto flag_view = arg_view.substr(slash_offset, data_sep - slash_offset);
            args.emplace(flag_view, std::vector<std::string_view>{});
        }
    }
}

void cla::internal::parse_unix(int argc, char const** argv) noexcept
{

}