#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP


#include <string>

extern std::string GLOBAL_BBS_PATH;      // BBS System
extern std::string GLOBAL_DATA_PATH;
extern std::string GLOBAL_MENU_PATH;
extern std::string GLOBAL_TEXTFILE_PATH;

/**
 * @brief Const Key values for global text prompt lookup
 */
const std::string GLOBAL_PROMPT_PAUSE = "pause";



/**
 * @brief List of interfaces for pipe2ansi conversion
 */
enum
{
    STANDARD_MCI,   // Access to All Standard MCI Codes and PIPE Colors
    MENU_PROMPT_MCI // Access to Menu Prompt MCI Codes and PIPE Colors
    // WIP rest, ie Stat screens and others with unique codes.
};

/**
 * @brief Saves All MCI and PIPE Matches with Offsets for replacment.
 */
typedef struct MapType
{
    std::string::size_type m_offset;
    std::string::size_type m_length;
    std::string::size_type m_match;
    std::string            m_code;
} MapType;


/*struct Area
{
   struct Area *Next;
   BOOL changed;
   struct Aka  *Aka;
   uchar Flags;
   uchar AreaType;
   uchar Tagname[80];
   uchar Description[80];
   struct Messagebase *Messagebase;
   uchar Path[80];
   uchar Group;

   struct jbList TossNodes;
   struct jbList BannedNodes;

   // Maint

   ulong KeepNum,KeepDays;

   // Stats

   ulong Texts;
   ulong NewTexts;
   ulong Dupes;
   ulong NewDupes;
   ushort Last8Days[8];
   time_t FirstTime;
   time_t LastTime;

   // Other

   BOOL scanned;
};
*/

/*
struct Sql_Area
{
   struct Sql_Area *Next;
   struct Area *area;
   ulong LowMsg;
   ulong HighMsg;
   ulong OldHighWater;
   ulong HighWater;
};*/


/* SQL
CREATE TABLE `marea_cfg` (
  `Id` int(11) NOT NULL auto_increment,        -- Unique Area ID
  `Area` varchar(80) NOT NULL default '',      -- Area / Filename ie 0N-BBS etc..
  `Desc` varchar(128) NOT NULL default '',     -- Description ie..  BBS Discussion
  `Type` tinyint(4) NOT NULL default '0',      -- Local,Email,Echomail,Netmail
  'Status' tinyint(4) NOT NULL default '0',    -- Public, Private
  `Maxmsg` int(11) NOT NULL default '0',       -- Max Messages Per area.
  `FTNdomain` varchar(64) NOT NULL default '', -- This will match AKA table with Node Address and Origin Lines.
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM ;

// Message Import (intermediate table container)
// MYSQL, rework to sqlite.
typedef struct FidoMessage {

  char Area[80];		// varchar(80) NOT NULL default '',
  char MSGID[80];		//`varchar(80) NOT NULL default '',
  char REPLY[80];		// Reply` varchar(80) NOT NULL default '',
  short Type;			// tinyint(4) NOT NULL default '0',
  char From[36];		// varchar(36) NOT NULL default '',
  char To[36];			// `To` varchar(36) NOT NULL default '',
  char FromAddr[56];		// varchar(56) NOT NULL default '',
  char ToAddr[56];		// varchar(56) NOT NULL default '',
  char Subject[72];		// ` varchar(72) NOT NULL default '',
  char CreatedBy[72];	// varchar(72) NOT NULL default '', // PID!
  ulong DateWritten; 	//  int(11) NOT NULL default '0',
  ulong DateArrive;	    //  int(11) NOT NULL default '0',
  int Attr;			    // int(11) NOT NULL default '0',
  std::string Kludge;    //` text NOT NULL,
  std::string MessageText; //` text NOT NULL,
  std::string SeenBy;	//` text NOT NULL,
  char ENCLFILE[100];   // File Attachment.
  char ENCLFREQ[100];   // File Request.

} FidoMessage;
 */

#endif // STRUCTURES_HPP
