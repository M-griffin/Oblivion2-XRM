##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=xrm-unittest
ConfigurationName      :=Release
WorkspacePath          :=../linux
ProjectPath            :=../linux
IntermediateDirectory  :=Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Mercyful
Date                   :=21/12/23
CodeLitePath           :=/home/merc/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)WITHOUT_SDL $(PreprocessorSwitch)UNIT_TEST 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="xrm-unittest.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -O2
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../src $(IncludeSwitch)../sqlitewrap $(IncludeSwitch)../linux/Release $(IncludeSwitch)/usr/include/unittest++ $(IncludeSwitch)/usr/include/UnitTest++ $(IncludeSwitch)../src/sdl2_net $(IncludeSwitch)../src/utf-cpp $(IncludeSwitch)../src/utf8 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)UnitTest++ $(LibrarySwitch)SqliteWrapped $(LibrarySwitch)sqlite3 $(LibrarySwitch)pthread $(LibrarySwitch)ssl $(LibrarySwitch)crypto $(LibrarySwitch)yaml-cpp $(LibrarySwitch)ssh $(LibrarySwitch)uuid 
ArLibs                 :=  "libUnitTest++.a" "libSqliteWrapped.a" "sqlite3" "pthread" "ssl" "crypto" "yaml-cpp" "libssh" "uuid" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)../linux/Release $(LibraryPathSwitch)../sqlitewrap $(LibraryPathSwitch)../src 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -Os -O2 -std=c++11 -Wno-psabi -Wno-deprecated-declarations $(Preprocessors)
CFLAGS   :=  -Os -O2 -std=c++11 -Wno-psabi -Wno-deprecated-declarations $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_processor_ansi.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_encryption.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_processor_text.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_logging.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_state_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_base.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_session.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_socket_telnet.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_processor_ansi_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_menu_base.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_processor_text_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_telnet_decoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_menu_system.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_processor_base.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_mod_message_editor_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_socket_handler.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_io_service.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d Release || $(MakeDirCommand) Release


$(IntermediateDirectory)/.d:
	@test -d Release || $(MakeDirCommand) Release

PreBuild:
	@echo Executing Pre Build commands ...
	sed -i 's+../+../+' xrm-unittest.mk
	@echo Done


##
## Objects
##
$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnetUDP.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(DependSuffix) -MM ../src/sdl2_net/SDLnetUDP.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/sdl2_net/SDLnetUDP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnetUDP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(PreprocessSuffix) ../src/sdl2_net/SDLnetUDP.cpp

$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnetTCP.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(DependSuffix) -MM ../src/sdl2_net/SDLnetTCP.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/sdl2_net/SDLnetTCP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnetTCP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(PreprocessSuffix) ../src/sdl2_net/SDLnetTCP.cpp

$(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(DependSuffix) -MM ../src/sdl2_net/SDLnet.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/sdl2_net/SDLnet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(PreprocessSuffix) ../src/sdl2_net/SDLnet.cpp

$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix): ../src/data-app/oneliners_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(DependSuffix) -MM ../src/data-app/oneliners_dao.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-app/oneliners_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(PreprocessSuffix): ../src/data-app/oneliners_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(PreprocessSuffix) ../src/data-app/oneliners_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix): ../src/data-sys/users_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(DependSuffix) -MM ../src/data-sys/users_dao.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/users_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(PreprocessSuffix): ../src/data-sys/users_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(PreprocessSuffix) ../src/data-sys/users_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix): ../src/data-sys/session_stats_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(DependSuffix) -MM ../src/data-sys/session_stats_dao.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/session_stats_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(PreprocessSuffix): ../src/data-sys/session_stats_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(PreprocessSuffix) ../src/data-sys/session_stats_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix): ../src/data-sys/message_area_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(DependSuffix) -MM ../src/data-sys/message_area_dao.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/message_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/message_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(PreprocessSuffix) ../src/data-sys/message_area_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix): ../src/data-sys/conference_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(DependSuffix) -MM ../src/data-sys/conference_dao.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/conference_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(PreprocessSuffix): ../src/data-sys/conference_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(PreprocessSuffix) ../src/data-sys/conference_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix): ../src/data-sys/config_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix) -MM ../src/data-sys/config_dao.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/config_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix): ../src/data-sys/config_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix) ../src/data-sys/config_dao.cpp

$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix): ../src/forms/form_system_config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(DependSuffix) -MM ../src/forms/form_system_config.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/forms/form_system_config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(PreprocessSuffix): ../src/forms/form_system_config.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(PreprocessSuffix) ../src/forms/form_system_config.cpp

$(IntermediateDirectory)/up_src_processor_ansi.cpp$(ObjectSuffix): ../src/processor_ansi.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_processor_ansi.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_processor_ansi.cpp$(DependSuffix) -MM ../src/processor_ansi.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/processor_ansi.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_processor_ansi.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_processor_ansi.cpp$(PreprocessSuffix): ../src/processor_ansi.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_processor_ansi.cpp$(PreprocessSuffix) ../src/processor_ansi.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(ObjectSuffix): ../src/data-sys/menu_prompt_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(DependSuffix) -MM ../src/data-sys/menu_prompt_dao.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/menu_prompt_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(PreprocessSuffix): ../src/data-sys/menu_prompt_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(PreprocessSuffix) ../src/data-sys/menu_prompt_dao.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(ObjectSuffix): ../xrm-unittest/src/xrm_test.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(DependSuffix) -MM ../xrm-unittest/src/xrm_test.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/xrm_test.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(PreprocessSuffix): ../xrm-unittest/src/xrm_test.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(PreprocessSuffix) ../xrm-unittest/src/xrm_test.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/oneliners_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(DependSuffix) -MM ../xrm-unittest/src/oneliners_dao_it.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/oneliners_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/oneliners_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(PreprocessSuffix) ../xrm-unittest/src/oneliners_dao_it.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix): ../src/data-sys/menu_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(DependSuffix) -MM ../src/data-sys/menu_dao.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/menu_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(PreprocessSuffix): ../src/data-sys/menu_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(PreprocessSuffix) ../src/data-sys/menu_dao.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/file_area_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(DependSuffix) -MM ../xrm-unittest/src/file_area_dao_it.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/file_area_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/file_area_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(PreprocessSuffix) ../xrm-unittest/src/file_area_dao_it.cpp

$(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(ObjectSuffix): ../src/mods/mod_menu_editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(DependSuffix) -MM ../src/mods/mod_menu_editor.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_menu_editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(PreprocessSuffix): ../src/mods/mod_menu_editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(PreprocessSuffix) ../src/mods/mod_menu_editor.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/message_area_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(DependSuffix) -MM ../xrm-unittest/src/message_area_dao_it.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/message_area_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/message_area_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(PreprocessSuffix) ../xrm-unittest/src/message_area_dao_it.cpp

$(IntermediateDirectory)/up_src_encryption.cpp$(ObjectSuffix): ../src/encryption.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_encryption.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_encryption.cpp$(DependSuffix) -MM ../src/encryption.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/encryption.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_encryption.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_encryption.cpp$(PreprocessSuffix): ../src/encryption.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_encryption.cpp$(PreprocessSuffix) ../src/encryption.cpp

$(IntermediateDirectory)/up_src_processor_text.cpp$(ObjectSuffix): ../src/processor_text.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_processor_text.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_processor_text.cpp$(DependSuffix) -MM ../src/processor_text.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/processor_text.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_processor_text.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_processor_text.cpp$(PreprocessSuffix): ../src/processor_text.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_processor_text.cpp$(PreprocessSuffix) ../src/processor_text.cpp

$(IntermediateDirectory)/up_src_logging.cpp$(ObjectSuffix): ../src/logging.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_logging.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_logging.cpp$(DependSuffix) -MM ../src/logging.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/logging.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_logging.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_logging.cpp$(PreprocessSuffix): ../src/logging.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_logging.cpp$(PreprocessSuffix) ../src/logging.cpp

$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix): ../src/data-sys/protocol_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(DependSuffix) -MM ../src/data-sys/protocol_dao.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/protocol_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(PreprocessSuffix): ../src/data-sys/protocol_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(PreprocessSuffix) ../src/data-sys/protocol_dao.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/session_io_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(DependSuffix) -MM ../xrm-unittest/src/session_io_ut.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/session_io_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/session_io_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/session_io_ut.cpp

$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(ObjectSuffix): ../src/mods/mod_prelogon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(DependSuffix) -MM ../src/mods/mod_prelogon.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_prelogon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(PreprocessSuffix): ../src/mods/mod_prelogon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(PreprocessSuffix) ../src/mods/mod_prelogon.cpp

$(IntermediateDirectory)/up_src_state_manager.cpp$(ObjectSuffix): ../src/state_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_state_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_state_manager.cpp$(DependSuffix) -MM ../src/state_manager.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/state_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_state_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_state_manager.cpp$(PreprocessSuffix): ../src/state_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_state_manager.cpp$(PreprocessSuffix) ../src/state_manager.cpp

$(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(ObjectSuffix): ../src/mods/mod_message_editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(DependSuffix) -MM ../src/mods/mod_message_editor.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_message_editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(PreprocessSuffix): ../src/mods/mod_message_editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(PreprocessSuffix) ../src/mods/mod_message_editor.cpp

$(IntermediateDirectory)/up_src_mods_mod_base.cpp$(ObjectSuffix): ../src/mods/mod_base.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_base.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_base.cpp$(DependSuffix) -MM ../src/mods/mod_base.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_base.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_base.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_base.cpp$(PreprocessSuffix): ../src/mods/mod_base.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_base.cpp$(PreprocessSuffix) ../src/mods/mod_base.cpp

$(IntermediateDirectory)/up_src_session.cpp$(ObjectSuffix): ../src/session.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_session.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_session.cpp$(DependSuffix) -MM ../src/session.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/session.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_session.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_session.cpp$(PreprocessSuffix): ../src/session.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_session.cpp$(PreprocessSuffix) ../src/session.cpp

$(IntermediateDirectory)/up_src_socket_telnet.cpp$(ObjectSuffix): ../src/socket_telnet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_socket_telnet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_socket_telnet.cpp$(DependSuffix) -MM ../src/socket_telnet.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/socket_telnet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_socket_telnet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_socket_telnet.cpp$(PreprocessSuffix): ../src/socket_telnet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_socket_telnet.cpp$(PreprocessSuffix) ../src/socket_telnet.cpp

$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix): ../src/common_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix) -MM ../src/common_io.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/common_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix): ../src/common_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix) ../src/common_io.cpp

$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix): ../src/data-sys/security_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(DependSuffix) -MM ../src/data-sys/security_dao.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/security_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(PreprocessSuffix): ../src/data-sys/security_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(PreprocessSuffix) ../src/data-sys/security_dao.cpp

$(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(ObjectSuffix): ../src/mods/mod_level_editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(DependSuffix) -MM ../src/mods/mod_level_editor.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_level_editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(PreprocessSuffix): ../src/mods/mod_level_editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(PreprocessSuffix) ../src/mods/mod_level_editor.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_processor_ansi_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/processor_ansi_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_processor_ansi_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_processor_ansi_ut.cpp$(DependSuffix) -MM ../xrm-unittest/src/processor_ansi_ut.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/processor_ansi_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_processor_ansi_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_processor_ansi_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/processor_ansi_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_processor_ansi_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/processor_ansi_ut.cpp

$(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnetselect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(DependSuffix) -MM ../src/sdl2_net/SDLnetselect.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/sdl2_net/SDLnetselect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnetselect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(PreprocessSuffix) ../src/sdl2_net/SDLnetselect.cpp

$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(ObjectSuffix): ../src/mods/mod_logon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(DependSuffix) -MM ../src/mods/mod_logon.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_logon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(PreprocessSuffix): ../src/mods/mod_logon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(PreprocessSuffix) ../src/mods/mod_logon.cpp

$(IntermediateDirectory)/up_src_menu_base.cpp$(ObjectSuffix): ../src/menu_base.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_menu_base.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_menu_base.cpp$(DependSuffix) -MM ../src/menu_base.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/menu_base.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_menu_base.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_menu_base.cpp$(PreprocessSuffix): ../src/menu_base.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_menu_base.cpp$(PreprocessSuffix) ../src/menu_base.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_processor_text_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/processor_text_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_processor_text_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_processor_text_ut.cpp$(DependSuffix) -MM ../xrm-unittest/src/processor_text_ut.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/processor_text_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_processor_text_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_processor_text_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/processor_text_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_processor_text_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/processor_text_ut.cpp

$(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(ObjectSuffix): ../src/mods/mod_user_editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(DependSuffix) -MM ../src/mods/mod_user_editor.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_user_editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(PreprocessSuffix): ../src/mods/mod_user_editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(PreprocessSuffix) ../src/mods/mod_user_editor.cpp

$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(ObjectSuffix): ../src/telnet_decoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(DependSuffix) -MM ../src/telnet_decoder.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/telnet_decoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(PreprocessSuffix): ../src/telnet_decoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_telnet_decoder.cpp$(PreprocessSuffix) ../src/telnet_decoder.cpp

$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix): ../src/data-sys/file_area_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(DependSuffix) -MM ../src/data-sys/file_area_dao.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/file_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/file_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(PreprocessSuffix) ../src/data-sys/file_area_dao.cpp

$(IntermediateDirectory)/up_src_menu_system.cpp$(ObjectSuffix): ../src/menu_system.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_menu_system.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_menu_system.cpp$(DependSuffix) -MM ../src/menu_system.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/menu_system.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_menu_system.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_menu_system.cpp$(PreprocessSuffix): ../src/menu_system.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_menu_system.cpp$(PreprocessSuffix) ../src/menu_system.cpp

$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix): ../src/encoding.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_encoding.cpp$(DependSuffix) -MM ../src/encoding.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/encoding.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_encoding.cpp$(PreprocessSuffix): ../src/encoding.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_encoding.cpp$(PreprocessSuffix) ../src/encoding.cpp

$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix): ../src/session_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_session_io.cpp$(DependSuffix) -MM ../src/session_io.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/session_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_session_io.cpp$(PreprocessSuffix): ../src/session_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_session_io.cpp$(PreprocessSuffix) ../src/session_io.cpp

$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix): ../src/data-sys/text_prompts_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(DependSuffix) -MM ../src/data-sys/text_prompts_dao.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/text_prompts_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(PreprocessSuffix): ../src/data-sys/text_prompts_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(PreprocessSuffix) ../src/data-sys/text_prompts_dao.cpp

$(IntermediateDirectory)/up_src_processor_base.cpp$(ObjectSuffix): ../src/processor_base.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_processor_base.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_processor_base.cpp$(DependSuffix) -MM ../src/processor_base.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/processor_base.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_processor_base.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_processor_base.cpp$(PreprocessSuffix): ../src/processor_base.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_processor_base.cpp$(PreprocessSuffix) ../src/processor_base.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_mod_message_editor_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/mod_message_editor_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_mod_message_editor_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_mod_message_editor_ut.cpp$(DependSuffix) -MM ../xrm-unittest/src/mod_message_editor_ut.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/mod_message_editor_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_mod_message_editor_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_mod_message_editor_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/mod_message_editor_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_mod_message_editor_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/mod_message_editor_ut.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/common_mock_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(DependSuffix) -MM ../xrm-unittest/src/common_mock_ut.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/common_mock_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/common_mock_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/common_mock_ut.cpp

$(IntermediateDirectory)/up_src_socket_handler.cpp$(ObjectSuffix): ../src/socket_handler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_socket_handler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_socket_handler.cpp$(DependSuffix) -MM ../src/socket_handler.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/socket_handler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_socket_handler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_socket_handler.cpp$(PreprocessSuffix): ../src/socket_handler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_socket_handler.cpp$(PreprocessSuffix) ../src/socket_handler.cpp

$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix): ../src/session_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_session_manager.cpp$(DependSuffix) -MM ../src/session_manager.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/session_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_session_manager.cpp$(PreprocessSuffix): ../src/session_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_session_manager.cpp$(PreprocessSuffix) ../src/session_manager.cpp

$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix): ../src/access_condition.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_access_condition.cpp$(DependSuffix) -MM ../src/access_condition.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/access_condition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_access_condition.cpp$(PreprocessSuffix): ../src/access_condition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_access_condition.cpp$(PreprocessSuffix) ../src/access_condition.cpp

$(IntermediateDirectory)/up_src_io_service.cpp$(ObjectSuffix): ../src/io_service.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_io_service.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_io_service.cpp$(DependSuffix) -MM ../src/io_service.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/io_service.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_io_service.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_io_service.cpp$(PreprocessSuffix): ../src/io_service.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_io_service.cpp$(PreprocessSuffix) ../src/io_service.cpp

$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix): ../src/communicator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_communicator.cpp$(DependSuffix) -MM ../src/communicator.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/communicator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_communicator.cpp$(PreprocessSuffix): ../src/communicator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_communicator.cpp$(PreprocessSuffix) ../src/communicator.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/access_condition_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(DependSuffix) -MM ../xrm-unittest/src/access_condition_ut.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/access_condition_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/access_condition_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/access_condition_ut.cpp

$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix): ../src/data-sys/access_level_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(DependSuffix) -MM ../src/data-sys/access_level_dao.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/access_level_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(PreprocessSuffix): ../src/data-sys/access_level_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(PreprocessSuffix) ../src/data-sys/access_level_dao.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/form_system_config_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(DependSuffix) -MM ../xrm-unittest/src/form_system_config_ut.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/form_system_config_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/form_system_config_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/form_system_config_ut.cpp

$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(ObjectSuffix): ../src/mods/mod_signup.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(DependSuffix) -MM ../src/mods/mod_signup.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_signup.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(PreprocessSuffix): ../src/mods/mod_signup.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(PreprocessSuffix) ../src/mods/mod_signup.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/common_io_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(DependSuffix) -MM ../xrm-unittest/src/common_io_ut.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/common_io_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/common_io_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/common_io_ut.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/conference_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(DependSuffix) -MM ../xrm-unittest/src/conference_dao_it.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/conference_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/conference_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(PreprocessSuffix) ../xrm-unittest/src/conference_dao_it.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r Release/


