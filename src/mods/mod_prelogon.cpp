#include "mod_prelogon.hpp"

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


    /** if were in emulation detection, ANSI, we need to parse here
     *  for response, otherwise getInputField will eat this up!
     */

/*
    std::string input = "";
    std::string result = m_session_io.getInputField(character_buffer, input);
    if(result == "aborted") // ESC was hit, make this just clear the input text, or start over!
    {
        std::cout << "aborted!" << std::endl;
    }
    else if(result[0] == '\n')
    {
        // Send back the entire string.  TESTING
        // This should then be processed becasue ENTER was hit.
        //m_session_data->deliver(input);
        m_session_data->deliver(result);

        // Process the completed input for the string.
        m_mod_functions[m_mod_function_index](input);
    }
    else
    {
        // Send back the single input received TESTING
        m_session_data->deliver(result);
    }*/

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
    value[PROMPT_DETECTED_ANSI]    = std::make_pair("ANSI Emulation Detected", "|CR|04E|12m|14ulation: An|12s|04i");
    value[PROMPT_DETECTED_NONE]    = std::make_pair("Emulation Detect:None", "|CREmulation Detect: None");

    value[PROMPT_USE_ANSI]         = std::make_pair("Use ANSI Colors (Y/n) ", "|CR|08Press [|15Y|08 or |15ENTER|08] to use ANSI : ");

    value[PROMPT_USE_CP437]        = std::make_pair("Use CP437 Output Encoding", "|D1|08[|15Y|08] Select MS-DOS CP-437 Output |CR|08[|15N|08] Select UTF-8 Terminal Output");
    value[PROMPT_USE_UTF8]         = std::make_pair("Use UTF-8 Output Encoding", "|D1|08[|15Y|08] Select UTF-8 Terminal Output |CR|08[|15N|08] Select MS-DOS CP-437 Output");

    value[PROMPT_CODEPAGE_CP437]   = std::make_pair("Output Encoding, CP437 Default", "|D1|08Press [|15Y|08 or |15ENTER|08] to use ANSI : ");
    value[PROMPT_CODEPAGE_UTF8]    = std::make_pair("Output Encoding, UTF-8 Default", "|D1|08Press [|15Y|08 or |15ENTER|08] to use UTF-8 : ");

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

    // Deliver ANSI Location Sequence
    m_session_data->deliver("\x1b[s\x1b[255B\x1b[6n\x1b[1;1H");
    m_session_data->deliver("\x1b[u");

    M_StringPair prompt = m_text_prompts_dao->getPrompt(PROMPT_DETECT_EMULATION);
    //std::cout << "TEST: " << prompt.first << ", " << prompt.second << std::endl;
    std::string result = m_session_io.pipe2ansi(prompt.second);

    m_session_data->deliver(result);

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
    /*
    prompt = m_text_prompts_dao->getPrompt(PROMPT_USE_ANSI);
    std::cout << "TEST: " << prompt.first << ", " << prompt.second << std::endl;
    std::string result = m_session_io.pipe2ansi(prompt.second);
    m_session_data->deliver(result);*/
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
        if (ch == 27)
        {
            m_is_esc_detected = true;
        }

        // Check for sequence terminator.
        if (m_is_esc_detected)
        {
            if (toupper(ch) == 'R')
            {
                m_is_emulation_detected = true;
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
    m_is_esc_detected = false;

    if (m_is_emulation_detected)
    {
        // Emulation Detected ANSI
        M_StringPair prompt = m_text_prompts_dao->getPrompt(PROMPT_DETECTED_ANSI);
        std::cout << "TEST: " << prompt.first << ", " << prompt.second << std::endl;
        std::string result = m_session_io.pipe2ansi(prompt.second);
        m_session_data->deliver(result);
    }
    else
    {
        // Emulation Detected NONE
        M_StringPair prompt = m_text_prompts_dao->getPrompt(PROMPT_DETECTED_NONE);
        std::cout << "TEST: " << prompt.first << ", " << prompt.second << std::endl;
        std::string result = m_session_io.pipe2ansi(prompt.second);
        m_session_data->deliver(result);

        // Move to the next Method to ask to use ANSI Color
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

    // handle input for using ansi color, hot key or ENTER after..  hmmm

    /*
    std::string input = "";
    std::string result = m_session_io.getInputField(character_buffer, input);
    if(result == "aborted") // ESC was hit, make this just clear the input text, or start over!
    {
        std::cout << "aborted!" << std::endl;
    }
    else if(result[0] == '\n')
    {
        // Send back the entire string.  TESTING
        // This should then be processed becasue ENTER was hit.
        //m_session_data->deliver(input);
        m_session_data->deliver(result);

        // Process the completed input for the string.
        m_mod_functions[m_mod_function_index](input);
    }
    else
    {
        // Send back the single input received TESTING
        m_session_data->deliver(result);
    }*/
    return true;
}