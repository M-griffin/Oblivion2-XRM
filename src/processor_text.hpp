#ifndef PROCESSOR_TEXT_HPP
#define PROCESSOR_TEXT_HPP

#include "processor_base.hpp"

#include <string>

#include "model-sys/screen_pixel.hpp"
#include "common_io.hpp"

/**
 * @class ProcessorText
 * @author Michael Griffin
 * @date 01/13/2019
 * @file processor_text.hpp
 * @brief Processes Text or Message Text into Screen Buffer Data
 */
class ProcessorText
        : public ProcessorBase {
public:
    ProcessorText(int term_height, int term_width);

    int m_tab_width;
    int m_line_number;
    bool m_is_double_backspace;

    ~ProcessorText();

    std::string buildPullDownBars(int pulldown_id, bool active);

    void clearPullDownBars();

    int getMaxRowsUsedOnScreen();

    std::string screenBufferParse();

    std::string screenBufferToString();

    void screenBufferDisplayTest();

    int getMCIOffSet(std::string mci_code);

    std::string getScreenFromBuffer(bool clearScreen);

    // Override for Testing Mesasge Box Redraw!
    std::string getScreenFromBuffer(bool clearScreen, int left_border);


    // Screen Buffer Modifiers
    void screenBufferSetGlyph(const std::string &char_sequence);

    void screenBufferScrollUp();

    void screenBufferClearRange(int start, int end);

    void screenBufferClear();

    void clearScreen();

    void parseTextToBuffer(char *buff);

    int getCurrentLine() const {
        return m_line_number;
    }

    bool isDoubleBackSpace() const {
        return m_is_double_backspace;
    }

    void setDoubleBackSpace(bool is_double_backspace) {
        m_is_double_backspace = is_double_backspace;
    }

    int getMaxCharactersPerLine() const {
        return m_characters_per_line;
    }

    // Processor Text Specific    
    void handleTextInput(const std::string &char_sequence);

    void moveHomePosition();

    void moveEndPosition();

    void moveNextXPosition();

    void moveNextYPosition();

    void movePreviousXPosition();

    void movePreviousYPosition();

    void moveBackSpace();

    void moveDelete();

    void moveNewLine();

    void moveTabWidth();

    void screenBufferScrollDown();

    // This could be shared.
    void escapeSequenceParsing(LocalizedBuffer &buffer,
                               std::string::iterator &it,
                               std::string::iterator &line_end);

    std::vector<ScreenPixel> m_screen_buffer;
    std::map<int, ScreenPixel> m_pull_down_options;
    std::map<int, int> m_line_ending_map;

    std::map<int, int> getLineEndingMap() const;
};

#endif // PROCESSOR_TEXT_HPP
