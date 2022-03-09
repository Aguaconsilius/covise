/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#include "string_util.h"
#include <ctype.h>
#include <cstring>
#include <cstdlib>

#include <sstream>

std::string
strip(const std::string &str)
{
    std::string retval = str;
    while (!retval.empty() && isspace(retval[retval.size() - 1]))
        retval.pop_back();

    size_t i = 0;
    for (; i < retval.size(); ++i)
    {
        if (!isspace(retval[i]))
            break;
    }
    return retval.substr(i);
}

std::string
strip(const char *ch)
{
    return strip(std::string(ch));
}

std::string
replace(const std::string &where, const char *what, const char *with, int times)
{
    std::string result(where);
    size_t len = strlen(with);
    std::string::size_type pos = 0;
    for (int i = 0; i < times || times < 0; ++i)
    {
        pos = result.find(what, pos);
        if (pos == std::string::npos)
            break;
        result.replace(pos, len, with);
        pos += len;
    }
    return result;
}

int
isIntNumber(const std::string &str)
{
    size_t cnt = 0;
    size_t size = str.size();
    for (size_t i = 0; i < size; ++i)
    {
        if (isdigit(str.c_str()[i]))
            cnt++;
    }
    if (cnt == size)
        return atoi(str.c_str());
    return -1;
}

std::vector<std::string> split(const std::string &str, char delimiter, bool skipEmptyParts)
{
    std::stringstream stream(str);
    std::string item;
    std::vector<std::string> rv;
    while (std::getline(stream, item, delimiter))
    {
        if (skipEmptyParts && item.empty())
            continue;
        rv.push_back(item);
    }
    return rv;
}

std::vector<std::string> split(const std::string &str, const std::regex &delimiter, bool skipEmptyParts)
{
    std::vector<std::string> rv;
    std::smatch m;
    auto s = str;
    while (std::regex_search(s, m, delimiter))
    {
        std::string p = m.prefix();
        if (!p.empty() || !skipEmptyParts)
            rv.push_back(p);
        s = m.suffix();
    }
    rv.push_back(s);

    return rv;
}

std::string toLower(const std::string &str)
{
    std::string lower;
    size_t size = str.length();
    for (int ch = 0; ch < size; ++ch)
        lower.push_back((unsigned char)tolower(str[ch]));
    return lower;
}

std::string toUpper(const std::string &str)
{
    std::string lower;
    size_t size = str.length();
    for (int ch = 0; ch < size; ++ch)
        lower.push_back((unsigned char)toupper(str[ch]));
    return lower;
}

std::string url_decode(const std::string &str, bool in_path)
{
    std::string decoded;
    decoded.reserve(str.size());
    for (std::size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] == '%')
        {
            if (i + 3 <= str.size())
            {
                int value = 0;
                std::istringstream is(str.substr(i + 1, 2));
                if (is >> std::hex >> value)
                {
                    decoded += static_cast<char>(value);
                    i += 2;
                }
                else
                {
                    decoded.clear();
                    break;
                }
            }
            else
            {
                decoded.clear();
                break;
            }
        }
        else if (in_path && str[i] == '+')
        {
            decoded += ' ';
        }
        else
        {
            decoded += str[i];
        }
    }
    return decoded;
}
