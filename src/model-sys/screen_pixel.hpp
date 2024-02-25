#ifndef SCREEN_PIXEL_HPP
#define SCREEN_PIXEL_HPP

#include <string>
#include <memory>

/**
 * @class ScreenPixel
 * @author Michael Griffin
 * @date 10/8/2015
 * @file ansi_processor.hpp
 * @brief Managers Characters and their attributes
 */
class ScreenPixel     
{
    
public:   
    explicit ScreenPixel()
        : char_sequence("\0")
        , x_position(1)
        , y_position(1)
        , attribute(0)
        , foreground(37) // FG_WHITE;
        , background(40) // BG_BLACK;
        , selected_attribute(0)
        , selected_foreground(0)
        , selected_background(0) 
    {}
    
    ~ScreenPixel() {}   
 
    std::string char_sequence;
    int x_position;
    int y_position;
    int attribute;
    int foreground;
    int background;
    int selected_attribute;
    int selected_foreground;
    int selected_background;

};

typedef std::shared_ptr<ScreenPixel> screen_pixel_ptr;

#endif // SCREEN_PIXEL_HPP
