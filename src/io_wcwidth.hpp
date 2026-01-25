#ifndef IO_WCWIDTH_HPP
#define IO_WCWIDTH_HPP

struct interval {
    int first;
    int last;
};

static int bisearch(wchar_t ucs, const interval *table, int max);

#endif
