#ifndef PROCESSOR_TEXT_HPP
#define PROCESSOR_TEXT_HPP

#include "processor_base.hpp"

#include <memory>
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
    : public ProcessorBase
{
public:

    ProcessorText(int term_height, int term_width);
    
    int m_tab_width;
    int m_line_number;
    bool m_is_double_backspace;

    virtual ~ProcessorText() override;
    
    virtual std::string buildPullDownBars(int pulldown_id, bool active) override;
    virtual void clearPullDownBars() override;

    virtual int getMaxRowsUsedOnScreen() override;
    virtual std::string screenBufferParse() override;
    virtual std::string screenBufferToString() override;
    virtual void screenBufferDisplayTest() override;
    virtual int getMCIOffSet(std::string mci_code) override;
    virtual std::string getScreenFromBuffer(bool clearScreen) override;
    
    // Override for Testing Mesasge Box Redraw!
    std::string getScreenFromBuffer(bool clearScreen, int left_border);


    // Screen Buffer Modifiers
    virtual void screenBufferSetGlyph(const std::string &char_sequence) override;
    virtual void screenBufferScrollUp() override;
    virtual void screenBufferClearRange(int start, int end) override;
    virtual void screenBufferClear() override;

    virtual void clearScreen() override;
    virtual void parseTextToBuffer(char *buff) override;

    int getCurrentLine() const
    {
        return m_line_number;
    }
    
    bool isDoubleBackSpace() const
    {
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
                               
    std::vector <ScreenPixel>  m_screen_buffer;
    std::map<int, ScreenPixel> m_pull_down_options;
    std::map<int, int>         m_line_ending_map;
    
    std::map<int, int> getLineEndingMap() const;

};

typedef std::shared_ptr<ProcessorText> processor_text_ptr;

#endif // PROCESSOR_TEXT_HPP
