#ifndef ANSI_PROCESSOR_HPP
#define ANSI_PROCESSOR_HPP

#include "processor_base.hpp"
#include "model-sys/screen_pixel.hpp"

#include <memory>
#include <stdint.h>
#include <string>
#include <vector>
#include <map>


/**
 * @class AnsiProcessor
 * @author Michael Griffin
 * @date 10/8/2015
 * @file ansi_processor.hpp
 * @brief Processes Screen data into a Screen Buffer
 */
class ProcessorAnsi
    : public ProcessorBase
{
public:

    ProcessorAnsi(int term_height, int term_width);

    virtual ~ProcessorAnsi() override;

    virtual std::string buildPullDownBars(int pulldown_id, bool active) override;
    virtual void clearPullDownBars() override;

    virtual int getMaxRowsUsedOnScreen() override;
    virtual std::string screenBufferParse() override;
    virtual std::string screenBufferToString() override;
    virtual void screenBufferDisplayTest() override;
    virtual int getMCIOffSet(std::string mci_code) override;
    virtual std::string getScreenFromBuffer(bool clearScreen) override;


    // Screen Buffer Modifiers
    virtual void screenBufferSetGlyph(const std::string &char_sequence) override;
    virtual void screenBufferScrollUp() override;
    virtual void screenBufferClearRange(int start, int end) override;
    virtual void screenBufferClear() override;
    virtual void clearScreen() override;
    virtual void parseTextToBuffer(char *buff) override;
    
    std::vector <ScreenPixel>  m_screen_buffer;
    std::map<int, ScreenPixel> m_pull_down_options;
    std::map<int, int>         m_line_ending_map;
    
    std::map<int, int> getLineEndingMap() const;

};

typedef std::shared_ptr<ProcessorAnsi> processor_ansi_ptr;

#endif // ANSI_PROCESSOR_HPP
