
#include "communicator.hpp"
#include "struct.hpp"

#include <iostream>
#include <string>
#include <cstring>

/**
 * @brief  Global Singleton Instance, needed to initalize the class.
 */
Communicator* Communicator::m_globalInstance = nullptr;

/**
 * @brief Default Constructors required.
 * @return
 */
Communicator::Communicator()
{
    std::cout << "Communicator" << std::endl;
}

Communicator::~Communicator()
{
    std::cout << "~Communicator" << std::endl;
}


/**
 * @brief Loads the System configuration
 */
void Communicator::loadSystemConfig()
{
    // Should move this into a seperate class for ConfigIO!

    // Clear then load the record.
    memset(&m_config_record, 0, sizeof(ConfigRec));
    if (!readConfigurationRecord(&m_config_record, "CONFIG.BBS", 0))
    {
        std::cout << "Error Reading Configuration File: CONFIG.BBS in " << BBS_PATH << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "Configuration File Loaded." << std::endl;
    }

    // Convert only Pascal Strings to Cstrings
    m_common_io.PascalToCString(m_config_record.BBSDir);
    m_common_io.PascalToCString(m_config_record.TextDir);
    m_common_io.PascalToCString(m_config_record.BoardDir);
    m_common_io.PascalToCString(m_config_record.FilesDir);
    m_common_io.PascalToCString(m_config_record.DataDir);
    m_common_io.PascalToCString(m_config_record.MenuDir);
    m_common_io.PascalToCString(m_config_record.TFileDir);
    m_common_io.PascalToCString(m_config_record.DoorDir);
    m_common_io.PascalToCString(m_config_record.LogDir);
    m_common_io.PascalToCString(m_config_record.SysopName);
    m_common_io.PascalToCString(m_config_record.BoardName);
    m_common_io.PascalToCString(m_config_record.DSZLogName);
    m_common_io.PascalToCString(m_config_record.PostAnon);
    m_common_io.PascalToCString(m_config_record.SysopACS);
    m_common_io.PascalToCString(m_config_record.MSysopACS);
    m_common_io.PascalToCString(m_config_record.FSysopACS);
    m_common_io.PascalToCString(m_config_record.DefaultFlags);
    m_common_io.PascalToCString(m_config_record.Infoforms[0].InfoName);
    m_common_io.PascalToCString(m_config_record.Infoforms[1].InfoName);
    m_common_io.PascalToCString(m_config_record.Infoforms[2].InfoName);
    m_common_io.PascalToCString(m_config_record.Infoforms[3].InfoName);
    m_common_io.PascalToCString(m_config_record.Infoforms[4].InfoName);
    m_common_io.PascalToCString(m_config_record.SysPass);
    m_common_io.PascalToCString(m_config_record.System1Pass);
    m_common_io.PascalToCString(m_config_record.NewUserPass);
    m_common_io.PascalToCString(m_config_record.DescForm[0]);
    m_common_io.PascalToCString(m_config_record.DescForm[1]);
    m_common_io.PascalToCString(m_config_record.DescForm[2]);
    m_common_io.PascalToCString(m_config_record.DescForm[3]);
    m_common_io.PascalToCString(m_config_record.DescForm[4]);
    m_common_io.PascalToCString(m_config_record.DescForm[5]);
    m_common_io.PascalToCString(m_config_record.DescForm[6]);
    m_common_io.PascalToCString(m_config_record.DescForm[7]);

    // Most likely will not be used, maybe print out for fun!
    // Array is [NumNodes], we'll only use the first in the array!
    m_common_io.PascalToCString(m_config_record.InitStr[0]);
    m_common_io.PascalToCString(m_config_record.HangUpStr[0]);
    m_common_io.PascalToCString(m_config_record.OffHookStr[0]);
    m_common_io.PascalToCString(m_config_record.SendCarrierStr[0]);
    m_common_io.PascalToCString(m_config_record.ReceiveCarrierStr[0]);
    //
    m_common_io.PascalToCString(m_config_record.LockOutPass);
    m_common_io.PascalToCString(m_config_record.CanDoExec);
    m_common_io.PascalToCString(m_config_record.PostMand);
    m_common_io.PascalToCString(m_config_record.QWKName);
    m_common_io.PascalToCString(m_config_record.SMenu);
    m_common_io.PascalToCString(m_config_record.DefUserNote);
    m_common_io.PascalToCString(m_config_record.ComSpecifying);
    m_common_io.PascalToCString(m_config_record.ValCode);
    m_common_io.PascalToCString(m_config_record.DailyFlagChange);
    m_common_io.PascalToCString(m_config_record.CallFlagChange);
    m_common_io.PascalToCString(m_config_record.PostMand);
    m_common_io.PascalToCString(m_config_record.SwapDir);
    m_common_io.PascalToCString(m_config_record.FileMailACS);
    m_common_io.PascalToCString(m_config_record.TopTenACS);
    m_common_io.PascalToCString(m_config_record.RRACS);
    m_common_io.PascalToCString(m_config_record.ScanCom);
    m_common_io.PascalToCString(m_config_record.BBSAd);
    m_common_io.PascalToCString(m_config_record.CommentFN);
    m_common_io.PascalToCString(m_config_record.BadDir);
    m_common_io.PascalToCString(m_config_record.FilesList);
    m_common_io.PascalToCString(m_config_record.AskAvailabilityACS);
    m_common_io.PascalToCString(m_config_record.Special1ACS);
    m_common_io.PascalToCString(m_config_record.Special2ACS);
    m_common_io.PascalToCString(m_config_record.Special3ACS);
    m_common_io.PascalToCString(m_config_record.Special4ACS);
    m_common_io.PascalToCString(m_config_record.Special5ACS);
    m_common_io.PascalToCString(m_config_record.Special6ACS);
    m_common_io.PascalToCString(m_config_record.SpecialCACS);
    m_common_io.PascalToCString(m_config_record.SpecialDACS);
    m_common_io.PascalToCString(m_config_record.SpecialEACS);
    m_common_io.PascalToCString(m_config_record.SpecialFACS);
    m_common_io.PascalToCString(m_config_record.SpecialJACS);
    m_common_io.PascalToCString(m_config_record.SpecialMACS);
    m_common_io.PascalToCString(m_config_record.SpecialNACS);
    m_common_io.PascalToCString(m_config_record.SpecialQACS);
    m_common_io.PascalToCString(m_config_record.SpecialSACS);
    m_common_io.PascalToCString(m_config_record.SpecialTACS);
    m_common_io.PascalToCString(m_config_record.SpecialVACS);

    m_common_io.PascalToCString(m_config_record.MultiNodeDir);
    m_common_io.PascalToCString(m_config_record.HiddenFilesACS);
    m_common_io.PascalToCString(m_config_record.ChangeDirACS);
    m_common_io.PascalToCString(m_config_record.IncPromptsDir);
    m_common_io.PascalToCString(m_config_record.WFCOption[0].BatchFile);
    m_common_io.PascalToCString(m_config_record.WFCOption[0].Title);
    m_common_io.PascalToCString(m_config_record.WFCOption[1].BatchFile);
    m_common_io.PascalToCString(m_config_record.WFCOption[1].Title);
    m_common_io.PascalToCString(m_config_record.WFCOption[2].BatchFile);
    m_common_io.PascalToCString(m_config_record.WFCOption[2].Title);
    m_common_io.PascalToCString(m_config_record.WFCOption[3].BatchFile);
    m_common_io.PascalToCString(m_config_record.WFCOption[3].Title);
    m_common_io.PascalToCString(m_config_record.WFCOption[4].BatchFile);
    m_common_io.PascalToCString(m_config_record.WFCOption[4].Title);
    m_common_io.PascalToCString(m_config_record.WFCOption[5].BatchFile);
    m_common_io.PascalToCString(m_config_record.WFCOption[5].Title);
    m_common_io.PascalToCString(m_config_record.WFCOption[6].BatchFile);
    m_common_io.PascalToCString(m_config_record.WFCOption[6].Title);
    m_common_io.PascalToCString(m_config_record.WFCOption[7].BatchFile);
    m_common_io.PascalToCString(m_config_record.WFCOption[7].Title);
    m_common_io.PascalToCString(m_config_record.WFCOption[8].BatchFile);
    m_common_io.PascalToCString(m_config_record.WFCOption[8].Title);
}
