#ifndef CELL_HPP
#define CELL_HPP

#include <string>

/**
 * @struct Cell
 * Represents a single character cell for ANSI output.
 */
struct Cell {
    std::string utf8_char;
    int fg_color = -1;
    int bg_color = -1;
    bool bold = false;
    bool inverse = false;
    bool lightbar = false;
    size_t width = 1;
};


#endif
