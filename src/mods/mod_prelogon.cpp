#include "mod_prelogon.hpp"
#include "../model-sys/config.hpp"
#include "../encoding.hpp"
#include "../logging.hpp"

#include <algorithm>
#include <iostream>
#include <string>


/**
 * @brief Handles Updates or Data Input from Client
 * @return bool, not used anymore?!?
 */
bool ModPreLogon::update(const std::string &character_buffer, const bool &)
{
    // Make sure system is active, when system is done, success or fails
    // We change this is inactive to single the login process is completed.
    if(!m_is_active)
    {
        return false;
    }

    // Return True when were keeping module active / else false;
    if(character_buffer.size() == 0)
    {
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
bool ModPreLogon::onEnter()
{
    m_is_active = true;

    // Grab ANSI Screen, display, if desired.. logon.ans maybe?
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
bool ModPreLogon::onExit()
{
    m_is_active = false;
    return true;
}

/**
 * @brief Create Default Text Prompts for module
 */
void ModPreLogon::createTextPrompts()
{
    // Create Mapping to pass for file creation (default values)
    M_TextPrompt value;

    value[PROMPT_DETECT_EMULATION] = std::make_pair("Detecting Emulation", "Detecting Emulation");
    value[PROMPT_DETECTED_ANSI]    = std::make_pair("Emulation Detected: Ansi ESC Supported", "|CREmulation Detected: |03ANSI ESC Supported.");
    value[PROMPT_DETECTED_NONE]    = std::make_pair("Emulation Detected: None", "|CREmulation Detect: |03none");

    value[PROMPT_USE_ANSI]         = std::make_pair("Use ANSI Colors (Y/n) ", "|CRPress [y/ENTER or n] to use ANSI Colors or to Select ASCII No Colors: ");
    value[PROMPT_USE_INVALID]      = std::make_pair("Invalid Response to Y/N/ENTER", "|04Invalid Response! Try again.");
    value[PROMPT_ANSI_SELECTED]    = std::make_pair("ANSI Color Selected", "Selected: |03Ansi.");
    value[PROMPT_ASCII_SELECTED]   = std::make_pair("ASCII No Colors Selected", "Selected: None.");

    value[PROMPT_DETECT_TERMOPTS]  = std::make_pair("Detecting Terminal Options", "|CR|CRDetecting Terminal Options");
    value[PROMPT_DETECTED_TERM]    = std::make_pair("Detecting Terminal: |OT ", "|CRDetected Terminal Type: |03|OT");
    value[PROMPT_DETECTED_SIZE]    = std::make_pair("Detecting Terminal Size: |OT ", "|CRDetected Screen Size: |03|OT");

    value[PROMPT_ASK_CP437]        = std::make_pair("Use CP437 Output Encoding", "|CR|CR[y/ENTER or n] Select Output Encoding CP-437: ");
    value[PROMPT_ASK_UTF8]         = std::make_pair("Use UTF-8 Output Encoding", "|CR|CR[y/ENTER or n] Select Output Encoding UTF-8: ");

    value[PROMPT_CP437_SELECTED]   = std::make_pair("Selected CP437 Output Encoding", "Selected: |03CP-437 Codepage.");
    value[PROMPT_UTF8_SELECTED]    = std::make_pair("Selected UTF-8 Output Encoding", "Selected: |03UTF-8 Codepage.");

    m_text_prompts_dao->writeValue(value);
}

/**
 * @brief Sets an individual module index.
 * @param mod_function_index
 */
void ModPreLogon::changeModule(int mod_function_index)
{
    // Set, and Execute the Setup module.
    m_mod_function_index = mod_function_index;
    m_setup_functions[m_mod_function_index]();
}

/**
 * @brief Redisplay's the current module prompt.
 * @param mod_function_index
 */
void ModPreLogon::redisplayModulePrompt()
{
    m_setup_functions[m_mod_function_index]();
}

/**
 * @brief Pull and Display Prompts
 * @param prompt
 */
void ModPreLogon::displayPrompt(const std::string &prompt)
{
    baseDisplayPrompt(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and Display Prompts with following newline
 * @param prompt
 */
void ModPreLogon::displayPromptAndNewLine(const std::string &prompt)
{
    baseDisplayPromptAndNewLine(prompt, m_text_prompts_dao);
}

/**
 * @brief Startup ANSI Emulation Detection.
 * @return
 */
void ModPreLogon::setupEmulationDetection()
{
    // Deliver ANSI Location Sequence to Detect Emulation Response
    // Only detects if terminal handles ESC responses.
    // Windows Console Telnet will response it's at 259 y!
    std::string detection = m_session_io.pipe2ansi("|00\x1b[255B\x1b[6n");
    std::string restore_position = "\x1b[1;1H\x1b[2J";

    baseProcessAndDeliver(detection);
    baseProcessAndDeliver(restore_position);

    // Display Detecting Emulation, not using display prompt cause we need to append.
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_DETECT_EMULATION)
                         );

    // If response is echoed back, make it black on black.
    result.append("|00");
    std::string output = m_session_io.pipe2ansi(result);

    baseProcessAndDeliver(output);

    // Start Timeout for Detection.
    startDetectionTimer();
}

/**
 * @brief Ask to us ANSI Color {Only ask for color if emulation detection fails!}
 * @return
 */
void ModPreLogon::setupAskANSIColor()
{
    displayPrompt(PROMPT_DETECT_EMULATION);
}

/**
 * @brief Displays Terminal Detection after Emulation Detection.
 */
void ModPreLogon::displayTerminalDetection()
{
    Logging *log = Logging::instance();

    // Grab Detected Terminal, ANSI, XTERM, etc..
    displayPrompt(PROMPT_DETECT_TERMOPTS);

    // Grab Detected Terminal, ANSI, XTERM, etc..
    // Where grabbing both pairs first so we can parse the local MCI code
    // before we parse for colors and other stuff that would remove it!
    // NOTE, Term and Size can be made global mci codes later on. :)
    M_StringPair prompt_term = m_text_prompts_dao->getPrompt(PROMPT_DETECTED_TERM);

    // Grab Detected Terminal Size 80x24, 80x50 etc..
    M_StringPair prompt_size = m_text_prompts_dao->getPrompt(PROMPT_DETECTED_SIZE);

    // Send out the results of the prompts after parsing MCI and Color codes.
    // These prompts have special |OT place holders for variables.
    std::string mci_code = "|OT";

    // Handle Term, only display if prompt is not empty!
    if(prompt_term.second.size() > 0)
    {
        std::string result = prompt_term.second;
        std::string term = m_session_data->m_telnet_state->getTermType();


        log->xrmLog<Logging::CONSOLE_LOG>("Term Type=", term);

        m_session_io.m_common_io.parseLocalMCI(result, mci_code, term);
        result = m_session_io.pipe2ansi(result);
        baseProcessAndDeliver(result);
    }

    // Handle Screen Size only display if prompt is not empty!
    if(prompt_size.second.size() > 0)
    {
        std::string result = prompt_size.second;
        std::string term_size = std::to_string(m_session_data->m_telnet_state->getTermCols());
        term_size.append("x");
        term_size.append(std::to_string(m_session_data->m_telnet_state->getTermRows()));

        log->xrmLog<Logging::CONSOLE_LOG>("Term Size=", term_size);

        m_session_io.m_common_io.parseLocalMCI(result, mci_code, term_size);
        result = m_session_io.pipe2ansi(result);
        baseProcessAndDeliver(result);
    }

    // jump straight to asking code page after terminal detection
    changeModule(MOD_ASK_CODEPAGE);
}


/**
 * @brief Ask Setup CodePage CP437 / UTF-8
 * @return
 */
void ModPreLogon::setupAskCodePage()
{
    // Fill the local term type to work with.
    m_term_type = m_session_data->m_telnet_state->getTermType();

    // If ANSI terminal detected, or 'undetected', then default ENTER to set for CP437
    // Otherwise default to UTF-8 for Xterm etc.. and all other terminals.
    if(m_term_type == "undetected" || m_term_type == "ansi")
    {
        displayPrompt(PROMPT_ASK_CP437);
    }
    else
    {
        displayPrompt(PROMPT_ASK_UTF8);
    }
}

/**
 * @brief Were Detecting Emulation here, we should get response.
 * @return
 */
bool ModPreLogon::emulationDetection(const std::string &input)
{
    bool result = false;

    if(input.size() != 0)
    {
        unsigned int ch = 0;
        ch = input[0];

        // Read in buffer once ESC sequence is hit to
        // Parse the ESC[6n Response
        if(ch == 27)
        {
            m_is_esc_detected = true;
        }

        // Check for sequence terminator.
        if(m_is_esc_detected)
        {
            if(toupper(ch) == 'R')
            {
                m_session_data->m_is_use_ansi = true;
                // Make sure anything piggy backing doesn't reset
                // Once were detected.
                m_is_esc_detected = false;
            }
            else
            {
                m_session_data->m_is_use_ansi = false;
            }
        }

        /* -- Get Secondary screen size detection for emulation response.
         * -- Modem or virtual modem will not have Telnet Options negotiation
         * -- So then we have to detect old fashion way with ESC response!
        // Were inside sequence.
        if (m_esc_detected)
        {
            // Get x[##;xx
            if(isdigit(ch))
            {
                xy[i] = ch;
                ++i;
            }

            //now get x[xx;##
            if(ch==';')
            {
                i = 0;
                m_x_position = atoi(xy);
            }

            //now get end of sequence.
            if (toupper(ch) == 'R')
            {
                m_y_position = atoi(xy);

                // Register we received a completed sequence.
                m_is_emulation_detected = true;
                break;
            }
        }*/
    }

    return result;
}

/**
 * @brief Detection Completed, display results.
 * @return
 */
void ModPreLogon::emulationCompleted()
{
    if(m_session_data->m_is_use_ansi)
    {
        displayPrompt(PROMPT_DETECTED_ANSI);
        displayTerminalDetection();
    }
    else
    {
        displayPrompt(PROMPT_DETECTED_NONE);
        changeModule(MOD_ASK_ANSI_COLOR);
    }
}

/**
 * @brief ASK ANSI Color {Only ask for color if emulation detection fails!}
 * @return
 */
bool ModPreLogon::askANSIColor(const std::string &input)
{
    Logging *log = Logging::instance();
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);

    // ESC was hit
    if(result == "aborted")
    {
        return false;
    }
    else if(result[0] == '\n')
    {
        // If ENTER Default to Yes, or Single Y is hit
        if(key.size() == 0 || (toupper(key[0]) == 'Y' && key.size() == 1))
        {
            // Key == 0 on [ENTER] pressed alone.
            if(key.size() == 0)
            {
                std::string yes_prompt = "Yes";
                baseProcessAndDeliverNewLine(yes_prompt);
            }

            log->xrmLog<Logging::CONSOLE_LOG>("Ansi Selected");

            m_session_data->m_is_use_ansi = true;
            displayPrompt(PROMPT_ANSI_SELECTED);
            displayTerminalDetection();
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            log->xrmLog<Logging::CONSOLE_LOG>("Ascii Selected");

            baseProcessDeliverNewLine();
            displayPrompt(PROMPT_ASCII_SELECTED);
            m_session_data->m_is_use_ansi = false;
            displayTerminalDetection();
        }
        else
        {
            log->xrmLog<Logging::CONSOLE_LOG>("Invalid Color selection ANSI/ASCII");
            baseProcessDeliverNewLine();
            displayPrompt(PROMPT_USE_INVALID);
            redisplayModulePrompt();
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
        }
    }

    return true;
}

/**
 * @brief ASK CodePage CP437 / UTF-8
 * @return
 */
bool ModPreLogon::askCodePage(const std::string &input)
{
    Logging *log = Logging::instance();
    std::string blackColor = "|00";
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);

    // ESC was hit
    if(result == "aborted")
    {
        return false;
    }
    else if(result[0] == '\n')
    {
        // If ENTER Default to Yes, or Single Y is hit
        if(key.size() == 0 || (toupper(key[0]) == 'Y' && key.size() == 1))
        {
            // Key == 0 on [ENTER] pressed alone.
            if(key.size() == 0)
            {
                // If ENTER, then display Yes as key press.
                std::string yes_prompt = "Yes";
                baseProcessAndDeliverNewLine(yes_prompt);
            }

            baseProcessDeliverNewLine();

            std::string message = "";

            if(m_term_type == "undetected" || m_term_type == "ansi")
            {
                // Switch to ISO, then CP437 Character Set.
                message = "\x1b[0m" + m_session_io.pipeColors(blackColor);
                message += "\x1b%@\x1b(U \r\n\x1b[A";
                m_session_data->deliver(message);

                message = m_session_io.parseTextPrompt(
                              m_text_prompts_dao->getPrompt(PROMPT_CP437_SELECTED)
                          );

                // Even though it's default, lets set it anyways/
                log->xrmLog<Logging::CONSOLE_LOG>("Encoding set to CP437");
                m_session_data->m_encoding_text = Encoding::ENCODING_TEXT_CP437;
                m_session_data->m_encoding = Encoding::ENCODE_CP437;
            }
            else
            {
                // Switch to Unicode Character Set.
                message = "\x1b[0m" + m_session_io.pipeColors(blackColor);
                message += "\x1b%@\x1b%G \r\n\x1b[A";
                m_session_data->deliver(message);

                message = m_session_io.parseTextPrompt(
                              m_text_prompts_dao->getPrompt(PROMPT_UTF8_SELECTED)
                          );

                // Even though it's default, lets set it anyways/
                log->xrmLog<Logging::CONSOLE_LOG>("Encoding set to UTF-8");
                m_session_data->m_encoding_text = Encoding::ENCODING_TEXT_UTF8;
                m_session_data->m_encoding = Encoding::ENCODE_UTF8;
            }

            baseProcessAndDeliverNewLine(message);
            m_is_active = false;
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            baseProcessDeliverNewLine();

            std::string message = "";

            if(m_term_type == "undetected" || m_term_type == "ansi")
            {
                // Switch to Unicode Character Set.
                message = "\x1b[0m" + m_session_io.pipeColors(blackColor);
                message += "\x1b%@\x1b%G \r\n\x1b[A";
                m_session_data->deliver(message);

                message = m_session_io.parseTextPrompt(
                              m_text_prompts_dao->getPrompt(PROMPT_UTF8_SELECTED)
                          );

                // Even though it's default, lets set it anyways/
                log->xrmLog<Logging::CONSOLE_LOG>("Encoding set to UTF-8");
                m_session_data->m_encoding_text = Encoding::ENCODING_TEXT_UTF8;
                m_session_data->m_encoding = Encoding::ENCODE_UTF8;
            }
            else
            {
                // Switch to ISO, then CP437 Character Set.
                message = "\x1b[0m" + m_session_io.pipeColors(blackColor);
                message += "\x1b%@\x1b(U \r\n\x1b[A";
                m_session_data->deliver(message);

                message = m_session_io.parseTextPrompt(
                              m_text_prompts_dao->getPrompt(PROMPT_CP437_SELECTED)
                          );

                // Even though it's default, lets set it anyways/
                log->xrmLog<Logging::CONSOLE_LOG>("Encoding set to CP437");
                m_session_data->m_encoding_text = Encoding::ENCODING_TEXT_CP437;
                m_session_data->m_encoding = Encoding::ENCODE_CP437;
            }

            baseProcessAndDeliverNewLine(message);
            m_is_active = false;
        }
        else
        {
            baseProcessDeliverNewLine();
            displayPrompt(PROMPT_USE_INVALID);
            redisplayModulePrompt();
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
        }
    }

    return true;
}
