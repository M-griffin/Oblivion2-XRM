#include "mod_prelogon.hpp"
#include "../model/config.hpp"

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <string>


/**
 * @brief Handles Updates or Data Input from Client
 * @return bool, not used anymore?!?
 */
bool ModPreLogon::update(const std::string &character_buffer, const bool &)
{
    // Make sure system is active, when system is done, success or failes
    // We change this is inactive to single the login process is completed.
    if(!m_is_active)
    {
        std::cout << "prelogon() !m_is_active" << std::endl;
        return false;
    }

    // Return True when were keeping module active / else false;
    if(character_buffer.size() == 0)
    {
        std::cout << "prelogon() !character_buffer size 0" << std::endl;
        return true;
    }

    // Process all incoming data stright to the input functions.
    m_mod_functions[m_mod_function_index](character_buffer);

    return true;
}

/**
 * @brief Startup class, setup and display initial screens / interface.
 * @return
 */
bool ModPreLogon::onEnter()
{
    std::cout << "OnEnter() ModPreLogon\n";
    m_is_active = true;

    // Grab ANSI Screen, display, if desired.. logon.ans maybe?

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
    std::cout << "OnExit() ModLogin\n";
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

    value[PROMPT_DETECT_EMULATION] = std::make_pair("Detecting Emulation", "|CR|CR|15 - |08Detecting Emulation |15-");
    value[PROMPT_DETECTED_ANSI]    = std::make_pair("ANSI Emulation Detected", "|CR|04E|12m|14ulation: An|12s|04i|07");
    value[PROMPT_DETECTED_NONE]    = std::make_pair("Emulation Detect:None", "|CREmulation Detect: None");

    value[PROMPT_USE_ANSI]         = std::make_pair("Use ANSI Colors (Y/n) ", "|CR|CRPress [Y or ENTER] to use ANSI Colors |CR|08[|15N|08] to Select ASCII No Colors |15: ");
    value[PROMPT_USE_INVALID]      = std::make_pair("Invalid Response to Y/N/ENTER", "|CR|12Invalid Response! Try again");
    value[PROMPT_ANSI_SELECTED]    = std::make_pair("ANSI Color Selected", "|CR|04A|12N|14SI Colors Select|12e|04d");
    value[PROMPT_ASCII_SELECTED]   = std::make_pair("ASCII No Colors Selected", "|CRASCII No Colors Selected");

    value[PROMPT_DETECT_TERMOPTS]  = std::make_pair("Detecting Terminal Options", "|CR|CR|08|15 - |08Detecting Terminal Options |15-|08");
    value[PROMPT_DETECTED_TERM]    = std::make_pair("Detecting Terminal: |OT ", "|CR|08Detected Terminal: |04|OT |07");
    value[PROMPT_DETECTED_SIZE]    = std::make_pair("Detecting Terminal Size: |OT ", "|CR|08Detected Screen Size: |04|OT |07");

    value[PROMPT_ASK_CP437]        = std::make_pair("Use CP437 Output Encoding", "|CR|CR|08[|15Y or ENTER|08] to Select MS-DOS CP-437 Codepage |CR|08[|15N|08] to Select UTF-8 Codepage : |15");
    value[PROMPT_ASK_UTF8]         = std::make_pair("Use UTF-8 Output Encoding", "|CR|CR|08[|15Y or ENTER|08] to Select UTF-8 Terminal Codepage|CR|08[|15N|08] to Select MS-DOS CP-437 Codepage : |15");

    value[PROMPT_CP437_SELECTED]   = std::make_pair("Selected CP437 Output Encoding", "|CR|04S|12e|14lected MS-DOS CP-437 Codepa|12g|04e|07");
    value[PROMPT_UTF8_SELECTED]    = std::make_pair("Selected UTF-8 Output Encoding", "|CR|04S|12e|14lected UTF-8 Terminal Codepa|12g|04e|07");

    m_text_prompts_dao->writeValue(value);
}

/**
 * @brief Sets an indivdual module index.
 * @param mod_function_index
 */
void ModPreLogon::changeModule(int mod_function_index)
{
    // Set, and Execute the Setup module.
    m_mod_function_index = mod_function_index;
    m_setup_functions[m_mod_function_index]();
}

/**
 * @brief Startup ANSI Emulation Detection.
 * @return
 */
void ModPreLogon::setupEmulationDetection()
{
    std::cout << "setupEmulationDetection()" << std::endl;

    // Deliver ANSI Location Sequence to Detect Emulation Response
    // Only detects if terminal handles ESC responses.
    // Windows Console Telnet will response it's at 259 y!
    baseProcessAndDeliver("\x1b[s\x1b[255B\x1b[6n");
    baseProcessAndDeliver("\x1b[u");

    // Display Detecting Emulation
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_DETECT_EMULATION)
                         );
    baseProcessAndDeliver(result);

    // Start Timeout for Detection.
    startDetectionTimer();
}

/**
 * @brief Ask to us ANSI Color {Only ask for color if emulation detection fails!}
 * @return
 */
void ModPreLogon::setupAskANSIColor()
{
    std::cout << "setupAskANSIColor()" << std::endl;

    // Ask to use colors - mode to next method to ask question and get reponse!
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_USE_ANSI)
                         );
    baseProcessAndDeliver(result);
}

/**
 * @brief Displays Terminal Detection after Emulation Detection.
 */
void ModPreLogon::displayTerminalDetection()
{
    // Grab Detected Terminal, ANSI, XTERM, etc..
    std::string result = m_session_io.parseTextPrompt(
                              m_text_prompts_dao->getPrompt(PROMPT_DETECT_TERMOPTS)
                          );

    baseProcessAndDeliver(result);

    // Grab Detected Terminal, ANSI, XTERM, etc..
    // Where grabbing both pairs first so we can parse the local MCI code
    // before we parse for colors and other stuff that would remove it!
    // NOTE, Term and Size can be made global mci codes later on. :)
    M_StringPair prompt_term = m_text_prompts_dao->getPrompt(PROMPT_DETECTED_TERM);


    // Grab Detected Terminal Size 80x24, 80x50 etc..
    M_StringPair prompt_size = m_text_prompts_dao->getPrompt(PROMPT_DETECTED_SIZE);


    // Send out the results of the prompts after parsing MCI and Color codes.
    // These prompts have spcial |OT place holders for variables.
    std::string mci_code = "|OT";

    // Handle Term, only display if prompt is not empty!
    if(prompt_term.second.size() > 0)
    {
        result = prompt_term.second;

        std::string term = m_session_data->m_telnet_state->getTermType();
        std::cout << "Term Type: " << term << std::endl;
        m_session_io.m_common_io.parseLocalMCI(result, mci_code, term);

        result = m_session_io.pipe2ansi(result);
        baseProcessAndDeliver(result);
    }

    // Handle Screen Size only display if prompt is not empty!
    if(prompt_size.second.size() > 0)
    {
        result = prompt_size.second;

        std::string term_size = std::to_string(m_session_data->m_telnet_state->getTermCols());
        term_size.append("x");
        term_size.append(std::to_string(m_session_data->m_telnet_state->getTermRows()));
        std::cout << "Term Size: " << term_size << std::endl;
        m_session_io.m_common_io.parseLocalMCI(result, mci_code, term_size);

        result = m_session_io.pipe2ansi(result);
        baseProcessAndDeliver(result);
    }

    // jump stright to asking code page after terminal detection
    changeModule(MOD_ASK_CODEPAGE);
}


/**
 * @brief Ask Setup CodePage CP437 / UTF-8
 * @return
 */
void ModPreLogon::setupAskCodePage()
{   
    std::cout << "setupAskCodePage()" << std::endl;

    // Display CodePage Selections, then prompt to ask for selection
    std::string result = "";

    // Fill the local term type to work with.
    m_term_type = m_session_data->m_telnet_state->getTermType();

    // If ANSI terminal detected, or 'undetected', then default ENTER to set for CP437
    // Otherwise default to UTF-8 for Xterm etc.. and all other terminals.
    if(boost::iequals(m_term_type, "undetected") ||
            boost::iequals(m_term_type, "ansi"))
    {
        result = m_session_io.parseTextPrompt(
                     m_text_prompts_dao->getPrompt(PROMPT_ASK_CP437)
                 );
    }
    else
    {
        result = m_session_io.parseTextPrompt(
                     m_text_prompts_dao->getPrompt(PROMPT_ASK_UTF8)
                 );
    }

    baseProcessAndDeliver(result);

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
        std::cout << "emulationDetection" << input << std::endl;

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
         * -- Modem or virtual modem will not have Telnet Options negoiation
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
    std::cout << "emulationCompleted: " << std::endl;
    // Reset to false so we can reuse for other methods.

    if(m_session_data->m_is_use_ansi)
    {
        // Emulation Detected ANSI
        std::string message = m_session_io.parseTextPrompt(
                                 m_text_prompts_dao->getPrompt(PROMPT_DETECTED_ANSI)
                             );
        baseProcessAndDeliver(message);

        // ANSI Detect, Move to Next Ask CodePage.
        displayTerminalDetection();

    }
    else
    {
        // Emulation Detected NONE
        std::string message = m_session_io.parseTextPrompt(
                                 m_text_prompts_dao->getPrompt(PROMPT_DETECTED_NONE)
                             );
        baseProcessAndDeliver(message);

        // Emulation not detected, ask to use ANSI Color, then move to CodePage.
        changeModule(MOD_ASK_ANSI_COLOR);
    }
}

/**
 * @brief ASK ANSI Color {Only ask for color if emulation detection fails!}
 * @return
 */
bool ModPreLogon::askANSIColor(const std::string &input)
{
    std::cout << "askANSIColor: " << input << std::endl;

    // handle input for using ansi color, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);
    if(result == "aborted") // ESC was hit, make this just clear the input text, or start over!
    {
        std::cout << "aborted!" << std::endl;
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
                baseProcessAndDeliver("Yes");
            }

            // Set ANSI Color Emulation to True
            m_session_data->m_is_use_ansi = true;

            // ANSI Selected Text Prompt
            std::string message = m_session_io.parseTextPrompt(
                                     m_text_prompts_dao->getPrompt(PROMPT_ANSI_SELECTED)
                                 );

            baseProcessAndDeliver(message);

            // Change to next Module
            displayTerminalDetection();
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            std::string message = m_session_io.parseTextPrompt(
                                     m_text_prompts_dao->getPrompt(PROMPT_ASCII_SELECTED)
                                 );

            baseProcessAndDeliver(message);

            // Set ANSI Color Emulation to False
            m_session_data->m_is_use_ansi = false;

            // Change to next module
            displayTerminalDetection();
        }
        else
        {
            std::string message = m_session_io.parseTextPrompt(
                                     m_text_prompts_dao->getPrompt(PROMPT_USE_INVALID)
                                 );

            baseProcessAndDeliver(message);

            // Invalid, Ask again
            setupAskANSIColor();
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if (result != "empty") {
            baseProcessAndDeliver(result);
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
    std::cout << "askCodePage: " << input << std::endl;

    // handle input for using ansi color, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);
    if(result == "aborted") // ESC was hit, make this just clear the input text, or start over!
    {
        std::cout << "aborted!" << std::endl;
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
                baseProcessAndDeliver("Yes");
            }

            std::string message = "";
            if(boost::iequals(m_term_type, "undetected") ||
                    boost::iequals(m_term_type, "ansi"))
            {
                // Switch to ISO, then CP437 Character Set.
                message = "\x1b%@\x1b(U";
                message += m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_CP437_SELECTED)
                         );

                // Even though it's default, lets set it anyways/
                m_session_data->m_output_encoding = "cp437";
            }
            else
            {
                // Switch to Unicode Character Set.
                message = "\x1b%@\x1b%G";
                message += m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_UTF8_SELECTED)
                         );

                // Even though it's default, lets set it anyways/
                m_session_data->m_output_encoding = "utf-8";
            }

            // Deliver result.
            baseProcessAndDeliver(message);

            // Change to next Module ( or Exit Prelogin and move to Login module!!
            m_is_active = false;
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            std::string message = "";
            if(boost::iequals(m_term_type, "undetected") ||
                    boost::iequals(m_term_type, "ansi"))
            {
                // Switch to Unicode Character Set.
                message = "\x1b%@\x1b%G";
                message += m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_UTF8_SELECTED)
                         );

                // Even though it's default, lets set it anyways/
                m_session_data->m_output_encoding = "utf-8";
            }
            else
            {
                // Switch to ISO, then CP437 Character Set.
                message = "\x1b%@\x1b(U";
                message += m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_CP437_SELECTED)
                         );

                // Even though it's default, lets set it anyways/
                m_session_data->m_output_encoding = "cp437";

            }

            // Deliver result.
            baseProcessAndDeliver(message);

            // Change to next Module ( or Exit Prelogin and move to Login module!!
            m_is_active = false;
        }
        else
        {
            // Invalid Entry, try again!
            std::string message = m_session_io.parseTextPrompt(
                                     m_text_prompts_dao->getPrompt(PROMPT_USE_INVALID)
                                 );

            baseProcessAndDeliver(message);

            // Invalid, Ask again
            setupAskCodePage();
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if (result != "empty") {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}
