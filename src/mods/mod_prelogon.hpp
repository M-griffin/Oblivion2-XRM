#ifndef MOD_PRELOGON_HPP
#define MOD_PRELOGON_HPP

#include "mod_base.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <functional>

class DeadlineTimer;
typedef std::shared_ptr<DeadlineTimer> deadline_timer_ptr;


/**
 * @class ModPreLogin
 * @author Michael Griffin
 * @date 3/17/2016
 * @file mod_logon.hpp
 * @brief System PreLogin Module
 */
class ModPreLogon
    : public std::enable_shared_from_this<ModPreLogon>
    , public ModBase
{
public:
    ModPreLogon(session_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process,
        common_io_ptr common_io, session_io_ptr session_io);

    virtual ~ModPreLogon() override
    {
        std::cout << "~ModPreLogon()" << std::endl;
        std::vector<std::function< void()> >().swap(m_setup_functions);
        std::vector<std::function< void(const std::string &)> >().swap(m_mod_functions);
    }

    virtual bool update(const std::string &character_buffer, const bool &) override;
    virtual bool onEnter() override;
    virtual bool onExit() override;

    // This matches the index for and key for setup -> mod_functions.push_back
    enum
    {
        MOD_DETECT_EMULATION,
        MOD_ASK_ANSI_COLOR,
        MOD_ASK_CODEPAGE
    };

    // Create Prompt Constants, these are the keys for key/value lookup
    const std::string PROMPT_DETECT_EMULATION = "detect_emu";
    const std::string PROMPT_DETECTED_ANSI = "detected_ansi";
    const std::string PROMPT_DETECTED_NONE = "detected_none";

    const std::string PROMPT_USE_ANSI = "use_ansi";
    const std::string PROMPT_USE_INVALID = "ansi_invalid";
    const std::string PROMPT_ANSI_SELECTED = "ansi_selected";
    const std::string PROMPT_ASCII_SELECTED = "ascii_selected";

    const std::string PROMPT_DETECT_TERMOPTS = "detect_term";
    const std::string PROMPT_DETECTED_TERM = "detected_term";
    const std::string PROMPT_DETECTED_SIZE = "detected_size";

    const std::string PROMPT_ASK_CP437 = "use_cp437";
    const std::string PROMPT_ASK_UTF8 = "use_utf8";
    const std::string PROMPT_CP437_SELECTED = "cp437_selected";
    const std::string PROMPT_UTF8_SELECTED = "utf8_selected";


    /**
     * @brief Create Default Text Prompts for module
     */
    void createTextPrompts();

    /**
     * @brief Sets an individual module index.
     * @param mod_function_index
     */
    void changeModule(int mod_function_index);

    /**
     * @brief Redisplay's the current module prompt.
     * @param mod_function_index
     */
    void redisplayModulePrompt();

    /**
     * @brief Pull and Display Prompts
     * @param prompt
     */
    void displayPrompt(const std::string &prompt);

    /**
     * @brief Pull and Display Prompts with following newline
     * @param prompt
     */
    void displayPromptAndNewLine(const std::string &prompt);

    /**
     * @brief Start ANSI ESC[6n ANSI Detection
     * @return
     */
    void setupEmulationDetection();

    /**
     * @brief Detection Completed, display results.
     * @return
     */
    void setupAskANSIColor();

    /**
     * @brief Displays Terminal Detection after Emulation Detection.
     */
    void displayTerminalDetection();

    /**
     * @brief Displays Terminal Detection before Asking CodePage.
     * @return
     */
    void setupDisplayTerminalDetection();

    /**
     * @brief Ask Setup CodePage CP437 / UTF-8
     * @return
     */
    void setupAskCodePage();

    /**
     * @brief Quick Timer Methods left in the Header.
     */

    /**
     * @brief Start ANSI Detection timer
     */
    void startDetectionTimer();

    /**
     * @brief Deadline Detection Timer for ANSI Detection
     * @param timer
     */
    void handleDetectionTimer();

    /**
     * @brief After Emulation Detection is completed
     * @param input
     */
    void emulationCompleted();

private:

    /**
     * @brief Detect ANSI Emulation
     * @return
     */
    bool emulationDetection(const std::string &input);

    /**
     * @brief ASK ANSI Color
     * @return
     */
    bool askANSIColor(const std::string &input);

    /**
     * @brief ASK CodePage CP437 / UTF-8
     * @return
     */
    bool askCodePage(const std::string &input);

    // Function Input Vector.
    std::vector<std::function< void()> >                    m_setup_functions;
    std::vector<std::function< void(const std::string &)> > m_mod_functions;

    text_prompts_dao_ptr   m_text_prompts_dao;
    deadline_timer_ptr     m_deadline_timer;

    int                    m_mod_function_index;
    bool                   m_is_text_prompt_exist;
    bool                   m_is_esc_detected;
    std::string            m_input_buffer;
    int                    m_x_position;
    int                    m_y_position;
    std::string            m_term_type;
};

#endif // MOD_PRELOGON_HPP
