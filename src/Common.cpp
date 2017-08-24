// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com


#include "../include/analyzer/Common.hpp"

namespace analyzer::common
{
    void trim_left (std::string& str) noexcept
    {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int32_t, int32_t>(std::isspace))));
    }

    void trim_right (std::string& str) noexcept
    {
        str.erase(std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int32_t, int32_t>(std::isspace))).base(), str.end());
    }

    void trim (std::string& str) noexcept
    {
        trim_left(str);
        trim_right(str);
    }


    std::vector<std::string> split (const std::string& str, char delimiter) noexcept
    {
        std::vector<std::string> result;
        split(str, delimiter, std::back_inserter(result));
        return result;
    }

}  // namespace common.