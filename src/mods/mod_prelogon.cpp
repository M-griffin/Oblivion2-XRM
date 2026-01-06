#include "mod_prelogon.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <chrono>

#include "mod_base.hpp"

#include "../data-sys/text_prompts_dao.hpp"
#include "../model-sys/structures.hpp"
#include "../model-sys/config.hpp"

#include "../encoding.hpp"
#include "../logging.hpp"
#include "../session_io.hpp"
#include "../tcp_session.hpp"
#include "../common_io.hpp"

ModPreLogon::ModPreLogon(Context &ctx)
    : ModBase(ctx, MOD_FILENAME)
      , m_text_prompts_dao(GLOBAL_DATA_PATH, MOD_FILENAME)
      , m_mod_function_index(MOD_HUMAN_SHIELD)
      , m_is_text_prompt_exist(false)
      , m_is_esc_detected(false)
      , m_is_human_shield(false)
      , m_x_position(0)
      , m_y_position(0)
      , m_term_type("undetected") {
    m_setup_functions.emplace_back([this] { setupHumanShield(); });
    m_setup_functions.emplace_back([this] { setupEmulationDetection(); });
    m_setup_functions.emplace_back([this] { setupAskANSIColor(); });
    m_setup_functions.emplace_back([this] { setupAskCodePage(); });

    m_mod_functions.emplace_back(
        [this](const std::string &s) { return humanShieldDetection(s); });
    m_mod_functions.emplace_back(
        [this](const std::string &s) { return emulationDetection(s); });
    m_mod_functions.emplace_back(
        [this](const std::string &s) { return askANSIColor(s); });
    m_mod_functions.emplace_back(
        [this](const std::string &s) { return askCodePage(s); });

    m_is_text_prompt_exist = m_text_prompts_dao.fileExists();
    if (!m_is_text_prompt_exist) {
        createTextPrompts();
    }

    m_text_prompts_dao.readPrompts();
}

ModPreLogon::~ModPreLogon() {
    m_setup_functions.clear();
    m_mod_functions.clear();
}

void ModPreLogon::rebuildFunctionTables() {
    m_setup_functions.clear();
    m_mod_functions.clear();

    m_setup_functions.emplace_back([this] { setupHumanShield(); });
    m_setup_functions.emplace_back([this] { setupEmulationDetection(); });
    m_setup_functions.emplace_back([this] { setupAskANSIColor(); });
    m_setup_functions.emplace_back([this] { setupAskCodePage(); });

    m_mod_functions.emplace_back(
        [this](const std::string &s) { return humanShieldDetection(s); });
    m_mod_functions.emplace_back(
        [this](const std::string &s) { return emulationDetection(s); });
    m_mod_functions.emplace_back(
        [this](const std::string &s) { return askANSIColor(s); });
    m_mod_functions.emplace_back(
        [this](const std::string &s) { return askCodePage(s); });
}

// Move constructor
ModPreLogon::ModPreLogon(ModPreLogon &&other) noexcept
    : ModBase(std::move(other))
      , m_text_prompts_dao(std::move(other.m_text_prompts_dao))
      , m_mod_function_index(other.m_mod_function_index)
      , m_is_text_prompt_exist(other.m_is_text_prompt_exist)
      , m_is_esc_detected(other.m_is_esc_detected)
      , m_is_human_shield(other.m_is_human_shield)
      , m_input_buffer(std::move(other.m_input_buffer))
      , m_x_position(other.m_x_position)
      , m_y_position(other.m_y_position)
      , m_term_type(std::move(other.m_term_type))
      , m_esc_sequence(std::move(other.m_esc_sequence)) {
    rebuildFunctionTables();

    // Leave other in valid state
    other.m_mod_function_index = 0;
    other.m_is_text_prompt_exist = false;
    other.m_is_esc_detected = false;
    other.m_is_human_shield = false;
    other.m_x_position = 0;
    other.m_y_position = 0;
}

// Move assignment operator
ModPreLogon &ModPreLogon::operator=(ModPreLogon &&other) noexcept {
    if (this != &other) {
        ModBase::operator=(std::move(other));

        m_text_prompts_dao = std::move(other.m_text_prompts_dao);
        m_mod_function_index = other.m_mod_function_index;
        m_is_text_prompt_exist = other.m_is_text_prompt_exist;
        m_is_esc_detected = other.m_is_esc_detected;
        m_is_human_shield = other.m_is_human_shield;
        m_input_buffer = std::move(other.m_input_buffer);
        m_x_position = other.m_x_position;
        m_y_position = other.m_y_position;
        m_term_type = std::move(other.m_term_type);
        m_esc_sequence = std::move(other.m_esc_sequence);

        rebuildFunctionTables();

        other.m_mod_function_index = 0;
        other.m_is_text_prompt_exist = false;
        other.m_is_esc_detected = false;
        other.m_is_human_shield = false;
        other.m_x_position = 0;
        other.m_y_position = 0;
    }
    return *this;
}

/**
 * @brief Handles Updates or Data Input from Client
 * @return bool, not used anymore?!?
 */
bool ModPreLogon::update(const std::string &character_buffer, const bool &) const {
    // Make sure system is active, when system is done, success or fails
    // We change this is inactive to single the login process is completed.
    if (!isModuleActive()) {
        return false;
    }

    // Return True when were keeping module active / else false;
    if (character_buffer.empty()) {
        return true;
    }

    // Process all incoming data straight to the input functions.
    m_mod_functions[m_mod_function_index](character_buffer);

    return true;
}

/**
 * @brief Startup class, setup and display initial screens / interface.
 * @return
 */
bool ModPreLogon::onEnter() {
    // On Initial Startup, setup user record with system colors for menu system
    // this is overwritten once the user logs in, otherwise the menu system
    // will use these defaults for theming.
    m_ctx.getUser().sRegColor = m_ctx.getCfg().default_color_regular;
    m_ctx.getUser().sPromptColor = m_ctx.getCfg().default_color_prompt;
    m_ctx.getUser().sInputColor = m_ctx.getCfg().default_color_input;
    m_ctx.getUser().sInverseColor = m_ctx.getCfg().default_color_inverse;
    m_ctx.getUser().sStatColor = m_ctx.getCfg().default_color_stat;
    m_ctx.getUser().sBoxColor = m_ctx.getCfg().default_color_box;

    // Setup Module Startup
    setModuleActive();

    // Grab ANSI Screen, display, if desired. logon.ans maybe?
    std::string prompt = "\x1b[?25h"; // Turn on Cursor.
    baseProcessAndDeliver(prompt);

    // Execute the initial setup index.
    m_setup_functions[m_mod_function_index]();

    return true;
}

/**
 * @brief Exit, close down, display screens to change over data.
 * @return
 */
bool ModPreLogon::onExit() {
    setModuleInActive();
    return true;
}

/**
 * @brief Create Default Text Prompts for module
 */
void ModPreLogon::createTextPrompts() {
    // Create Mapping to pass for file creation (default values)
    M_TextPrompt value;

    value[PROMPT_HUMAN_SHIELD] = std::make_pair("Hit [ESC] twice to continue",
                                                "|CR|12<|04Human Shield|12>|08: |15Hit |08[|12ESC|08] |15twice within |124 |15seconds to continue!");
    value[PROMPT_HUMAN_SHIELD_SUCCESS] = std::make_pair("ESC Detection Successful",
                                                        "|CR |08- |15Input Detected|08, |15Loading |08- |151 Moment \x1b[0m");
    value[PROMPT_HUMAN_SHIELD_FAIL] = std::make_pair("No Human Detected",
                                                     "|CR |08- |15No Input Detected|08, |15Disconnecting|08...  \x1b[0m");

    value[PROMPT_DETECT_EMULATION] = std::make_pair("Detecting Emulation", "|09Detecting Emulation");
    value[PROMPT_DETECTED_ANSI] = std::make_pair("Emulation Detected: Ansi ESC Supported",
                                                 "|CR|15Emulation Detected: |03ANSI ESC Supported.");
    value[PROMPT_DETECTED_NONE] = std::make_pair("Emulation Detected: None", "|CR|15Emulation Detect: |03none");

    value[PROMPT_USE_ANSI] = std::make_pair("Use ANSI Colors (Y/n) ",
                                            "|CRPress [y/ENTER or n] to use ANSI Colors or to Select ASCII No Colors: ");
    value[PROMPT_USE_INVALID] = std::make_pair("Invalid Response to Y/N/ENTER", "|04Invalid Response! Try again.");
    value[PROMPT_ANSI_SELECTED] = std::make_pair("ANSI Color Selected", "Selected: |03Ansi.");
    value[PROMPT_ASCII_SELECTED] = std::make_pair("ASCII No Colors Selected", "Selected: None.");

    value[PROMPT_DETECT_TERMOPTS] = std::make_pair("Detecting Terminal Options", "|CR|CR|09Detecting Terminal Options");
    value[PROMPT_DETECTED_TERM] = std::make_pair("Detecting Terminal: |OT ", "|CR|15Detected Terminal Type: |03|OT");
    value[PROMPT_DETECTED_SIZE] = std::make_pair("Detecting Terminal Size: |OT ", "|CR|15Detected Screen Size: |03|OT");

    value[PROMPT_ASK_CP437] = std::make_pair("Use CP437 Output Encoding",
                                             "|CR|CR|15Detected Default Encoding |03CP-437|15 |CR[ENTER] to Continue, or [N] for |11UTF-8|15: ");
    value[PROMPT_ASK_UTF8] = std::make_pair("Use UTF-8 Output Encoding",
                                            "|CR|CR|15Detected Default Encoding |03UTF-8|15 |CR[ENTER] to Continue, or [N] for |11CP437|15: ");

    // These are not displayed properly need to add adjustments
    // prior to matrix menu display, or DE delay action once it's displayed.
    value[PROMPT_CP437_SELECTED] = std::make_pair("Selected CP437 Output Encoding", "|09Selected: |03CP-437 Codepage.");
    value[PROMPT_UTF8_SELECTED] = std::make_pair("Selected UTF-8 Output Encoding", "|09Selected: |03UTF-8 Codepage.");

    m_text_prompts_dao.writeValue(value);
}

/**
 * @brief Sets an individual module index.
 * @param mod_function_index
 */
void ModPreLogon::changeModule(const int mod_function_index) {
    // Set, and Execute the Setup module.
    m_mod_function_index = mod_function_index;
    m_setup_functions[m_mod_function_index]();
}

/**
 * @brief Redisplay's the current module prompt.
 */
void ModPreLogon::redisplayModulePrompt() const {
    m_setup_functions[m_mod_function_index]();
}

/**
 * @brief Pull and Display Prompts
 * @param prompt
 */
void ModPreLogon::displayPrompt(const std::string &prompt) {
    baseDisplayPrompt(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and Display Prompts, Then Disconnect
 * @param prompt
 */
void ModPreLogon::displayPromptThenDisconnect(const std::string &prompt) {
    baseDisplayPrompt(prompt, m_text_prompts_dao, DISCONNECT_USER);
}

/**
 * @brief Pull and Display Prompts with following newline
 * @param prompt
 */
void ModPreLogon::displayPromptAndNewLine(const std::string &prompt) {
    baseDisplayPromptAndNewLine(prompt, m_text_prompts_dao);
}

/**
 * @brief Startup Human Shield, ESC Twice detection.
 * @return
 */
void ModPreLogon::setupHumanShield() {
    // Display Detecting Emulation, not using display prompt because we need to append.
    std::string result = "|07" + m_ctx.getCommonIO().centerPadding(
                             BUILD_INFO, m_ctx.getTelnet().getTermCols()) + "\r\n";

    result += m_ctx.getSessionIO().parseTextPrompt(
        m_text_prompts_dao.getPrompt(PROMPT_HUMAN_SHIELD)
    );

    // If response is echoed back, make it black on black.
    result.append("|00");
    std::string output = m_ctx.getSessionIO().pipe2ansi(result);

    baseProcessAndDeliver(output);

    startHumanShieldTimer();
}

/**
 * @brief Startup ANSI Emulation Detection.
 * @return
 */
void ModPreLogon::setupEmulationDetection() {
    // Deliver ANSI Location Sequence to Detect Emulation Response
    // Only detects if terminal handles ESC responses.
    // Windows Console Telnet will response it's at 259 y!
    // Also use Session Deliver, we don't need to use internal screen buffer on detection.
    const std::string detection = "\x1b[40;30m\x1b[255B\x1b[255C\x1b[6n";
    m_ctx.getBase().send(detection);

    std::string reset_position = "\x1b[1;1H\x1b[2J";
    baseProcessAndDeliver(reset_position);

    // Display Detecting Emulation, not using display prompt because we need to append.
    std::string result = m_ctx.getSessionIO().parseTextPrompt(
        m_text_prompts_dao.getPrompt(PROMPT_DETECT_EMULATION)
    );

    // If response is echoed back, make it black on black.
    result.append("|00");
    std::string output = m_ctx.getSessionIO().pipe2ansi(result);

    baseProcessAndDeliver(output);

    // Start Timeout for Detection.
    startDetectionTimer();
}

/**
 * @brief Ask is ANSI Color {Only ask for color if emulation detection fails!}
 * @return
 */
void ModPreLogon::setupAskANSIColor() {
    displayPrompt(PROMPT_DETECT_EMULATION);
}

/**
 * @brief Displays Terminal Detection after Emulation Detection.
 */
void ModPreLogon::displayTerminalDetection() {
    m_log.setNode(m_ctx.getBase().getNodeNumber());

    // Grab Detected Terminal, ANSI, XTERM, etc..
    displayPrompt(PROMPT_DETECT_TERMOPTS);

    // Grab Detected Terminal, ANSI, XTERM, etc.
    // Where grabbing both pairs first so we can parse the local MCI code
    // before we parse for colors and other stuff that would remove it!
    // NOTE, Term and Size can be made global mci codes later on. :)
    const M_StringPair prompt_term = m_text_prompts_dao.getPrompt(PROMPT_DETECTED_TERM);

    // Grab Detected Terminal Size 80x24, 80x50 etc.
    const M_StringPair prompt_size = m_text_prompts_dao.getPrompt(PROMPT_DETECTED_SIZE);

    // Send out the results of the prompts after parsing MCI and Color codes.
    // These prompts have special |OT placeholders for variables.
    const std::string mci_code = "|OT";

    // Handle Term, only display if prompt is not empty!
    if (!prompt_term.second.empty()) {
        std::string result = prompt_term.second;
        const std::string term = m_ctx.getTelnet().getTermType();

        m_log.log(Logging::LogLevel::Console, "Term Type=", term);

        m_ctx.getCommonIO().parseLocalMCI(result, mci_code, term);
        result = m_ctx.getSessionIO().pipe2ansi(result);
        baseProcessAndDeliver(result);
    }

    // Handle Screen Size only display if prompt is not empty!
    if (!prompt_size.second.empty()) {
        std::string result = prompt_size.second;
        std::string term_size;
        if (m_x_position == 0 || m_y_position == 0) {
            m_log.log(Logging::LogLevel::Console, "*** NAWS TermSize Detection!");
            // Make this Prompts for Customization!
            term_size = std::to_string(m_ctx.getTelnet().getTermCols());
            term_size.append("x");
            term_size.append(std::to_string(m_ctx.getTelnet().getTermRows()));
        } else {
            m_log.log(Logging::LogLevel::Console, "*** ESC TermSize Detection!");
            // Make this Prompts for Customization!
            term_size = std::to_string(m_x_position);
            term_size.append("x");
            term_size.append(std::to_string(m_y_position));

            m_ctx.getTelnet().setTermCols(m_x_position);
            m_ctx.getTelnet().setTermRows(m_y_position);
        }

        m_log.log(Logging::LogLevel::Console, "Term Size=", term_size);

        m_ctx.getCommonIO().parseLocalMCI(result, mci_code, term_size);
        result = m_ctx.getSessionIO().pipe2ansi(result);
        baseProcessAndDeliver(result);
    }

    // jump straight to asking code page after terminal detection
    changeModule(MOD_ASK_CODEPAGE);
}


/**
 * @brief Ask Setup CodePage CP437 / UTF-8
 * @return
 */
void ModPreLogon::setupAskCodePage() {
    // Fill the local term type to work with.
    m_term_type = m_ctx.getTelnet().getTermType();

    // If ANSI terminal detected, or 'undetected', then default ENTER to set for CP437
    // Otherwise default to UTF-8 for Xterm etc.. and all other terminals.
    if (m_term_type == "undetected" ||
        m_term_type.find("ansi", 0) != std::string::npos ||
        m_term_type.find("ANSI", 0) != std::string::npos) {
        displayPrompt(PROMPT_ASK_CP437);
    } else {
        displayPrompt(PROMPT_ASK_UTF8);
    }
}

/**
 * @brief Were Detecting ESC Twice Here.
 * @return
 */
bool ModPreLogon::humanShieldDetection(const std::string &input) {
    constexpr bool result = false;

    if (!input.empty()) {
        unsigned int ch = 0;
        ch = input[0];

        // Read in buffer once ESC sequence is hit to
        // Parse the ESC[6n Response
        if (ch == 27 && !m_is_esc_detected) {
            // First ESC Only, Mark True
            m_is_esc_detected = true;
        } else if ((ch == 27) && m_is_esc_detected && !m_is_human_shield) {
            // Second ESC Detected, Completed
            m_is_human_shield = true;
            m_is_esc_detected = false;

            displayPrompt(PROMPT_HUMAN_SHIELD_SUCCESS);
            return true;
        } else {
            // If you get other keys then ESC Excluding Null's, then reset Flags.
            if (ch != 0 && !m_is_human_shield) {
                m_is_esc_detected = false;
                m_is_human_shield = false;
            }
        }
    }

    return result;
}

/**
 * @brief Were Detecting Emulation here, we should get response from ESC[6n for Screen Position
 * @return
 */
bool ModPreLogon::emulationDetection(const std::string &input) {
    bool result = false;

    if (input.size() != 0) {
        unsigned int ch = 0;
        ch = input[0];

        // Read in buffer once ESC sequence is hit to
        // Parse the ESC[6n Response
        if (ch == 27) {
            m_is_esc_detected = true;
        }

        // Check for sequence terminator.
        if (m_is_esc_detected) {
            if (ch != 27 && ch != '\0' && ch != '[' && ch != 'R') {
                m_esc_sequence += static_cast<char>(ch);
            }

            if (toupper(ch) == 'R') {
                m_ctx.setAnsi(true);
                m_ctx.setAnsi(true);
                m_is_esc_detected = false;

                // Parse out x/y position coordinates for Screen Size returned.
                // Splunk String on : for X/Y Positions from Response
                const std::vector<std::string> positions = m_ctx.getCommonIO().splitString(m_esc_sequence, ';');
                if (positions.size() > 1) {
                    m_log.log(Logging::LogLevel::Debug, "X=", positions[1], "Y=", positions[0]);
                    m_x_position = m_ctx.getCommonIO().stringToInt(positions[1]);
                    m_y_position = m_ctx.getCommonIO().stringToInt(positions[0]);
                }
            } else {
                m_ctx.setAnsi(false);
            }
        }
    }

    return result;
}

/**
 * @brief ASK ANSI Color {Only ask for color if emulation detection fails!}
 * @return
 */
bool ModPreLogon::askANSIColor(const std::string &input) {
    std::string key;
    std::string result = m_ctx.getSessionIO().getInputField(input, key, Config::sSingle_key_length);

    // ESC was hit
    if (result == "aborted") {
        return false;
    } else if (result[0] == '\n') {
        // If ENTER Default to Yes, or Single Y is hit
        if (key.empty() || (toupper(key[0]) == 'Y' && key.size() == 1)) {
            // Key == 0 on [ENTER] pressed alone.
            if (key.empty()) {
                std::string yes_prompt = "Yes";
                baseProcessAndDeliverNewLine(yes_prompt);
            }

            m_log.log(Logging::LogLevel::Console, "Ansi Selected");

            m_ctx.setAnsi(true);
            displayPrompt(PROMPT_ANSI_SELECTED);
            displayTerminalDetection();
        }
        // Else check for single N for No to default to ASCII no colors.
        else if (toupper(key[0]) == 'N' && key.size() == 1) {
            m_log.log(Logging::LogLevel::Console, "Ascii Selected");

            baseProcessDeliverNewLine();
            displayPrompt(PROMPT_ASCII_SELECTED);
            m_ctx.setAnsi(false);
            displayTerminalDetection();
        } else {
            m_log.log(Logging::LogLevel::Console, "Invalid Color selection ANSI/ASCII");
            baseProcessDeliverNewLine();
            displayPrompt(PROMPT_USE_INVALID);
            redisplayModulePrompt();
        }
    } else {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if (result != "empty") {
            baseProcessDeliverInput(result);
        }
    }

    return true;
}

/**
 * @brief ASK CodePage CP437 / UTF-8
 * @return
 */
bool ModPreLogon::askCodePage(const std::string &input) {
    const std::string blackColor = "|00";
    std::string key;
    std::string result = m_ctx.getSessionIO().getInputField(input, key, Config::sSingle_key_length);

    // ESC was hit
    if (result == "aborted") {
        return false;
    }

    if (result[0] == '\n') {
        // If ENTER Default to Yes, or Single Y is hit
        if (key.size() == 0 || (toupper(key[0]) == 'Y' && key.size() == 1)) {
            // Key == 0 on [ENTER] pressed alone.
            if (key.size() == 0) {
                // If ENTER, then display Yes as key press.
                std::string yes_prompt = "Yes";
                baseProcessAndDeliverNewLine(yes_prompt);
            }

            baseProcessDeliverNewLine();

            std::string message = "";

            if (m_term_type == "undetected" ||
                m_term_type.find("ansi", 0) != std::string::npos ||
                m_term_type.find("ANSI", 0) != std::string::npos) {
                // Switch to ISO, then CP437 Character Set.
                message = "\x1b[0m" + m_ctx.getSessionIO().pipeColors(blackColor);
                message += "\x1b%@\x1b(U \r\n\x1b[A";
                m_ctx.getBase().send(message);

                message = m_ctx.getSessionIO().parseTextPrompt(
                    m_text_prompts_dao.getPrompt(PROMPT_CP437_SELECTED)
                );

                // Even though it's default, lets set it anyway
                m_log.log(Logging::LogLevel::Console, "Encoding set to CP437");
                m_ctx.m_encoding = Encoding::TextEncoding::CP437;
            } else {
                // Switch to Unicode Character Set.
                message = "\x1b[0m" + m_ctx.getSessionIO().pipeColors(blackColor);
                message += "\x1b%@\x1b%G \r\n\x1b[A";
                m_ctx.getBase().send(message);

                message = m_ctx.getSessionIO().parseTextPrompt(
                    m_text_prompts_dao.getPrompt(PROMPT_UTF8_SELECTED)
                );

                // Even though it's default, lets set it anyway
                m_log.log(Logging::LogLevel::Console, "Encoding set to UTF-8");
                m_ctx.m_encoding = Encoding::TextEncoding::UTF8;
            }

            baseProcessAndDeliverNewLine(message);
            setModuleInActive();
        }
        // Else check for single N for No to default to ASCII no colors.
        else if (toupper(key[0]) == 'N' && key.size() == 1) {
            baseProcessDeliverNewLine();

            std::string message;

            if (m_term_type == "undetected" ||
                m_term_type.find("ansi", 0) != std::string::npos ||
                m_term_type.find("ANSI", 0) != std::string::npos) {
                // Switch to Unicode Character Set.
                message = "\x1b[0m" + m_ctx.getSessionIO().pipeColors(blackColor);
                message += "\x1b%@\x1b%G \r\n\x1b[A";
                m_ctx.getBase().send(message);

                message = m_ctx.getSessionIO().parseTextPrompt(
                    m_text_prompts_dao.getPrompt(PROMPT_UTF8_SELECTED)
                );

                // Even though it's default, lets set it anyways/
                m_log.log(Logging::LogLevel::Console, "Encoding set to UTF-8");
                m_ctx.m_encoding = Encoding::TextEncoding::UTF8;
            } else {
                // Switch to ISO, then CP437 Character Set.
                message = "\x1b[0m" + m_ctx.getSessionIO().pipeColors(blackColor);
                message += "\x1b%@\x1b(U \r\n\x1b[A";
                m_ctx.getBase().send(message);

                message = m_ctx.getSessionIO().parseTextPrompt(
                    m_text_prompts_dao.getPrompt(PROMPT_CP437_SELECTED)
                );

                // Even though it's default, lets set it anyways
                m_log.log(Logging::LogLevel::Console, "Encoding set to CP437");
                m_ctx.m_encoding = Encoding::TextEncoding::CP437;
            }

            baseProcessAndDeliverNewLine(message);
            setModuleInActive();
        } else {
            baseProcessDeliverNewLine();
            displayPrompt(PROMPT_USE_INVALID);
            redisplayModulePrompt();
        }
    } else {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if (result != "empty") {
            baseProcessDeliverInput(result);
        }
    }

    return true;
}

/**
 * @brief Start ESC Twice Timer
 */
void ModPreLogon::startHumanShieldTimer() {
    // Add Deadline Timer for 1.5 seconds for complete Telopt Sequences responses
    //m_deadline_timer->setWaitInMilliseconds(8000);
    //m_deadline_timer->asyncWait(
    //    std::bind(&ModPreLogon::handleHumanShieldTimer, shared_from_this())
    //);
    auto callback_function = std::bind(&ModPreLogon::handleHumanShieldTimer, this);
    // m_ctx.getBase().m_async_io->asyncWait(4000, callback_function);
}

/**
 * @brief Start ANSI Detection timer
 */
void ModPreLogon::startDetectionTimer() {
    // Add Deadline Timer for 1.5 seconds for complete Telopt Sequences responses
    //m_deadline_timer->setWaitInMilliseconds(1500);
    //m_deadline_timer->asyncWait(
    //    std::bind(&ModPreLogon::handleDetectionTimer, shared_from_this())
    //);

    auto callback_function = std::bind(&ModPreLogon::handleDetectionTimer, this);
    //m_ctx.getBase().m_async_io->asyncWait(1500, callback_function);
}

/**
 * @brief Deadline Detection Timer ESC Twice
 * @param timer
 */
void ModPreLogon::handleHumanShieldTimer() {
    humanShieldCompleted();
}

/**
 * @brief ESC Twice Completed
 * @return
 */
void ModPreLogon::humanShieldCompleted() {
    m_log.setNode(m_ctx.getBase().getNodeNumber());
    if (m_is_human_shield) {
        // Move to Next Detection
        changeModule(MOD_DETECT_EMULATION);
    } else {
        // Logoff is too fast, Prompt is never displayed. hmm
        displayPromptThenDisconnect(PROMPT_HUMAN_SHIELD_FAIL);

        // Disconnect User
        setModuleInActive();
        m_log.log(Logging::LogLevel::Console, "Human Shield Failed, disconnecting!");
    }
}

/**
 * @brief Deadline Detection Timer for ANSI Detection
 */
void ModPreLogon::handleDetectionTimer() {
    // Jump to Emulation completed.
    emulationCompleted();
}

/**
 * @brief Detection Completed, display results.
 * @return
 */
void ModPreLogon::emulationCompleted() {
    if (m_ctx.isAnsi()) {
        displayPrompt(PROMPT_DETECTED_ANSI);
        displayTerminalDetection();
    } else {
        displayPrompt(PROMPT_DETECTED_NONE);
        changeModule(MOD_ASK_ANSI_COLOR);
    }
}


/*

if (sequence == "%G") {   // ESC % G
    m_encoding = TextEncoding::UTF8;
    return "";
}

if (sequence == "%@") {   // ESC % @
    m_encoding = TextEncoding::ISO_8859_1;
    return "";
}

if (sequence == "(0") {   // ESC ( 0
    m_encoding = TextEncoding::CP437;
    return "";
}

if (sequence == "(B") {   // ESC ( B
    m_encoding = TextEncoding::ASCII;
    return "";
}
*/
