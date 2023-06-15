#ifndef MOD_SIGNUP_HPP
#define MOD_SIGNUP_HPP

#include "mod_base.hpp"

#include <string>
#include <memory>
#include <vector>
#include <functional>

class Security;
typedef std::shared_ptr<Security> security_ptr;

class MenuBase;
typedef std::shared_ptr<MenuBase> menu_base_ptr;

class Users;
typedef std::shared_ptr<Users> user_ptr;

/**
 * @class ModSignup
 * @author Michael Griffin
 * @date 3/28/2016
 * @file mod_signup.hpp
 * @brief New User Application Module
 */
class ModSignup
    : public ModBase
{
public:
    ModSignup(session_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process);

    virtual ~ModSignup() override
    {
        std::vector<std::function< void()> >().swap(m_setup_functions);
        std::vector<std::function< void(const std::string &)> >().swap(m_mod_functions);
    }

    virtual bool update(const std::string &character_buffer, const bool &) override;
    virtual bool onEnter() override;
    virtual bool onExit() override;

    // This matches the index for mod_functions.push_back
    enum
    {
        MOD_NUP,
        MOD_DISCLAIMER,
        MOD_HANDLE,
        MOD_REALNAME,
        MOD_ADDRESS,
        MOD_LOCATION,
        MOD_COUNTRY,
        MOD_EMAIL,
        MOD_USERNOTE,
        MOD_BIRTHDATE,
        MOD_GENDER,
        MOD_PASSWORD,
        MOD_PASSWORD_VERIFY,
        MOD_CHALLENGE_QUEST,
        MOD_CHALLENGE_ANSWER,
        MOD_CHALLENGE_VERIFY,
        MOD_YESNOBARS,
        MOD_PAUSE,
        MOD_CLEAR_SCREEN,
        MOD_ANSI_COLOR,
        MOD_BACKSPACE,
        MOD_VERIFY_SAVE
    };

    const bool USE_HIDDEN_OUTPUT_TRUE = true;
    const bool USE_HIDDEN_OUTPUT_FALSE = false;

    // Create Prompt Constants, these are the keys for key/value lookup
    const std::string PROMPT_NUP = "newuser_password";
    const std::string PROMPT_DISCLAIMER = "disclaimer";
    const std::string PROMPT_HANDLE = "handle";
    const std::string PROMPT_REAL_NAME = "real_name";
    const std::string PROMPT_USER_NUMBER = "user_number";
    const std::string PROMPT_ADDRESS = "address";
    const std::string PROMPT_LOCATION = "location";
    const std::string PROMPT_COUNTRY = "country";
    const std::string PROMPT_EMAIL = "email";
    const std::string PROMPT_USER_NOTE = "user_note";
    const std::string PROMPT_BIRTH_DATE = "birth_date";
    const std::string PROMPT_GENDER = "gender";
    const std::string PROMPT_PASSWORD = "password";
    const std::string PROMPT_VERIFY_PASSWORD = "verify_password";
    const std::string PROMPT_CHALLENGE_QUESTION = "challenge_question";
    const std::string PROMPT_CHALLENGE_ANSWER = "challenge_answer";
    const std::string PROMPT_VERIFY_ANSWER = "verify_answer";
    const std::string PROMPT_YESNO_BARS = "yes_no_bars";
    const std::string PROMPT_USE_PAUSE = "use_pause";
    const std::string PROMPT_USE_CLEAR = "use_clear";
    const std::string PROMPT_USE_ANSI_COLOR = "use_ansi_color";
    const std::string PROMPT_BACK_SPACE = "use_back_space";
    const std::string PROMPT_VERIFY_SAVE = "verify_save";

    const std::string PROMPT_TEXT_INVALID = "text_invalid";
    const std::string PROMPT_DATE_INVALID = "date_invalid";
    const std::string PROMPT_PASS_INVALID = "pass_invalid";
    const std::string PROMPT_HANDLE_INVALID = "handle_invalid";
    const std::string PROMPT_NAME_INVALID = "name_invalid";
    const std::string PROMPT_EMAIL_INVALID = "email_invalid";

    const std::string PROMPT_SAVED = "confirmed_save";
    const std::string PROMPT_NOT_SAVED = "record_not_saved";

    const std::string PROMPT_AUTO_VALIDATION = "auto_validation";
    const std::string PROMPT_NOT_VALIDATED = "not_validated";
    const std::string PROMPT_NEWUSER_VOTING = "new_user_voting";


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
     * @brief Changes to Next module index.
     */
    void changeNextModule();

    /**
     * @brief Changes to Previous module index.
     */
    void changePreviousModule();

    /**
     * @brief Redisplay's the current module prompt.
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
     * @brief New User Password
     * @return
     */
    void setupNewUserPassword();

    /**
     * @brief Pre Application Disclaimer
     * @return
     */
    void setupDisclaimer();

    /**
     * @brief Get Handle from User
     * @return
     */
    void setupHandle();

    /**
     * @brief Get Real Name
     * @return
     */
    void setupRealName();

    /**
     * @brief Get Address
     * @return
     */
    void setupAddress();

    /**
     * @brief Get Location
     * @return
     */
    void setupLocation();

    /**
     * @brief Get Country
     * @return
     */
    void setupCountry();

    /**
     * @brief Get Email
     * @return
     */
    void setupEmail();

    /**
     * @brief Get UserNote
     * @return
     */
    void setupUserNote();

    /**
     * @brief Get Birthdate
     * @return
     */
    void setupBirthday();

    /**
     * @brief Get Gender
     * @return
     */
    void setupGender();

    /**
     * @brief Get Password
     * @return
     */
    void setupPassword();

    /**
     * @brief Verify Password
     * @return
     */
    void setupVerifyPassword();

    /**
     * @brief Set Password Reset Challenge Question
     * @return
     */
    void setupChallengeQuestion();

    /**
     * @brief Set Password Reset Challenge Answer
     * @return
     */
    void setupChallengeAnswer();

    /**
     * @brief Set Password Reset Challenge Answer
     * @return
     */
    void setupVerifyChallengeAnswer();

    /**
     * @brief Get Yes / No Bar Preference
     * @return
     */
    void setupYesNoBars();

    /**
     * @brief Get Pause Preference
     * @return
     */
    void setupDoPause();

    /**
     * @brief Get Clear or Scroll preference
     * @return
     */
    void setupClearOrScroll();

    /**
     * @brief Get ANSI Color preference
     * @return
     */
    void setupAnsiColor();

    /**
     * @brief Get Backspace Preference WINDOWS/LINUX (Terminal)
     * @return
     */
    void setupBackSpace();

    /**
     * @brief Confirm and save user record.
     * @return
     */
    void setupVerifyAndSave();

private:

    /**
     * @brief Base Class for shared input (receives function pointer for custom execution)
     *
     * @param input
     * @param field_length
     * @param useHiddenOutput
     * @param func_pointer
     * @return true
     * @return false
     */
    bool fieldInputAndProcess(std::string input, int field_length, bool use_hidden_output,
        std::function<bool(std::string &, std::string &)> function_pointer);

    /**
     * @brief New User Password
     * @return
     */
    bool newUserPassword(const std::string &input);

    /**
     * @brief Pre Application Disclaimer
     * @return
     */
    bool disclaimer(const std::string &input);

    /**
     * @brief Get Handle from User
     * @return
     */
    bool handle(const std::string &input);

    /**
     * @brief Get Real Name
     * @return
     */
    bool realName(const std::string &input);

    /**
     * @brief Get Address
     * @return
     */
    bool address(const std::string &input);

    /**
     * @brief Get Location
     * @return
     */
    bool location(const std::string &input);

    /**
     * @brief Get Country
     * @return
     */
    bool country(const std::string &input);

    /**
     * @brief Get Email
     * @return
     */
    bool email(const std::string &input);

    /**
     * @brief Get UserNote
     * @return
     */
    bool userNote(const std::string &input);

    /**
     * @brief Get Birthdate
     * @return
     */
    bool birthday(const std::string &input);

    /**
     * @brief Get Gender
     * @return
     */
    bool gender(const std::string &input);

    /**
     * @brief Get Password
     * @return
     */
    bool password(const std::string &input);

    /**
     * @brief Verify Password
     * @return
     */
    bool verifyPassword(const std::string &input);

    /**
     * @brief Set Password Reset Challenge Question
     * @return
     */
    bool challengeQuestion(const std::string &input);

    /**
     * @brief Set Password Reset Challenge Answer
     * @return
     */
    bool challengeAnswer(const std::string &input);

    /**
     * @brief Set Password Reset Challenge Answer
     * @return
     */
    bool verifyChallengeAnswer(const std::string &input);

    /**
     * @brief Get Yes / No Bar Preference
     * @return
     */
    bool yesNoBars(const std::string &input);

    /**
     * @brief Get Pause Preference
     * @return
     */
    bool doPause(const std::string &input);

    /**
     * @brief Get Clear or Scroll preference
     * @return
     */
    bool clearOrScroll(const std::string &input);

    /**
     * @brief Get ANSI Color preference
     * @return
     */
    bool ansiColor(const std::string &input);

    /**
     * @brief Get Backspace Preference WINDOWS/LINUX (Terminal)
     * @return
     */
    bool backSpace(const std::string &input);

    /**
     * @brief Confirm and save user record.
     * @return
     */
    bool verifyAndSave(const std::string &input);

    /**
     * @brief Save New User Record
     */
    void saveNewUserRecord();

    // Function Input Vector.
    std::vector<std::function< void()> >                    m_setup_functions;
    std::vector<std::function< void(const std::string &)> > m_mod_functions;

    session_io_ptr         m_session_io;
    menu_base_ptr          m_menu_base;
    std::string            m_filename;
    text_prompts_dao_ptr   m_text_prompts_dao;
    user_ptr               m_user_record;
    security_ptr           m_security_record;

    int                    m_mod_function_index;
    bool                   m_is_text_prompt_exist;
    int                    m_newuser_password_attempts;

};

#endif // MOD_SIGNUP_HPP
