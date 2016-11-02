#ifndef SESSION_IO_HPP
#define SESSION_IO_HPP

#include "session_data.hpp"
#include "common_io.hpp"

#include "model/structures.hpp"

#include <string>
#include <map>

/**
 * @class SessionIO
 * @author Michael Griffin
 * @date 9/21/2015
 * @file session_io.hpp
 * @brief Higher Level I/O sepecific to Menu Interfaces
 */
class SessionIO
{
public:

    // Types for Text Prompt formatting to file.
    typedef std::pair<std::string, std::string> M_StringPair;

    SessionIO(session_data_ptr session_data);
    ~SessionIO();

    /**
     * @brief Single Key Input or Esc Sequence parser.
     *        Only Accepts single characters at a time then passes through.
     *        Bascially all input passes through till there is something to display
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
    std::string pipeReplaceForground(int foreground);

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
    * @brief Parsed Pipe Codes with 1 or 2 Digits
    * @param pipe_code
    * @return
    */
    std::string parsePipeWithCharsDigits(const std::string &code, int value);

    /**
    * @brief Seperate Pipe Codes with 1 or 2 Digits
    * @param pipe_code
    * @return
    */
    std::string seperatePipeWithCharsDigits(const std::string &pipe_code);

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
     * @brief Parses Code Map and replaces ansi_string codes with ANSI Sequences.
     * @param ansi_string
     * @param code_map
     * @return
     */
    std::string parseCodeMap(std::string ansi_string, std::vector<MapType> &code_map);

    /**
     * @brief Parses string and returns code mapping and positions
     * @param sequence
     * @return
     */
    std::vector<MapType> pipe2codeMap(const std::string &sequence);

    /**
     * @brief Converts MCI Sequences to Ansi screen output.
     * @param sequence
     * @return
     */
    std::string pipe2ansi(const std::string &sequence);

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


    // Internal Methods
    session_data_ptr                   m_session_data; // SessionData
    CommonIO                           m_common_io;    // CommonIO
    std::map<std::string, std::string> m_mapped_codes; // MCI Code Translation for specific screens.
};

#endif // MENU_IO_HPP
