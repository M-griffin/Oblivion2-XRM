#ifndef ANSI_PROCESSOR_HPP
#define ANSI_PROCESSOR_HPP

#include "screen_base.hpp"

#include <string>
#include <vector>
#include <map>
#include <regex>

#include "model-sys/screen_pixel.hpp"

/**
 * @class ScreenAnsiProc
 * @author Michael Griffin
 * @date 10/8/2015
 * @file screen_ansi_proc.hpp
 * @brief Processes Screen data into a Screen Buffer
 */
class ScreenAnsiProc
        : public ScreenBase {

    const std::regex LIGHT_BAR_EXPRESSION {"(\\|[0-9]{2}[%][0-9]{2})"};

public:
    explicit ScreenAnsiProc();
    ~ScreenAnsiProc();

    ScreenAnsiProc(const ScreenAnsiProc &) = delete;
    ScreenAnsiProc &operator=(const ScreenAnsiProc &) = delete;
    ScreenAnsiProc(ScreenAnsiProc &&other) = delete;
    ScreenAnsiProc &operator=(ScreenAnsiProc &&) = delete;

    void resize(int term_height, int term_width);
    std::string buildPullDownBars(int pullDownId, bool isSelected);
    void clearPullDownBars();
    int getMaxRowsUsedOnScreen();
    std::string screenBufferParse();
    std::string screenBufferToString();
    std::string getScreenFromBuffer(bool clearScreen);

    // Screen Buffer Modifiers
    void screenBufferSetGlyph(const std::string &charSequence);
    void screenBufferScrollUp();
    void screenBufferClearRange(int start, int end);
    void screenBufferClear();
    void clearScreen();
    void parseTextToBuffer(const std::string &buff);

    std::vector<ScreenPixel> m_screen_buffer;
    std::map<int, ScreenPixel> m_pull_down_options;
    std::map<int, int> m_line_ending_map;

    [[nodiscard]]
    std::map<int, int> getLineEndingMap() const;

    [[nodiscard]]
    int getPullDownMenuSize() const;

    [[nodiscard]]
    int getMaxLines() const;
};

#endif
