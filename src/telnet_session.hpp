#ifndef TELNET_SESSION_HPP
#define TELNET_SESSION_HPP

#include <string>
#include <utility>
#include <unordered_set>
#include <vector>
#include <cstdint>

using Byte = uint8_t;
using ByteBuffer = std::vector<Byte>;

class Logging;
class Session;

class TelnetSession {
public:
    explicit TelnetSession(Session &session);

    ~TelnetSession();

    // Move constructor
    TelnetSession(TelnetSession &&other) noexcept
        : m_log(other.m_log),
          m_session(other.m_session),
          m_nawsRow(other.m_nawsRow),
          m_nawsCol(other.m_nawsCol),
          m_termType(std::move(other.m_termType)),
          m_isBinary(other.m_isBinary),
          m_isEcho(other.m_isEcho),
          m_isSga(other.m_isSga),
          m_isLinemode(other.m_isLinemode),
          m_isNawsDetected(other.m_isNawsDetected),
          m_isUseAnsi(other.m_isUseAnsi),
          m_teloptStage(other.m_teloptStage),
          m_teloptCommand(other.m_teloptCommand),
          m_currentOption(other.m_currentOption),
          m_subnegoOption(other.m_subnegoOption),
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
    TelnetSession &operator=(TelnetSession &&other) noexcept {
        if (this != &other) {
            m_nawsRow = other.m_nawsRow;
            m_nawsCol = other.m_nawsCol;
            m_termType = std::move(other.m_termType);
            m_isBinary = other.m_isBinary;
            m_isEcho = other.m_isEcho;
            m_isSga = other.m_isSga;
            m_isLinemode = other.m_isLinemode;
            m_isNawsDetected = other.m_isNawsDetected;
            m_isUseAnsi = other.m_isUseAnsi;
            m_teloptStage = other.m_teloptStage;
            m_teloptCommand = other.m_teloptCommand;
            m_currentOption = other.m_currentOption;
            m_subnegoOption = other.m_subnegoOption;
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
    TelnetSession(const TelnetSession &) = delete;

    TelnetSession &operator=(const TelnetSession &) = delete;

    // Main Methods
    bool isValidCommand(Byte command);
    ByteBuffer telnetOptionParse(Byte byte);
    Byte telnetOptionAcknowledge(Byte command);
    Byte telnetOptionDeny(Byte command);
    void sendIACSequences(Byte command, Byte option);
    bool checkReply(Byte option);
    void addReply(Byte option);

    void decodeBuffer();
    void handleDoDont(Byte command, Byte option);
    void handleWillWont(Byte command, Byte option);

    int getTermRows() const;

    int getTermCols() const;

    void setTermRows(int value);

    void setTermCols(int value);

    std::string getTermType() const;

    void sendTTYPERequest();

    void sendENVRequest();

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

    Byte m_currentOption;
    Byte m_subnegoOption;

    std::unordered_set<Byte> m_replySequence;
    ByteBuffer m_dataSequence;
};

#endif
