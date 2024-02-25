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

#include "../telnet_decoder.hpp"
#include "../encoding.hpp"
#include "../logging.hpp"
#include "../session_io.hpp"
#include "../session.hpp"
#include "../async_io.hpp"
#include "../common_io.hpp"

ModPreLogon::ModPreLogon(session_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process,
        common_io_ptr common_io, session_io_ptr session_io)
    : ModBase(session_data, config, ansi_process, "mod_prelogon.yaml", common_io, session_io)
    , m_text_prompts_dao(nullptr)
    , m_mod_function_index(MOD_HUMAN_SHIELD)
    , m_is_text_prompt_exist(false)
    , m_is_esc_detected(false)
    , m_is_human_shield(false)
    , m_input_buffer("")
    , m_x_position(0)
    , m_y_position(0)
    , m_term_type("undetected")
    , m_esc_sequence("")
{
    // Setup Smart Pointers
    m_text_prompts_dao = std::make_shared<TextPromptsDao>(GLOBAL_DATA_PATH, m_filename);
    
    // Push function pointers to the stack.
    m_setup_functions.push_back(std::bind(&ModPreLogon::setupHumanShield, this));
    m_setup_functions.push_back(std::bind(&ModPreLogon::setupEmulationDetection, this));
    m_setup_functions.push_back(std::bind(&ModPreLogon::setupAskANSIColor, this));
    m_setup_functions.push_back(std::bind(&ModPreLogon::setupAskCodePage, this));

    m_mod_functions.push_back(std::bind(&ModPreLogon::humanShieldDetection, this, std::placeholders::_1));
    m_mod_functions.push_back(std::bind(&ModPreLogon::emulationDetection, this, std::placeholders::_1));
    m_mod_functions.push_back(std::bind(&ModPreLogon::askANSIColor, this, std::placeholders::_1));
    m_mod_functions.push_back(std::bind(&ModPreLogon::askCodePage, this, std::placeholders::_1));

    // Check of the Text Prompts exist.
    m_is_text_prompt_exist = m_text_prompts_dao->fileExists();

    if(!m_is_text_prompt_exist)
    {
        createTextPrompts();
    }

    // Loads all Text Prompts for current module
    m_text_prompts_dao->readPrompts();
}

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
    // On Initial Startup, setup user record with system colors for menu system
    // this is overwritten once the user logs in, otherwise the menu system
    // will use these defaults for theming.   
    m_session_data->m_user_record->sRegColor = m_config->default_color_regular;
    m_session_data->m_user_record->sPromptColor = m_config->default_color_prompt;
    m_session_data->m_user_record->sInputColor = m_config->default_color_input;
    m_session_data->m_user_record->sInverseColor = m_config->default_color_inverse;
    m_session_data->m_user_record->sStatColor = m_config->default_color_stat;
    m_session_data->m_user_record->sBoxColor = m_config->default_color_box;    
    
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
    
    value[PROMPT_HUMAN_SHIELD]         = std::make_pair("Hit [ESC] twice to continue", "|CR|12<|04Human Shield|12>|08: |15Hit |08[|12ESC|08] |15twice within |124 |15seconds to continue!");
    value[PROMPT_HUMAN_SHIELD_SUCCESS] = std::make_pair("ESC Detection Successful", "|CR |08- |15Input Detected|08, |15Loading |08- |151 Moment \x1b[0m");
    value[PROMPT_HUMAN_SHIELD_FAIL]    = std::make_pair("No Human Detected", "|CR |08- |15No Input Detected|08, |15Disconnecting|08...  \x1b[0m");

    value[PROMPT_DETECT_EMULATION]     = std::make_pair("Detecting Emulation", "|09Detecting Emulation");
    value[PROMPT_DETECTED_ANSI]        = std::make_pair("Emulation Detected: Ansi ESC Supported", "|CR|15Emulation Detected: |03ANSI ESC Supported.");
    value[PROMPT_DETECTED_NONE]        = std::make_pair("Emulation Detected: None", "|CR|15Emulation Detect: |03none");

    value[PROMPT_USE_ANSI]             = std::make_pair("Use ANSI Colors (Y/n) ", "|CRPress [y/ENTER or n] to use ANSI Colors or to Select ASCII No Colors: ");
    value[PROMPT_USE_INVALID]          = std::make_pair("Invalid Response to Y/N/ENTER", "|04Invalid Response! Try again.");
    value[PROMPT_ANSI_SELECTED]        = std::make_pair("ANSI Color Selected", "Selected: |03Ansi.");
    value[PROMPT_ASCII_SELECTED]       = std::make_pair("ASCII No Colors Selected", "Selected: None.");

    value[PROMPT_DETECT_TERMOPTS]      = std::make_pair("Detecting Terminal Options", "|CR|CR|09Detecting Terminal Options");
    value[PROMPT_DETECTED_TERM]        = std::make_pair("Detecting Terminal: |OT ", "|CR|15Detected Terminal Type: |03|OT");
    value[PROMPT_DETECTED_SIZE]        = std::make_pair("Detecting Terminal Size: |OT ", "|CR|15Detected Screen Size: |03|OT");

    value[PROMPT_ASK_CP437]            = std::make_pair("Use CP437 Output Encoding", "|CR|CR|15Detected Default Encoding |03CP-437|15 |CR[ENTER] to Continue, or [N] for |11UTF-8|15: ");
    value[PROMPT_ASK_UTF8]             = std::make_pair("Use UTF-8 Output Encoding", "|CR|CR|15Detected Default Encoding |03UTF-8|15 |CR[ENTER] to Continue, or [N] for |11CP437|15: ");

    // These are not displayed properly need to add adjustments 
    // prior to matrix menu display, or DE delay action once it's displayed.
    value[PROMPT_CP437_SELECTED]       = std::make_pair("Selected CP437 Output Encoding", "|09Selected: |03CP-437 Codepage.");
    value[PROMPT_UTF8_SELECTED]        = std::make_pair("Selected UTF-8 Output Encoding", "|09Selected: |03UTF-8 Codepage.");

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
 * @brief Pull and Display Prompts, Then Disconnect
 * @param prompt
 */
void ModPreLogon::displayPromptThenDisconnect(const std::string &prompt)
{
    baseDisplayPrompt(prompt, m_text_prompts_dao, DISCONNECT_USER);
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
 * @brief Startup Human Shield, ESC Twice detection.
 * @return
 */
void ModPreLogon::setupHumanShield()
{
    // Display Detecting Emulation, not using display prompt cause we need to append.
    std::string result = "|07" + m_common_io->centerPadding(BUILD_INFO, m_session_data->m_telnet_decoder->getTermCols()) + "\r\n";
    
    result += m_session_io->parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_HUMAN_SHIELD)
                         );

    // If response is echoed back, make it black on black.
    result.append("|00");
    std::string output = m_session_io->pipe2ansi(result);

    baseProcessAndDeliver(output);
    
    startHumanShieldTimer();
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
    // Also use Session Deliver, we don't need to use internal screen buffer on detection.
    std::string detection = "\x1b[40;30m\x1b[255B\x1b[255C\x1b[6n";
    m_session_data->deliver(detection);
            
    std::string reset_position = "\x1b[1;1H\x1b[2J";
    
    baseProcessAndDeliver(reset_position);

    // Display Detecting Emulation, not using display prompt cause we need to append.
    std::string result = m_session_io->parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_DETECT_EMULATION)
                         );

    // If response is echoed back, make it black on black.
    result.append("|00");
    std::string output = m_session_io->pipe2ansi(result);

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
    
    m_log.setUserInfo(m_session_data->m_node_number);
    
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
        std::string term = m_session_data->m_telnet_decoder->getTermType();

        m_log.write<Logging::CONSOLE_LOG>("Term Type=", term);

        m_common_io->parseLocalMCI(result, mci_code, term);
        result = m_session_io->pipe2ansi(result);
        baseProcessAndDeliver(result);
    }

    // Handle Screen Size only display if prompt is not empty!
    if(prompt_size.second.size() > 0)
    {
        std::string result = prompt_size.second;
        std::string term_size = "";
        if (m_x_position == 0 || m_y_position == 0)
        {
            m_log.write<Logging::CONSOLE_LOG>("*** NAWS TermSize Detection!");
            // Make this Propmpts for Customization!            
            term_size = std::to_string(m_session_data->m_telnet_decoder->getTermCols());
            term_size.append("x");
            term_size.append(std::to_string(m_session_data->m_telnet_decoder->getTermRows()));            
        }
        else
        {
            m_log.write<Logging::CONSOLE_LOG>("*** ESC TermSize Detection!");
            // Make this Propmpts for Customization!            
            term_size = std::to_string(m_x_position);
            term_size.append("x");
            term_size.append(std::to_string(m_y_position));
            
            m_session_data->m_telnet_decoder->setTermCols(m_x_position);
            m_session_data->m_telnet_decoder->setTermRows(m_y_position);
        }

        m_log.write<Logging::CONSOLE_LOG>("Term Size=", term_size);

        m_common_io->parseLocalMCI(result, mci_code, term_size);
        result = m_session_io->pipe2ansi(result);
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
    m_term_type = m_session_data->m_telnet_decoder->getTermType();

    // If ANSI terminal detected, or 'undetected', then default ENTER to set for CP437
    // Otherwise default to UTF-8 for Xterm etc.. and all other terminals.
    if(m_term_type == "undetected" || 
       m_term_type.find("ansi",0) != std::string::npos || 
       m_term_type.find("ANSI",0) != std::string::npos)
    {
        displayPrompt(PROMPT_ASK_CP437);
    }
    else
    {
        displayPrompt(PROMPT_ASK_UTF8);
    }
}

/**
 * @brief Were Detecting ESC Twice Here.
 * @return
 */
bool ModPreLogon::humanShieldDetection(const std::string &input)
{   
    bool result = false;

    if(input.size() != 0)
    {
        unsigned int ch = 0;
        ch = input[0];

        // Read in buffer once ESC sequence is hit to
        // Parse the ESC[6n Response
        if(ch == 27 && !m_is_esc_detected)
        {
            // First ESC Only, Mark True
            m_is_esc_detected = true;
        }
        else if ((ch == 27) && m_is_esc_detected && !m_is_human_shield)
        {
            // Second ESC Detected, Completed
            m_is_human_shield = true;
            m_is_esc_detected = false;
            
            displayPrompt(PROMPT_HUMAN_SHIELD_SUCCESS);
            return true;
        }
        else 
        {
            // If get other keys then ESC Excluding Null's, then reset Flags.
            if (ch != 0 && !m_is_human_shield)
            {
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
            if (ch != 27 && ch!= '\0' && ch != '[' && ch != 'R')
            {
                m_esc_sequence += (char)ch;
            }            
            
            if(toupper(ch) == 'R')
            {
                m_session_data->m_is_use_ansi = true;
                m_is_esc_detected = false;
                
                // Parse out x/y position coordinates for Screen Size returned.
                // Splunk String on : for X/Y Positions from Response
                std::vector<std::string> positions = m_common_io->splitString(m_esc_sequence, ';');
                if (positions.size() > 1)
                {
                    m_log.write<Logging::DEBUG_LOG>("X=", positions[1], "Y=", positions[0]);
                    m_x_position = m_common_io->stringToInt(positions[1]);
                    m_y_position = m_common_io->stringToInt(positions[0]);
                }
            }
            else
            {
                m_session_data->m_is_use_ansi = false;
            }
        }
    }

    return result;
}

/**
 * @brief ASK ANSI Color {Only ask for color if emulation detection fails!}
 * @return
 */
bool ModPreLogon::askANSIColor(const std::string &input)
{
    std::string key = "";
    std::string result = m_session_io->getInputField(input, key, Config::sSingle_key_length);

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

            m_log.write<Logging::CONSOLE_LOG>("Ansi Selected");

            m_session_data->m_is_use_ansi = true;
            displayPrompt(PROMPT_ANSI_SELECTED);
            displayTerminalDetection();
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            m_log.write<Logging::CONSOLE_LOG>("Ascii Selected");

            baseProcessDeliverNewLine();
            displayPrompt(PROMPT_ASCII_SELECTED);
            m_session_data->m_is_use_ansi = false;
            displayTerminalDetection();
        }
        else
        {
            m_log.write<Logging::CONSOLE_LOG>("Invalid Color selection ANSI/ASCII");
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
    std::string blackColor = "|00";
    std::string key = "";
    std::string result = m_session_io->getInputField(input, key, Config::sSingle_key_length);

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

            if(m_term_type == "undetected" || 
               m_term_type.find("ansi",0) != std::string::npos || 
               m_term_type.find("ANSI",0) != std::string::npos)
            {
                // Switch to ISO, then CP437 Character Set.
                message = "\x1b[0m" + m_session_io->pipeColors(blackColor);
                message += "\x1b%@\x1b(U \r\n\x1b[A";
                m_session_data->deliver(message);

                message = m_session_io->parseTextPrompt(
                              m_text_prompts_dao->getPrompt(PROMPT_CP437_SELECTED)
                          );

                // Even though it's default, lets set it anyways/
                m_log.write<Logging::CONSOLE_LOG>("Encoding set to CP437");
                m_session_data->m_encoding_text = Encoding::ENCODING_TEXT_CP437;
                m_session_data->m_encoding = Encoding::ENCODE_CP437;
            }
            else
            {
                // Switch to Unicode Character Set.
                message = "\x1b[0m" + m_session_io->pipeColors(blackColor);
                message += "\x1b%@\x1b%G \r\n\x1b[A";
                m_session_data->deliver(message);

                message = m_session_io->parseTextPrompt(
                              m_text_prompts_dao->getPrompt(PROMPT_UTF8_SELECTED)
                          );

                // Even though it's default, lets set it anyways/
                m_log.write<Logging::CONSOLE_LOG>("Encoding set to UTF-8");
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

            if(m_term_type == "undetected" || 
               m_term_type.find("ansi",0) != std::string::npos || 
               m_term_type.find("ANSI",0) != std::string::npos)
            {
                // Switch to Unicode Character Set.
                message = "\x1b[0m" + m_session_io->pipeColors(blackColor);
                message += "\x1b%@\x1b%G \r\n\x1b[A";
                m_session_data->deliver(message);

                message = m_session_io->parseTextPrompt(
                              m_text_prompts_dao->getPrompt(PROMPT_UTF8_SELECTED)
                          );

                // Even though it's default, lets set it anyways/
                m_log.write<Logging::CONSOLE_LOG>("Encoding set to UTF-8");
                m_session_data->m_encoding_text = Encoding::ENCODING_TEXT_UTF8;
                m_session_data->m_encoding = Encoding::ENCODE_UTF8;
            }
            else
            {
                // Switch to ISO, then CP437 Character Set.
                message = "\x1b[0m" + m_session_io->pipeColors(blackColor);
                message += "\x1b%@\x1b(U \r\n\x1b[A";
                m_session_data->deliver(message);

                message = m_session_io->parseTextPrompt(
                              m_text_prompts_dao->getPrompt(PROMPT_CP437_SELECTED)
                          );

                // Even though it's default, lets set it anyways/
                m_log.write<Logging::CONSOLE_LOG>("Encoding set to CP437");
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

/**
 * @brief Start ESC Twice Timer
 */
void ModPreLogon::startHumanShieldTimer()
{
    // Add Deadline Timer for 1.5 seconds for complete Telopt Sequences responses
    //m_deadline_timer->setWaitInMilliseconds(8000);
    //m_deadline_timer->asyncWait(
    //    std::bind(&ModPreLogon::handleHumanShieldTimer, shared_from_this())
    //);
    auto callback_function = std::bind(&ModPreLogon::handleHumanShieldTimer, shared_from_this());        
    m_session_data->m_async_io->asyncWait(4000, callback_function);
}

/**
 * @brief Start ANSI Detection timer
 */
void ModPreLogon::startDetectionTimer()
{
    // Add Deadline Timer for 1.5 seconds for complete Telopt Sequences responses
    //m_deadline_timer->setWaitInMilliseconds(1500);
    //m_deadline_timer->asyncWait(
    //    std::bind(&ModPreLogon::handleDetectionTimer, shared_from_this())
    //);
    
    auto callback_function = std::bind(&ModPreLogon::handleDetectionTimer, shared_from_this());        
    m_session_data->m_async_io->asyncWait(1500, callback_function);
}

/**
 * @brief Deadline Detection Timer ESC Twice
 * @param timer
 */
void ModPreLogon::handleHumanShieldTimer()
{
    humanShieldCompleted();
}

/**
 * @brief ESC Twice Completed
 * @return
 */
void ModPreLogon::humanShieldCompleted()
{
    m_log.setUserInfo(m_session_data->m_node_number);
    if(m_is_human_shield)
    {
        // Move to Next Detection
        changeModule(MOD_DETECT_EMULATION);
    }
    else
    {
        // Logoff is too fast, Prompt is never displayed.. hmm
        displayPromptThenDisconnect(PROMPT_HUMAN_SHIELD_FAIL);
        
        // Disconnect User        
        m_is_active = false;
        m_log.write<Logging::CONSOLE_LOG>("Human Shield Failed, disconnecting!");
    }
}


/**
 * @brief Deadline Detection Timer for ANSI Detection
 * @param timer
 */
void ModPreLogon::handleDetectionTimer()
{
    // Jump to Emulation completed.
    emulationCompleted();
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
