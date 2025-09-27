#ifndef TELNET_DECODER_HPP
#define TELNET_DECODER_HPP

#include <string>
#include <unordered_set>

class Session;
class Logging;

class TelnetDecoder {
public:
    explicit TelnetDecoder(Session& session);
    ~TelnetDecoder();

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

    void sendTTYPERequest() const;
    void sendENVRequest() const;

private:
    Logging& log;
    Session& session;

    int nawsRow;
    int nawsCol;
    std::string termType;

    bool isBinary;
    bool isEcho;
    bool isSga;
    bool isLinemode;
    bool isNawsDetected;

    int teloptStage;
    int teloptCommand;
    unsigned char currentOption;
    unsigned char subnegoOption;

    std::unordered_set<int> replySequence;
    std::string dataSequence;

};

#endif // TELNET_DECODER_HPP