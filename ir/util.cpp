#include "util.h"

std::deque<std::string> parseInterpolateString(const std::string& text) {
    std::deque<std::string> res;

    std::regex pattern("`(.+?)`");
    std::smatch match;
    for (std::sregex_iterator p(text.cbegin(), text.cend(), pattern), q; p != q;
         ++p) {
        res.push_back(p->format("$1"));
    }

    res.push_front(std::regex_replace(text, pattern, "%d"));
    return res;
}
