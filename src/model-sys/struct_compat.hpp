#ifndef STRUCT_HPP
#define STRUCT_HPP

#include <cstdio>
#include <string>
#include <iostream>
#include <stdint.h>
/*
 * This file contains structs for OBV/2 v2.30 data files
 */

 /**
 * Note: Null terminated strings are 1 longer then pascal
 * Basic Pascal to C++ Type Translations.
 * These are setup for specific 8, 16 and 32 bit widths
 * to make cross platform and cpu compatible.
 */
typedef int8_t LStr[81];    // 80
typedef int8_t MStr[36];    // 35
typedef int8_t SStr[16];    // 15
typedef int8_t Str8[9];     // 8

                            // Range                       Bytes
typedef int8_t   Char;      // -128 .. 127                 1
typedef uint8_t  Byte;      // 0 .. 255	                   1
typedef bool     Boolean;   // 0 .. 255	                   1
typedef int8_t   ShortInt;  // -128 .. 127                 1

typedef int16_t  Integer;   // -32768 .. 32767             2
typedef uint16_t Word;      // 0 .. 65535	               2

typedef int32_t  LongInt;   // -2147483648 .. 2147483647   4
typedef uint32_t LongWord;  // 0 .. 4294967295             4

// Undetermined length, much check String[0] for length.
typedef int8_t  String[256];

const   Byte MaxMesLines  = 150;
const   Byte NumPrompts   = 205;
const   Byte NumNodes     = 15;
const   Byte NumLast      = 20;



/**
 * @class TextPrompt
 * @author Michael Griffin
 * @date 3/17/2016
 * @brief Language File Text Prompts
 */
#pragma pack(push, 1)
typedef struct TextPrompt
{
    LStr    Desc;
    String  Prompt; // Not determined length, might parse manually on String[0]
} TextPrompt;
#pragma pack(pop)


/**
 * @class MenuPromptCompat
 * @author Michael Griffin
 * @date 9/22/2015
 * @brief Menu Prompt Record
 */
#pragma pack(push, 1)
typedef struct MenuPromptCompat
{
    MStr    Name;
    LStr    Data[3];
} MenuPromptCompat;
#pragma pack(pop)


/**
 * @class Statistics
 * @author Michael Griffin
 * @date 18/08/2015
 * @brief BBS Statistics Record
 */
#pragma pack(push, 1)
typedef struct Statistics
{
    LongInt TFiles,
            TPosts;
    Word    SysopMail;
    LongInt UserLastOn,
            LastAtWFC[NumNodes];       // : Array[1..NumNodes] Of LongInt;
    Word    NumBases;
    Byte    PageStatus;
    LongInt LastPChange,
            TotalCalls,
            TimeUsedToday[NumNodes];   // : Array[1..NumNodes] Of LongInt;
    Word    CallsToday,
            FilesToday,
            PostsToday,
            NewUsersToday;
} Statistics;
#pragma pack(pop)


/**
 * @class WFCO
 * @author Michael Griffin
 * @date 18/08/2015
 * @brief Waiting For Callers Options
 */
#pragma pack(push, 1)
typedef struct WFCO
{
    SStr    Title,
            BatchFile;
} WFCO;
#pragma pack(pop)


/**
 * @class InfoformData
 * @author Michael Griffin
 * @date 19/08/2015
 * @brief Holds Back Container for Infoform Data.
 */
#pragma pack(push, 1)
typedef struct InfoformData  // :  Array[1..5] Of Record
{
    MStr    InfoName;
    Boolean MandatoryInfoform;
} InfoformData;
#pragma pack(pop)

/**
 * @class ConfigRec
 * @author Michael Griffin
 * @date 18/08/2015
 * @brief BBS Configuration Record
 */
#pragma pack(push, 1)
typedef struct ConfigRec
{
    // {Communication Info}
    Word    Unused;

    // {Directory Info}
    LStr    BBSDir,
            TextDir,
            BoardDir,
            FilesDir,
            DataDir,
            MenuDir,
            WorkDir,
            TFileDir,
            FileMailDir,
            DoorDir,
            LogDir;

    // {Board Info}
    MStr    SysopName,
            BoardName;

    // {File Info}
    Word    KForEachPoint;
    Boolean AutoValidFiles,
            ExternalFileAreaListing,
            FRepeatMethod,
            CommissionFP;
    Word    PercentCTU;
    LStr    DSZLogName;
    Word    PointsBackPC;

    // {ACS Flags}
    MStr    PostAnon,
            SysopACS,
            MSysopACS,
            FSysopACS;

    // {EMail Info}
    Boolean AllowFileMail;
    Word    DaysToKeepFM;

    // {MultiNode Information}
    Boolean MultiNodeOp;
    Word    NodeNumber;

    // {Message Area}
    Boolean ExternalBaseListing,
            MRepeatMethod;

    // {Color Config}
    Byte    DefRegColor,
            DefStatColor,
            DefPromptColor,
            DefInputColor,
            DefInverseColor,
            DefBoxColor;

    // {New User Configuration}
    MStr    DefaultFlags;
    Word    DefLevel,
            DefFLevel,
            DefFilePoints,
            DefUDR,
            DefUDKR,
            DnKPerDay,
            DefPCR,
            DefUpCPS,
            DefDnCPS,
            DefTimeLimit;
    Word    NumMinsBeforeTimeOut;
    Char    HiddenInputChar;

    // {Infoforms}
    InfoformData Infoforms[5];  // :  Array[1..5] Of Record

    Boolean Unused2;
    MStr    SysPass,
            System1Pass,
            NewUserPass;
    Char    DescForm[46][8];    // : Array[1..8] Of String[45];
    Word    NumExt,
            PercentTimeBack;
    Word    Ports[NumNodes];    // : Array[1..NumNodes] Of Word;
    LStr    InitStr[NumNodes],  // : Array[1..NumNodes] Of LStr;
            HangUpStr[NumNodes],
            OffHookStr[NumNodes],
            SendCarrierStr[NumNodes],
            ReceiveCarrierStr[NumNodes];
    Word    DefBaud[NumNodes],  // : Array[1..NumNodes] Of Word;
            MinBaud[NumNodes];
    MStr    LockOutPass;
    Boolean ManualAnswer[NumNodes]; // : Array[1..NumNodes] Of Boolean;
    Word    DaysToSaveLog;
    MStr    CanDoExec;
    Word    ScrollLines,
            TotPTimes;
    MStr    PostMand;
    SStr    QWKName;
    Boolean UseBios,
            UseMaintain,
            KickOffNotV;
    SStr    SMenu;
    Boolean SysopAutologin,
            SLineOn,
            UseNuv;
    Word    VotesToValid,
            VotesToDel;
    MStr    DefUserNote,
            ComSpecifying;
    Word    MinKForUpload;
    MStr    ValCode,
            DailyFlagChange,
            CallFlagChange;
    Boolean QWKCheckTL,
            UseCPS;
    LStr    SwapDir;
    Word    UserTransferLine;
    Boolean LibraryDAAnsi,
            SearchAll;
    MStr    FileMailACS,
            TopTenACS,
            RRACS;
    Boolean UseULChecker;
    Word    DayConstraint;
    LStr    ScanCom;
    SStr    BBSAd,
            CommentFN;
    LStr    BadDir;
    SStr    FilesList;
    Boolean CheckSpace,
            ClearLFOpts;
    Char    TopTenC;
    Word    OptsPerLine;
    Boolean ReadMenuForGeneric;
    Word    DaysToAddFNuv;
    Boolean UseFileCatalog,
            FileCatalogLookup,
            ConfSearch,
            UseQuoteHeader,
            UseGreaterThanSign;
    Word    NumLPNode;
    Boolean UseRip;
    MStr    AskAvailabilityACS;
    Boolean UsePrelogon,
            UseWelcome,
            SpecialLogin;
    MStr    Special1ACS,       // {- Commands (Control)}
            Special2ACS,       // {& Commands (Multinode)}
            Special3ACS,       // {{ Commands (Matrix)}
            Special4ACS,       // {. Commands (Doors)}
            Special5ACS,       // {* Commands (Sysop)}
            Special6ACS,       // {^ Commands (NUV)}
            SpecialCACS,       // {C Commands (Conference Editor)}
            SpecialDACS,       // {D Commands (Data Area Editor)}
            SpecialEACS,       // {E Commands (Email)}
            SpecialFACS,       // {F Commands (File)}
            SpecialJACS,       // {J Commands (Jump Conference)}
            SpecialMACS,       // {M Commands (Message Base)}
            SpecialNACS,       // {N Commands (Singular Read)}
            SpecialQACS,       // {Q Commands (QWK Menu)}
            SpecialSACS,       // {S Commands (Message Base Sponsor)}
            SpecialTACS,       // {T Commands (File Sponsor)}
            SpecialVACS;       // {V Commands (Voting)}
    Boolean AllowUseofGandIMesComs,
            HangUpAfterCalls,
            ExternalMesHelp,
            ExternalFileHelp,
            YNCharsInApp,
            MakeDoorForMaintain,
            SwapToEMS,
            SwapToXMS,
            OpenCloseFDF;
    Boolean Lockat1152[NumNodes],       // : Array[1..NumNodes] Of Boolean;
            WaitUntilFlushed[NumNodes], // : Array[1..NumNodes] Of Boolean;
            GiveUpTimeSlices;
    LStr    MultiNodeDir;
    Boolean InformUsers;
    Word    ExtractPercentage;
    Boolean DoLogChat,
            RenameFiles,
            RunCopy,
            MultinodeFriendly,
            DontDoDevCheck;
    Char    BaseAddresses[5][NumNodes]; // : Array[1..NumNodes] Of String[4];
    Word    IrqNums[NumNodes];          // : Array[1..NumNodes] Of Word;
    Boolean NewUserVotingInfoform[5];   // : Array[1..5] Of Boolean;
    MStr    HiddenFilesACS,
            ChangeDirACS;
    LStr    IncPromptsDir;
    Boolean InformLogoff;
    WFCO    WFCOption[9];               // : Array[1..9] Of WFCO;
    Boolean UseWFCOpts,
            AutoValNewUsers;
} ConfigRec;
#pragma pack(pop)

// File Description Attributes.
enum FDesAttrType { Del, Reg, Last, FIDL };

/**
 * @class FDescRec
 * @author Michael Griffin
 * @date 18/08/2015
 * @brief File Description
 */
#pragma pack(push, 1)
typedef struct FDescRec
{
    FDesAttrType Attr;
    Char         FDes[46];
} FDescRec;
#pragma pack(pop)


/**
 * @class EMessageRec
 * @author Michael Griffin
 * @date 18/08/2015
 * @brief Email Message Record
 */
#pragma pack(push, 1)
typedef struct EMessageRec
{
    LStr    T[MaxMesLines];   // : Array[1..MaxMesLines] Of LStr;
    Integer Numlines;
    MStr    SentBy;
    MStr    SentTo;
    Boolean Anon;
    MStr    Title;
    Boolean AutoSigOn;
} EMessageRec;
#pragma pack(pop)


/**
 * @class UserRec
 * @author Michael Griffin
 * @date 18/08/2015
 * @brief User Record
 */
#pragma pack(push, 1)
typedef struct UserRec
{
    MStr    Handle,
            RealName,
            PhoneNum,
            Address,
            Location,
            Password,
            UserNote;
    Str8    BirthDay;
    Integer Infoforms[5];      // Array[1..5] Of Integer;
    Integer Level,
            Flevel;
    LongInt LastFiles,
            LastPosts,
            LastCall;
    Integer LastLevel,
            LastFLevel,
            OFilePoints;
    LongInt UpK,
            DnK;
    Byte    UDR,
            UDKR;
    Word    DnKPerDay,
            DnKToday,
            Uploads,
            Downloads,
            Posts,
            Calls,
            TimeLeft,
            TimeLimit;
    Byte    RegColor,
            PromptColor,
            InputColor,
            InverseColor,
            StatColor,
            BoxColor,
            PCR,
            Unused3,
            Unused4,
            PromptSel,
            MenuSel,
            StatusSel;
    Boolean Ansi,
            Avatar,
            vt100;
    Boolean FlConfig[10];      // : Array[1..10] Of Boolean;
    Byte    NuvVotesYes,
            NuvVotesNo;
    Word    UVotingYes[30];    // : Array[1..30] Of Word;
    Word    UVotingNo[30];     // : Array[1..30] Of Word;
    Integer ExtraMes;
    Char    DefProto[6];       // : Array[1..6] Of Char;
    LongInt PassChangeD;
    Word    Reserved[182];     // : Array[0..181] Of Word;
    LongInt LastRepDate;
    Boolean ScrollFL,
            Rip;
    Word    CallsToday,
            NewLevel,
            CSPassChange;
    Byte    CurArchiver;
    Boolean ReDisMes;
    Boolean CFlags[26];        // : Array[1..26] Of Boolean;
    Byte    Unused32[32];      // Fix for Set, Byte Array of 32!!
    Word    AverageUpCPS,
            AverageDnCPS;
    Boolean Wanted;
    LStr    AutoSig[5];        // : Array[1..5] Of LStr;
    Boolean UseFSE;
    Byte    HeaderType,
            LastMesConf,
            LastFileConf;
    Boolean ReplyReading;
    Word    NetMailB;
    Boolean AnsiAbort;
    LongInt ExpDate;
    Boolean IsMale;
    LongInt FirstOn;
    Boolean YesNoBars;
    Word    HackAttempts;
    Byte    VotingRec[35];     // : Array[1..35] Of Byte;
    LStr    NUVComments[10];   // : Array[1..10] Of LStr;
    Word    TimeBank;
    Byte    PageLen;
    Boolean DoPause;
    Word    TimeDesT;
    LongInt FilePoints;
    SStr    UInfoN[5];         // : Array[1..5] Of SStr;
    Boolean Flags2[26];        // : Array[1..26] Of Boolean;
    Byte    Unused[84];        // : Array[1..84] Of Byte;

} UserRec;
#pragma pack(pop)

/**
 * @class MenuOption
 * @author Michael Griffin
 * @date 9/1/2015
 * @brief Menu Option Record
 */
#pragma pack(push, 1)
typedef struct MenuCompatOption
{
    MStr    Acs,
            OptName;
    Boolean Hidden;
    Char    CKeys[3];
    SStr    Keys;
    MStr    CString;
    Word    PulldownID;
} MenuCompatOption;
#pragma pack(pop)

/**
 * @class MenuInfo
 * @author Michael Griffin
 * @date 9/1/2015
 * @brief Menu Record
 */
#pragma pack(push, 1)
typedef struct MenuCompatInfo
{
    MStr    Name,
            Password;
    SStr    FallBack,
            HelpID;
    MStr    ACS;
    MStr    NameInPrompt;
    MStr    MenuTitle;
    MStr    PulldownFN;
} MenuCompatInfo;
#pragma pack(pop)

/**
 * @class FileArea
 * @author Michael Griffin
 * @date 9/1/2015
 * @brief File Area Record
 */
#pragma pack(push, 1)
typedef struct FileArea
{
    MStr    Name,
            UploadACS,
            DownloadACS,
            ListACS,
            AccessACS,
            Sponsor,
            Password;
    SStr    AreaFName;
    LStr    Path,
            Reserved1,
            Reserved2,
            Reserved3;
    Char    DefSort;
    Byte    FileAreaMult;
    Boolean FreeArea,
            CopyFiles;
    Byte    A5,
            A6,
            A7;
} FileArea;
#pragma pack(pop)

/**
 * @class FRecord
 * @author Michael Griffin
 * @date 9/1/2015
 * @brief File Record
 */
#pragma pack(push, 1)
typedef struct FRecord
{
    SStr    FileName,
            Password;
    MStr    SendTo,
            Path,
            Uploader;
    LongInt WhenUp,
            WhenValid,
            LineSrt;
    Char    FID[46];
    Byte    DescLines;
    Boolean NewF,
            ResumeLater;
    LongInt FileSize;
    Integer Points;
    Byte    TimesDLed;
    Boolean Offline,
            FreeFile;
    Byte    Reserved[6]; //    : Array[1..6] Of Byte;
} FRecord;
#pragma pack(pop)

/**
 * @class ProtoRec
 * @author Michael Griffin
 * @date 9/1/2015
 * @brief Protocol Command Records
 */
#pragma pack(push, 1)
typedef struct ProtoRec
{
    Char Key;
    Char Desc[31];
    Char ProgName[13];
    Char Cline[61];
} ProtoRec;
#pragma pack(pop)


#endif
