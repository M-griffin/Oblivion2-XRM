#include "io_wcwidth.hpp"

#include <string>
#include <cwchar>

// Markus Kuhn wcwidth()
static int bisearch(wchar_t ucs, const interval *table, int max) {
    int min = 0, mid;
    if (ucs < table[0].first || ucs > table[max].last) return 0;
    while (max >= min) {
        mid = (min + max) / 2;
        if (ucs > table[mid].last) min = mid + 1;
        else if (ucs < table[mid].first) max = mid - 1;
        else return 1;
    }
    return 0;
}

int mk_wcwidth(wchar_t ucs) {
    static const struct interval combining[] = {
        {0x0300, 0x036F}, {0x0483, 0x0489}, {0x0591, 0x05BD},
        {0x05BF, 0x05BF}, {0x05C1, 0x05C2}, {0x05C4, 0x05C5},
        {0x05C7, 0x05C7}, {0x0600, 0x0603}, {0x0610, 0x0615},
        {0x064B, 0x065E}, {0x0670, 0x0670}, {0x06D6, 0x06E4},
        {0x06E7, 0x06E8}, {0x06EA, 0x06ED}, {0x070F, 0x070F},
        {0x0711, 0x0711}, {0x0730, 0x074A}, {0x07A6, 0x07B0},
        {0x07EB, 0x07F3}, {0x0901, 0x0902}, {0x093C, 0x093C},
        {0x0941, 0x0948}, {0x094D, 0x094D}, {0x0951, 0x0954},
        {0x0962, 0x0963}, {0x0981, 0x0981}, {0x09BC, 0x09BC},
        {0x09C1, 0x09C4}, {0x09CD, 0x09CD}, {0x09E2, 0x09E3}
    };
    if (ucs == 0) return 0;
    if (ucs < 32 || (ucs >= 0x7f && ucs < 0xa0)) return -1;
    if (bisearch(ucs, combining, sizeof(combining) / sizeof(struct interval) - 1))
        return 0;
    if ((ucs >= 0x1100 &&
         (ucs <= 0x115f || ucs == 0x2329 || ucs == 0x232a ||
          (ucs >= 0x2e80 && ucs <= 0xa4cf && ucs != 0x303f) ||
          (ucs >= 0xac00 && ucs <= 0xd7a3))))
        return 2;
    return 1;
}

size_t utf8_width(const std::string &utf8_char) {
    if (utf8_char.empty()) return 0;
    mbstate_t state = {};
    wchar_t wc;
    size_t len = mbrtowc(&wc, utf8_char.c_str(), utf8_char.size(), &state);
    if (len == static_cast<size_t>(-1) || len == static_cast<size_t>(-2)) return 1;
    const int w = mk_wcwidth(wc);
    return w > 0 ? static_cast<size_t>(w) : 1;
}