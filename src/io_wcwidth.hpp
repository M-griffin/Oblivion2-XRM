#ifndef IO_WCWIDTH_HPP
#define IO_WCWIDTH_HPP

#include <string>

struct interval {
    int first;
    int last;
};

static int bisearch(wchar_t ucs, const interval *table, int max);

int mk_wcwidth(wchar_t ucs);

size_t utf8_width(const std::string &utf8_char);

#endif
