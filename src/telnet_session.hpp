#ifndef TELNET_SESSION_HPP
#define TELNET_SESSION_HPP

#include <string>
#include <utility>
#include <unordered_set>

#include "session.hpp"
#include "model-sys/users.hpp"

class TelnetSession {

public:
    explicit TelnetSession(Session &session);
    ~TelnetSession();

    // Move constructor
    TelnetSession(TelnetSession&& other) noexcept
          : m_log(other.m_log),
          m_session(other.m_session),
          m_nawsRow(std::move(other.m_nawsRow)),
          m_nawsCol(std::move(other.m_nawsCol)),
          m_termType(std::move(other.m_termType)),
          m_isBinary(std::move(other.m_isBinary)),
          m_isEcho(std::move(other.m_isEcho)),
          m_isSga(std::move(other.m_isSga)),
          m_isLinemode(std::move(other.m_isLinemode)),
          m_isNawsDetected(std::move(other.m_isNawsDetected)),
          m_isUseAnsi(std::move(other.m_isUseAnsi)),
          m_teloptStage(std::move(other.m_teloptStage)),
          m_teloptCommand(std::move(other.m_teloptCommand)),
          m_currentOption(std::move(other.m_currentOption)),
          m_subnegoOption(std::move(other.m_subnegoOption)),
          m_replySequence(std::move(other.m_replySequence)),
          m_dataSequence(std::move(other.m_dataSequence)) {
        // Reset the moved-from object to a valid state
        other.m_nawsRow = 0;
        other.m_nawsCol = 0;
        other.m_termType.clear();
        other.m_isBinary = false;
        other.m_isEcho = false;
        other.m_isSga = false;
        other.m_isLinemode = false;
        other.m_isNawsDetected = false;
        other.m_isUseAnsi = false;
        other.m_teloptStage = 0;
        other.m_teloptCommand = 0;
        other.m_currentOption = 0;
        other.m_subnegoOption = 0;
        other.m_replySequence.clear();
        other.m_dataSequence.clear();
    }

    // Move assignment operator
    TelnetSession& operator=(TelnetSession&& other) noexcept {
        if (this != &other) {
            m_nawsRow = std::move(other.m_nawsRow);
            m_nawsCol = std::move(other.m_nawsCol);
            m_termType = std::move(other.m_termType);
            m_isBinary = std::move(other.m_isBinary);
            m_isEcho = std::move(other.m_isEcho);
            m_isSga = std::move(other.m_isSga);
            m_isLinemode = std::move(other.m_isLinemode);
            m_isNawsDetected = std::move(other.m_isNawsDetected);
            m_isUseAnsi = std::move(other.m_isUseAnsi);
            m_teloptStage = std::move(other.m_teloptStage);
            m_teloptCommand = std::move(other.m_teloptCommand);
            m_currentOption = std::move(other.m_currentOption);
            m_subnegoOption = std::move(other.m_subnegoOption);
            m_replySequence = std::move(other.m_replySequence);
            m_dataSequence = std::move(other.m_dataSequence);

            // Reset the moved-from object to a valid state
            other.m_nawsRow = 0;
            other.m_nawsCol = 0;
            other.m_termType.clear();
            other.m_isBinary = false;
            other.m_isEcho = false;
            other.m_isSga = false;
            other.m_isLinemode = false;
            other.m_isNawsDetected = false;
            other.m_isUseAnsi = false;
            other.m_teloptStage = 0;
            other.m_teloptCommand = 0;
            other.m_currentOption = 0;
            other.m_subnegoOption = 0;
            other.m_replySequence.clear();
            other.m_dataSequence.clear();
        }
        return *this;
    }

    // Delete copy constructor and copy assignment operator
    TelnetSession(const TelnetSession&) = delete;
    TelnetSession& operator=(const TelnetSession&) = delete;

    void sendIACSequences(unsigned char command, int option);

    bool checkReply(const unsigned char& option) const;
    void addReply(const unsigned char& option);

    void decodeBuffer();

    static bool isValidCommand(unsigned char command) ;
    void handleDoDont(unsigned char command, unsigned char option);
    void handleWillWont(unsigned char command, unsigned char option);

    unsigned char telnetOptionParse(const unsigned char& c);

    int getTermRows() const;
    int getTermCols() const;

    void setTermRows(int value);
    void setTermCols(int value);

    std::string getTermType() const;

    static unsigned char telnetOptionAcknowledge(const unsigned char& command);
    static unsigned char telnetOptionDeny(const unsigned char& command);

    void sendTTYPERequest();
    void sendENVRequest();

    Users &getUserRec();
    void setUserRec(const Users &userRec);

    void setUseAnsi(bool value);
    bool getUseAnsi() const;

private:
    Logging &m_log;
    Session &m_session;

    int m_nawsRow;
    int m_nawsCol;
    std::string m_termType;

    bool m_isBinary;
    bool m_isEcho;
    bool m_isSga;
    bool m_isLinemode;
    bool m_isNawsDetected;
    bool m_isUseAnsi;

    int m_teloptStage;
    int m_teloptCommand;
    unsigned char m_currentOption;
    unsigned char m_subnegoOption;

    std::unordered_set<int> m_replySequence;
    std::string m_dataSequence;

};

#endif