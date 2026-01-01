#ifndef MOD_PRELOGON_HPP
#define MOD_PRELOGON_HPP

#include "mod_base.hpp"
#include "../data-sys/text_prompts_dao.hpp"

#include <string>
#include <vector>
#include <functional>

/**
 * @class ModPreLogon
 * @author Michael Griffin
 * @date 3/17/2016
 * @file mod_prelogon.hpp
 * @brief System PreLogin Module
 */
class ModPreLogon
        : public ModBase {
    TextPromptsDao m_text_prompts_dao;

    int m_mod_function_index;
    bool m_is_text_prompt_exist;
    bool m_is_esc_detected;
    bool m_is_human_shield;
    std::string m_input_buffer;
    int m_x_position;
    int m_y_position;
    std::string m_term_type;
    std::string m_esc_sequence;

    // State Function Holders
    std::vector<std::function<void()> > m_setup_functions;
    std::vector<std::function<void(const std::string &)> > m_mod_functions;

public:
    ModPreLogon(TCPSession &session_data, Config &config, ProcessorAnsi &ansi_process,
                CommonIO &common_io, SessionIO &session_io);

    ~ModPreLogon();

    // Disable copy semantics
    ModPreLogon(const ModPreLogon &) = delete;

    ModPreLogon &operator=(const ModPreLogon &) = delete;

    // Move constructor
    ModPreLogon(ModPreLogon &&other) noexcept;

    ModPreLogon &operator=(ModPreLogon &&other) noexcept;

    // State Driven Methods
    bool update(const std::string &character_buffer, const bool &) const;

    bool onEnter();

    bool onExit();

    // This matches the index for and key for setup -> mod_functions.push_back
    enum {
        MOD_HUMAN_SHIELD,
        MOD_DETECT_EMULATION,
        MOD_ASK_ANSI_COLOR,
        MOD_ASK_CODEPAGE
    };

    std::string MOD_FILENAME = "mod_prelogon.yaml";

    // Create Prompt Constants, these are the keys for key/value lookup
    const std::string PROMPT_HUMAN_SHIELD = "human_shield";
    const std::string PROMPT_HUMAN_SHIELD_SUCCESS = "human_shield_success";
    const std::string PROMPT_HUMAN_SHIELD_FAIL = "human_shield_fail";

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
     */
    void redisplayModulePrompt() const;

    /**
     * @brief Pull and Display Prompts
     * @param prompt
     */
    void displayPrompt(const std::string &prompt);

    /**
     * @brief Pull and Display Prompts, Then Disconnect
     * @param prompt
     */
    void displayPromptThenDisconnect(const std::string &prompt);

    /**
     * @brief Pull and Display Prompts with following newline
     * @param prompt
     */
    void displayPromptAndNewLine(const std::string &prompt);

    /**
     * @brief Startup Human Shield, ESC Twice detection.
     * @return
     */
    void setupHumanShield();

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
     * @brief Displays Terminal Detection before Asking CodePage.
     * @return
     */
    //void setupDisplayTerminalDetection();

    /**
     * @brief Displays Terminal Detection after Emulation Detection.
     */
    void displayTerminalDetection();

    /**
     * @brief Ask Setup CodePage CP437 / UTF-8
     * @return
     */
    void setupAskCodePage();

    /**
     * @brief Quick Timer Methods left in the Header.
     */

    /**
     * @brief Start ESC Twice Timer
     */
    void startHumanShieldTimer();

    /**
     * @brief Deadline Detection Timer ESC Twice
     */
    void handleHumanShieldTimer();

    /**
     * @brief ESC Twice Completed
     * @return
     */
    void humanShieldCompleted();

    /**
     * @brief Start ANSI Detection timer
     */
    void startDetectionTimer();

    /**
     * @brief Deadline Detection Timer for ANSI Detection
     */
    void handleDetectionTimer();

    /**
     * @brief After Emulation Detection is completed
     */
    void emulationCompleted();

    /**
     * @brief Were Detecting ESC Twice Here.
     * @return
     */
    bool humanShieldDetection(const std::string &input);

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
};

#endif
