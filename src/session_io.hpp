#ifndef SESSION_IO_HPP
#define SESSION_IO_HPP

#include "session.hpp"
#include "common_io.hpp"

#include "model-sys/structures.hpp"

#include <memory>
#include <string>
#include <map>

class Logging;

class Config;
typedef std::shared_ptr<Config> config_ptr;


/**
 * @class SessionIO
 * @author Michael Griffin
 * @date 9/21/2015
 * @file session_io.hpp
 * @brief Higher Level I/O specific to Menu Interfaces
 */
class SessionIO
{
public:

    // Types for Text Prompt formatting to file.
    typedef std::pair<std::string, std::string> M_StringPair;

    explicit SessionIO();
    explicit SessionIO(session_ptr session_data);
    ~SessionIO();

    /**
     * @brief Single Key Input For Full Screen Editor or Esc Sequences
     * @param character_buffer
     * @return
     */
    std::string getFSEKeyInput(const std::string &character_buffer);

    /**
     * @brief Single Key Input or Esc Sequence parser.
     *        Only Accepts single characters at a time then passes through.
     *        Basically all input passes through till there is something to display
     *        Nothing loop on input in these functions to keep async.
     * @param character_buffer
     * @return
     */
    std::string getKeyInput(const std::string &character_buffer);

    /**
     * @brief Generates an input field with ANSI color background
     * @param field_name
     * @param len
     */
    void createInputField(std::string &field_name, int &len);

    /**
    * @brief Input Field, returns output for keypresses, completed field returns in result
    * @param character_buffer
    * @param result
    * @param length
    * @param leadoff
    * @param hidden
    * @return
    */
    std::string getInputField(const std::string &character_buffer, // Input.
                              std::string &result,      // Returned at [ENTER]
                              int length = 30,          // Default 30 Bytes
                              std::string leadoff = "", // Default None
                              bool hidden = false);     // Default Not hidden

    /**
     * @brief Parses for ANSI Foreground Colors
     * @param foreground
     */
    std::string pipeReplaceForeground(int foreground);

    /**
     * @brief Parses for ANSI Background Colors
     * @param background
     */
    std::string pipeReplaceBackground(int background);

    /**
     * @brief Parsed Pipe Colors Codes to ANSI ESC Sequence.
     * @param color_string
     * @return
     */
    std::string pipeColors(const std::string &color_string);


    /**
     * @brief Gets the Default Color Sequence
     * @return
     */
    std::string getDefaultColor(config_ptr config);

    /**
     * @brief Gets the Default Input Color Sequence
     * @return
     */
    std::string getDefaultInputColor(config_ptr config);

    /**
     * @brief Gets the Default Inverse Color Sequence
     * @return
     */
    std::string getDefaultInverseColor(config_ptr config);

    /**
     * @brief Gets the Default Prompt Color Sequence
     * @return
     */
    std::string getDefaultPromptColor(config_ptr config);

    /**
     * @brief Gets the Default stat Color Sequence
     * @return
     */
    std::string getDefaultStatColor(config_ptr config);

    /**
     * @brief Gets the Default box Color Sequence
     * @return
     */
    std::string getDefaultBoxColor(config_ptr config);


    /**
    * @brief Parsed Pipe Codes with 1 or 2 Digits
    * @param pipe_code
    * @return
    */
    std::string parsePipeWithCharsDigits(const std::string &code, int value);

    /**
    * @brief Separate Pipe Codes with 1 or 2 Digits
    * @param pipe_code
    * @return
    */
    std::string separatePipeWithCharsDigits(const std::string &pipe_code);

    /**
     * @brief Parse Files %%Filename.ext
     * @param pipe_code
     * @return
     */
    std::string parseFilename(const std::string &pipe_code);

    /**
     * @brief Parse Pipe Codes with no Following Digits
     * @param pipe_code
     * @return
     */
    std::string parsePipeWithChars(const std::string &pipe_code);

    /**
     * @brief Parses Code Map and replaces screen codes with ANSI Sequences.
     * @param screen
     * @param code_map
     * @return
     */
    std::string parseCodeMap(const std::string &screen, std::vector<MapType> &code_map);

    /**
     * @brief Parses Code Map and replaces screen codes with Generic Items.
     * @param screen
     * @param code_map
     * @return
     */
    std::string parseCodeMapGenerics(const std::string &screen, const std::vector<MapType> &code_map);

    /**
     * @brief Parses string and returns code mapping and positions
     * @param sequence
     * @return
     */
    std::vector<MapType> parseToCodeMap(const std::string &sequence, const std::string &expression);

    /**
     * @brief Converts MCI Sequences to Ansi screen output.
     * @param sequence
     * @return
     */
    std::string pipe2ansi(const std::string &sequence);

    /**
     * @brief Converts MCI Sequences to Code Maps for Multiple Parses of same string data
     * @param sequence
     * @return
     */
    std::vector<MapType> pipe2genericCodeMap(const std::string &sequence);

    /**
     * @brief Converts MCI Sequences to Code Maps for Prompt Strings
     * @param sequence
     * @return
     */
    std::vector<MapType> pipe2promptCodeMap(const std::string &sequence);

    /**
     * @brief Converts MCI Sequences to Code Maps for Prompt Formatting Strings
     * @param sequence
     * @return
     */
    std::vector<MapType> pipe2promptFormatCodeMap(const std::string &sequence);

    /**
     * @brief Colorizes Brackets and Text between brackets to system theme colors
     * @param sequence
     * @return
     */
    std::string parseFormatColorsBrackets(const std::string &sequence, config_ptr config);

    /**
     * @brief Colorizes Colons to system theme colors
     * @param sequence
     * @return
     */
    std::string parseFormatColorsColon(const std::string &sequence, config_ptr config);

    /**
     * @brief Parses unformatted prompt text and adds colors to brackets and colon's.
     * @param sequence
     * @return
     */
    std::string pipe2promptFormat(const std::string &sequence, config_ptr config);

    /**
     * @brief Checks a String if it matches the expression passed.
     * @param sequence
     * @param expression
     * @return
     */
    bool checkRegex(const std::string &sequence, const std::string &expression);

    /**
     * @brief Stores Key (MCI Code) Value (String for Replacement) in Mapping
     * @param key
     * @param value
     * @return
     */
    std::string parseTextPrompt(const M_StringPair &prompt);


    /**
     * @brief Parses Text Prompt String Pair
     * @param prompt
     * @return
     */
    void addMCIMapping(const std::string &key, const std::string &value);


    /**
     * @brief Clears all mappings
     */
    void clearAllMCIMapping();

    /**
     * @brief Get a Count of all Mapped MCI Codes
     * @return
     */
    int getMCIMappingCount();

    // Internal Methods
    Logging                           &m_log;
    session_ptr                        m_session_data; // SessionData
    CommonIO                           m_common_io;    // CommonIO
    std::map<std::string, std::string> m_mapped_codes; // MCI Code Translation for specific screens.

    const std::string STD_EXPRESSION = {"([|]{1}[0-9]{2})|([|]{1}[X][Y][0-9]{4})|"
                                        "([|]{1}[A-Z]{1,2}[0-9]{1,2})|([|]{1}[A-Z]{2})|"
                                        "([%]{2}[\\w]+[.]{1}[\\w]{3})|([%]{1}[A-Z]{2})|"
                                        "([%]{1}[0-9]{2})"
                                       };

    const std::string MID_EXPRESSION = {"([|]{1}[A-Z]{1}[0-9]{1,2})|([|]{1}[A-Z]{2})"};

    const std::string PROMPT_EXPRESSION = {"([\\^]{1}[A-Z]{1})|([\\\\/=|@*:#)(]{1}$)"};

    const std::string FORMAT_EXPRESSION = {"([[]{1}[\\w\\W]+[]]{1})|([:]{1})"};
};





#endif // MENU_IO_HPP
