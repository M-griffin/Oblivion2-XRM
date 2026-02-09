#ifndef IO_INPUT_HANDLER_HPP
#define IO_INPUT_HANDLER_HPP

#include <string>
#include <cwchar>

#include "io_common.hpp"
#include "util_log.hpp"
#include "tcp_session_wrapper.hpp"

class IoInputHandler {

    UtilLog &m_log;
    TcpSessionWrapper &m_session;
    IoCommon &m_io_common;

public:
    explicit IoInputHandler(TcpSessionWrapper &session, IoCommon &common)
        : m_log(UtilLog::getInstance())
        , m_session(session)
        , m_io_common(common) {

    }

    ~IoInputHandler() {
        m_log.log(UtilLog::LogLevel::Console, "~IoInputHandler()");
    }

    IoInputHandler(const IoInputHandler &other) = default;
    IoInputHandler(IoInputHandler &&other) = default;
    IoInputHandler & operator=(const IoInputHandler &other) noexcept = delete;
    IoInputHandler & operator=(IoInputHandler &&other) noexcept = delete;

    /**
     * @brief Single Key Input For Full Screen Editor or Esc Sequences
     * @param character_buffer
     * @return
     */
    std::string getFSEKeyInput(const std::string &character_buffer);

    /**
     * @brief Single Key Input or Esc Sequence
     * @param character_buffer
     * @return
     */
    std::string getKeyInput(const std::string &character_buffer);

    /**
     * @brief Generates an input field with ANSI color background
     * @param field_name {Is the Field label, precedes the field like 'Mail From: '}
     * @param len
     */
    void createInputField(std::string &field_name, int &len);

    /**
     * @brief Input Field, Returns output for keypresses, field stored in result on ENTER.
     * @param character_buffer
     * @param result
     * @param length
     * @param leadoff
     * @param hidden
     * @return
     */
    std::string getInputField(const std::string &character_buffer,
                              std::string &result,
                              int length,
                              std::string leadoff,
                              bool hidden);
};

#endif
