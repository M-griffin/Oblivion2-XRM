#ifndef ANSI_PROCESSOR_HPP
#define ANSI_PROCESSOR_HPP

#include "processor_base.hpp"

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "model-sys/screen_pixel.hpp"

/**
 * @class ProcessorAnsi
 * @author Michael Griffin
 * @date 10/8/2015
 * @file processor_ansi.hpp
 * @brief Processes Screen data into a Screen Buffer
 */
class ProcessorAnsi
        : public ProcessorBase {
public:
    explicit ProcessorAnsi();

    ~ProcessorAnsi();

    // Copy Constructors
    ProcessorAnsi(const ProcessorAnsi &) = delete;
    ProcessorAnsi &operator=(const ProcessorAnsi &) = delete;

    // Move Constructors
    ProcessorAnsi(ProcessorAnsi &&other) = delete;
    ProcessorAnsi &operator=(ProcessorAnsi &&) = delete;

    void resize(int term_height, int term_width);

    std::string buildPullDownBars(int pulldownId, bool active);

    void clearPullDownBars();

    int getMaxRowsUsedOnScreen();

    std::string screenBufferParse();

    std::string screenBufferToString();

    void screenBufferDisplayTest();

    int getMCIOffSet(std::string mci_code);

    std::string getScreenFromBuffer(bool clearScreen);

    // Screen Buffer Modifiers
    void screenBufferSetGlyph(const std::string &charSequence);

    void screenBufferScrollUp();

    void screenBufferClearRange(int start, int end);

    void screenBufferClear();

    void clearScreen();

    void parseTextToBuffer(char *buff);

    std::vector<ScreenPixel> m_screen_buffer;
    std::map<int, ScreenPixel> m_pull_down_options;
    std::map<int, int> m_line_ending_map;

    std::map<int, int> getLineEndingMap() const;
};

#endif // ANSI_PROCESSOR_HPP
