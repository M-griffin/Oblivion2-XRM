#ifndef PROCESSOR_TEXT_HPP
#define PROCESSOR_TEXT_HPP

#include "processor_base.hpp"

#include <memory>
#include <string>


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

    ProcessorText(int term_height, int term_width)
        : ProcessorBase(term_height, term_width)
    { }

    virtual ~ProcessorText() {}

    virtual std::string buildPullDownBars(int pulldown_id, bool active) override;
    virtual void clearPullDownBars() override;

    virtual int getMaxRowsUsedOnScreen() override;
    virtual std::string screenBufferParse() override;
    virtual std::string screenBufferToString() override;
    virtual void screenBufferDisplayTest() override;
    virtual int getMCIOffSet(std::string mci_code) override;
    virtual std::string getScreenFromBuffer(bool clearScreen) override;


    // Screen Buffer Modifiers
    virtual void screenBufferSetGlyph(std::string char_sequence) override;
    virtual void screenBufferScrollUp() override;
    virtual void screenBufferClearRange(int start, int end) override;
    virtual void screenBufferClear() override;
    virtual void clearScreen() override;
    virtual void parseTextToBuffer(char *buff) override;

};

typedef std::shared_ptr<ProcessorText> processor_text_ptr;

#endif // PROCESSOR_TEXT_HPP
