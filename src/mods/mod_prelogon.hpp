#ifndef MOD_PRELOGON_HPP
#define MOD_PRELOGON_HPP

#include "mod_base.hpp"

#include "../model/structures.hpp"
#include "../data/text_prompts_dao.hpp"

#include "../session_data.hpp"
#include "../session_io.hpp"

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/deadline_timer.hpp>

#include <vector>
#include <functional>

using boost::asio::deadline_timer;

/**
 * @class ModPreLogin
 * @author Michael Griffin
 * @date 3/17/2016
 * @file mod_logon.hpp
 * @brief System PreLogin Module
 */
class ModPreLogon
    : public boost::enable_shared_from_this<ModPreLogon>
    , public ModBase
{
public:
    ModPreLogon(session_data_ptr session_data)
        : ModBase(session_data)
        , m_session_io(session_data)
        , m_filename("mod_prelogon.yaml")
        , m_text_prompts_dao(new TextPromptsDao(GLOBAL_DATA_PATH, m_filename))
        , m_detection_deadline(session_data->m_io_service)
        , m_mod_function_index(MOD_DETECT_EMULATION)
        , m_is_text_prompt_exist(false)
        , m_is_esc_detected(false)
        , m_is_emulation_detected(false)
        , m_input_buffer("")
        , m_x_position(0)
        , m_y_position(0)
        , m_term_type("undetected")
    {
        std::cout << "ModPreLogon" << std::endl;

        // Push function pointers to the stack.
        m_setup_functions.push_back(std::bind(&ModPreLogon::setupEmulationDetection, this));
        m_setup_functions.push_back(std::bind(&ModPreLogon::setupAskANSIColor, this));
        m_setup_functions.push_back(std::bind(&ModPreLogon::setupAskCodePage, this));

        m_mod_functions.push_back(std::bind(&ModPreLogon::emulationDetection, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModPreLogon::askANSIColor, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModPreLogon::askCodePage, this, std::placeholders::_1));

        // Check of the Text Prompts exist.
        m_is_text_prompt_exist = m_text_prompts_dao->fileExists();
        if (!m_is_text_prompt_exist)
        {
            createTextPrompts();
        }

        // Loads all Text Prompts for current module
        m_text_prompts_dao->readPrompts();
    }

    virtual ~ModPreLogon()
    {
        std::cout << "~ModPreLogon" << std::endl;
        std::vector<std::function< void()> >().swap(m_setup_functions);
        std::vector<std::function< void(const std::string &)> >().swap(m_mod_functions);
    }

    virtual bool update(const std::string &character_buffer, const bool &);
    virtual bool onEnter();
    virtual bool onExit();

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

    const std::string PROMPT_ASK_CP437 = "use_cp437";
    const std::string PROMPT_ASK_UTF8 = "use_utf8";
    const std::string PROMPT_CP437_SELECTED = "cp437_selected";
    const std::string PROMPT_UTF8_SELECTED = "utf8_selected";


    /**
     * @brief Create Default Text Prompts for module
     */
    void createTextPrompts();

    /**
     * @brief Sets an indivdual module index.
     * @param mod_function_index
     */
    void changeModule(int mod_function_index);

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
     * @brief Ask Setup CodePage CP437 / UTF-8
     * @return
     */
    void setupAskCodePage();

    /**
     * @brief Start ANSI Detection timer
     */
    void startDetectionTimer()
    {
        // Add Deadline Timer for 1.5 seconds for complete Telopt Sequences reponses
        m_detection_deadline.expires_from_now(boost::posix_time::milliseconds(1500));
        m_detection_deadline.async_wait(
            boost::bind(&ModPreLogon::handleDetectionTimer, shared_from_this(), &m_detection_deadline));
    }


    /**
     * @brief Deadline Detection Timer for ANSI Detection
     * @param timer
     */
    void handleDetectionTimer(boost::asio::deadline_timer* /*timer*/)
    {
        std::cout << "Deadline ANSI Detection, EXPIRED!" << std::endl;

        // Jump to Emulation completed.
        emulationCompleted();
    }


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


    SessionIO            m_session_io;
    std::string          m_filename;
    text_prompts_dao_ptr m_text_prompts_dao;
    deadline_timer       m_detection_deadline;

    int                  m_mod_function_index;   
    bool                 m_is_text_prompt_exist;
    bool                 m_is_esc_detected;
    bool                 m_is_emulation_detected;
    std::string          m_input_buffer;
    int                  m_x_position;
    int                  m_y_position;
    std::string          m_term_type;
};

#endif // SYSTEM_STATE_HPP
