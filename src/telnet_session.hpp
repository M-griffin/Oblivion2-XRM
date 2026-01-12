#ifndef TELNET_SESSION_HPP
#define TELNET_SESSION_HPP

#include <string>
#include <unordered_set>
#include <vector>
#include <cstdint>

using Byte = uint8_t;
using ByteBuffer = std::vector<Byte>;

class Logging;
class SessionWriter;

class TelnetSession {
public:
    explicit TelnetSession(SessionWriter &writer);
    ~TelnetSession();

    // Move constructor
    TelnetSession(TelnetSession &&other) = delete;
    TelnetSession &operator=(TelnetSession &&other) = delete;

    // Delete copy constructor and copy assignment operator
    TelnetSession(const TelnetSession &) = delete;
    TelnetSession &operator=(const TelnetSession &) = delete;

    // Telnet parsing state
    enum TelnetState {
        DATA = 0,
        IAC_SEEN,
        COMMAND,
        SB_OPTION,
        SB_TTYPE_QUAL,
        SB_DATA,
        SB_IAC
    };

    // Main Methods
    bool isValidCommand(Byte command);
    ByteBuffer telnetOptionParse(Byte byte);
    Byte telnetOptionAcknowledge(Byte command);
    Byte telnetOptionDeny(Byte command);
    void sendIACSequences(Byte command, Byte option);
    bool checkReply(Byte option);
    void addReply(Byte option);

    void decodeSubnegotiationBuffer();
    void handleDoDont(Byte command, Byte option);
    void handleWillWont(Byte command, Byte option);
    void handleSubnegotiation(Byte option, const ByteBuffer &data);

    int getTermRows() const;
    int getTermCols() const;
    void setTermRows(int value);
    void setTermCols(int value);

    std::string getTermType() const;

    void sendTTYPERequest();
    void sendENVRequest();

private:
    Logging &m_log;
    SessionWriter &m_sessionWrite;

    int m_nawsRow;
    int m_nawsCol;

    bool m_isBinary;
    bool m_isEcho;
    bool m_isSga;
    bool m_isLinemode;
    bool m_isNawsDetected;

    std::string m_termType;

    TelnetState m_teloptStage;
    Byte m_teloptCommand;

    Byte m_currentOption;
    Byte m_subnegoOption;

    std::unordered_set<Byte> m_replySequence;
    ByteBuffer m_dataSequence;

    bool m_ttypeComplete{false};
};

#endif
