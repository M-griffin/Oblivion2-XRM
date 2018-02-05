##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=xrm-server
ConfigurationName      :=Debug
WorkspacePath          :=/home/blue/code/Oblivion2-XRM/linux
ProjectPath            :=/home/blue/code/Oblivion2-XRM/linux
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=
Date                   :=02/04/18
CodeLitePath           :=/home/blue/.codelite
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
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="xrm-server.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../src $(IncludeSwitch)../sqlitewrap $(IncludeSwitch)../sqlitewrap/Debug 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)boost_system $(LibrarySwitch)boost_thread $(LibrarySwitch)boost_chrono $(LibrarySwitch)boost_locale $(LibrarySwitch)boost_filesystem $(LibrarySwitch)boost_timer $(LibrarySwitch)boost_regex $(LibrarySwitch)boost_date_time $(LibrarySwitch)SqliteWrapped $(LibrarySwitch)sqlite3 $(LibrarySwitch)pthread $(LibrarySwitch)ssl $(LibrarySwitch)crypto $(LibrarySwitch)yaml-cpp $(LibrarySwitch)util 
ArLibs                 :=  "boost_system" "boost_thread" "boost_chrono" "boost_locale" "boost_filesystem" "boost_timer" "boost_regex" "boost_date_time" "libSqliteWrapped.a" "sqlite3" "pthread" "ssl" "crypto" "yaml-cpp" "util" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)../sqlitewrap $(LibraryPathSwitch)../sqlitewrap/Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -Wfatal-errors -g -O0 -pedantic -W -std=c++14 -Wall -Wno-deprecated -Wno-unused-local-typedefs -Wno-psabi $(Preprocessors)
CFLAGS   :=  -Wfatal-errors -g -O0 -pedantic -W -std=c++14 -Wall -Wno-deprecated -Wno-unused-local-typedefs -Wno-psabi $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_encryption.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_menu_system.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_menu_system_editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_session_data.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_state_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_telnet_decoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_menu_base.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_process_posix.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_sys_config.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) 



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
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix): ../src/ansi_processor.cpp $(IntermediateDirectory)/up_src_ansi_processor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/ansi_processor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ansi_processor.cpp$(DependSuffix): ../src/ansi_processor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ansi_processor.cpp$(DependSuffix) -MM ../src/ansi_processor.cpp

$(IntermediateDirectory)/up_src_ansi_processor.cpp$(PreprocessSuffix): ../src/ansi_processor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ansi_processor.cpp$(PreprocessSuffix) ../src/ansi_processor.cpp

$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix): ../src/common_io.cpp $(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/common_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix): ../src/common_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix) -MM ../src/common_io.cpp

$(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix): ../src/common_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix) ../src/common_io.cpp

$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix): ../src/communicator.cpp $(IntermediateDirectory)/up_src_communicator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/communicator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_communicator.cpp$(DependSuffix): ../src/communicator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_communicator.cpp$(DependSuffix) -MM ../src/communicator.cpp

$(IntermediateDirectory)/up_src_communicator.cpp$(PreprocessSuffix): ../src/communicator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_communicator.cpp$(PreprocessSuffix) ../src/communicator.cpp

$(IntermediateDirectory)/up_src_encryption.cpp$(ObjectSuffix): ../src/encryption.cpp $(IntermediateDirectory)/up_src_encryption.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/encryption.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_encryption.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_encryption.cpp$(DependSuffix): ../src/encryption.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_encryption.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_encryption.cpp$(DependSuffix) -MM ../src/encryption.cpp

$(IntermediateDirectory)/up_src_encryption.cpp$(PreprocessSuffix): ../src/encryption.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_encryption.cpp$(PreprocessSuffix) ../src/encryption.cpp

$(IntermediateDirectory)/up_src_main.cpp$(ObjectSuffix): ../src/main.cpp $(IntermediateDirectory)/up_src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_main.cpp$(DependSuffix): ../src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_main.cpp$(DependSuffix) -MM ../src/main.cpp

$(IntermediateDirectory)/up_src_main.cpp$(PreprocessSuffix): ../src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_main.cpp$(PreprocessSuffix) ../src/main.cpp

$(IntermediateDirectory)/up_src_menu_system.cpp$(ObjectSuffix): ../src/menu_system.cpp $(IntermediateDirectory)/up_src_menu_system.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/menu_system.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_menu_system.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_menu_system.cpp$(DependSuffix): ../src/menu_system.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_menu_system.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_menu_system.cpp$(DependSuffix) -MM ../src/menu_system.cpp

$(IntermediateDirectory)/up_src_menu_system.cpp$(PreprocessSuffix): ../src/menu_system.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_menu_system.cpp$(PreprocessSuffix) ../src/menu_system.cpp

$(IntermediateDirectory)/up_src_menu_system_editor.cpp$(ObjectSuffix): ../src/menu_system_editor.cpp $(IntermediateDirectory)/up_src_menu_system_editor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/menu_system_editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_menu_system_editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_menu_system_editor.cpp$(DependSuffix): ../src/menu_system_editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_menu_system_editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_menu_system_editor.cpp$(DependSuffix) -MM ../src/menu_system_editor.cpp

$(IntermediateDirectory)/up_src_menu_system_editor.cpp$(PreprocessSuffix): ../src/menu_system_editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_menu_system_editor.cpp$(PreprocessSuffix) ../src/menu_system_editor.cpp

$(IntermediateDirectory)/up_src_session_data.cpp$(ObjectSuffix): ../src/session_data.cpp $(IntermediateDirectory)/up_src_session_data.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/session_data.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_session_data.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_session_data.cpp$(DependSuffix): ../src/session_data.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_session_data.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_session_data.cpp$(DependSuffix) -MM ../src/session_data.cpp

$(IntermediateDirectory)/up_src_session_data.cpp$(PreprocessSuffix): ../src/session_data.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_session_data.cpp$(PreprocessSuffix) ../src/session_data.cpp

$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix): ../src/session_io.cpp $(IntermediateDirectory)/up_src_session_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/session_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_session_io.cpp$(DependSuffix): ../src/session_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_session_io.cpp$(DependSuffix) -MM ../src/session_io.cpp

$(IntermediateDirectory)/up_src_session_io.cpp$(PreprocessSuffix): ../src/session_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_session_io.cpp$(PreprocessSuffix) ../src/session_io.cpp

$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix): ../src/session_manager.cpp $(IntermediateDirectory)/up_src_session_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/session_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_session_manager.cpp$(DependSuffix): ../src/session_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_session_manager.cpp$(DependSuffix) -MM ../src/session_manager.cpp

$(IntermediateDirectory)/up_src_session_manager.cpp$(PreprocessSuffix): ../src/session_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_session_manager.cpp$(PreprocessSuffix) ../src/session_manager.cpp

$(IntermediateDirectory)/up_src_state_manager.cpp$(ObjectSuffix): ../src/state_manager.cpp $(IntermediateDirectory)/up_src_state_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/state_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_state_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_state_manager.cpp$(DependSuffix): ../src/state_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_state_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_state_manager.cpp$(DependSuffix) -MM ../src/state_manager.cpp

$(IntermediateDirectory)/up_src_state_manager.cpp$(PreprocessSuffix): ../src/state_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_state_manager.cpp$(PreprocessSuffix) ../src/state_manager.cpp

$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(ObjectSuffix): ../src/telnet_decoder.cpp $(IntermediateDirectory)/up_src_telnet_decoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/telnet_decoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(DependSuffix): ../src/telnet_decoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(DependSuffix) -MM ../src/telnet_decoder.cpp

$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(PreprocessSuffix): ../src/telnet_decoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_telnet_decoder.cpp$(PreprocessSuffix) ../src/telnet_decoder.cpp

$(IntermediateDirectory)/up_src_menu_base.cpp$(ObjectSuffix): ../src/menu_base.cpp $(IntermediateDirectory)/up_src_menu_base.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/menu_base.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_menu_base.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_menu_base.cpp$(DependSuffix): ../src/menu_base.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_menu_base.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_menu_base.cpp$(DependSuffix) -MM ../src/menu_base.cpp

$(IntermediateDirectory)/up_src_menu_base.cpp$(PreprocessSuffix): ../src/menu_base.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_menu_base.cpp$(PreprocessSuffix) ../src/menu_base.cpp

$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix): ../src/access_condition.cpp $(IntermediateDirectory)/up_src_access_condition.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/access_condition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_access_condition.cpp$(DependSuffix): ../src/access_condition.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_access_condition.cpp$(DependSuffix) -MM ../src/access_condition.cpp

$(IntermediateDirectory)/up_src_access_condition.cpp$(PreprocessSuffix): ../src/access_condition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_access_condition.cpp$(PreprocessSuffix) ../src/access_condition.cpp

$(IntermediateDirectory)/up_src_process_posix.cpp$(ObjectSuffix): ../src/process_posix.cpp $(IntermediateDirectory)/up_src_process_posix.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/process_posix.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_process_posix.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_process_posix.cpp$(DependSuffix): ../src/process_posix.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_process_posix.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_process_posix.cpp$(DependSuffix) -MM ../src/process_posix.cpp

$(IntermediateDirectory)/up_src_process_posix.cpp$(PreprocessSuffix): ../src/process_posix.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_process_posix.cpp$(PreprocessSuffix) ../src/process_posix.cpp

$(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix): ../src/form_manager.cpp $(IntermediateDirectory)/up_src_form_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/form_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_form_manager.cpp$(DependSuffix): ../src/form_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_form_manager.cpp$(DependSuffix) -MM ../src/form_manager.cpp

$(IntermediateDirectory)/up_src_form_manager.cpp$(PreprocessSuffix): ../src/form_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_form_manager.cpp$(PreprocessSuffix) ../src/form_manager.cpp

$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix): ../src/data-sys/config_dao.cpp $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-sys/config_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix): ../src/data-sys/config_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix) -MM ../src/data-sys/config_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix): ../src/data-sys/config_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix) ../src/data-sys/config_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix): ../src/data-sys/security_dao.cpp $(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-sys/security_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(DependSuffix): ../src/data-sys/security_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(DependSuffix) -MM ../src/data-sys/security_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(PreprocessSuffix): ../src/data-sys/security_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(PreprocessSuffix) ../src/data-sys/security_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix): ../src/data-sys/text_prompts_dao.cpp $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-sys/text_prompts_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(DependSuffix): ../src/data-sys/text_prompts_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(DependSuffix) -MM ../src/data-sys/text_prompts_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(PreprocessSuffix): ../src/data-sys/text_prompts_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(PreprocessSuffix) ../src/data-sys/text_prompts_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix): ../src/data-sys/users_dao.cpp $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-sys/users_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(DependSuffix): ../src/data-sys/users_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(DependSuffix) -MM ../src/data-sys/users_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(PreprocessSuffix): ../src/data-sys/users_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(PreprocessSuffix) ../src/data-sys/users_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix): ../src/data-sys/menu_dao.cpp $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-sys/menu_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(DependSuffix): ../src/data-sys/menu_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(DependSuffix) -MM ../src/data-sys/menu_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(PreprocessSuffix): ../src/data-sys/menu_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(PreprocessSuffix) ../src/data-sys/menu_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(ObjectSuffix): ../src/data-sys/menu_prompt_dao.cpp $(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-sys/menu_prompt_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(DependSuffix): ../src/data-sys/menu_prompt_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(DependSuffix) -MM ../src/data-sys/menu_prompt_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(PreprocessSuffix): ../src/data-sys/menu_prompt_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(PreprocessSuffix) ../src/data-sys/menu_prompt_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix): ../src/data-sys/session_stats_dao.cpp $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-sys/session_stats_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(DependSuffix): ../src/data-sys/session_stats_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(DependSuffix) -MM ../src/data-sys/session_stats_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(PreprocessSuffix): ../src/data-sys/session_stats_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(PreprocessSuffix) ../src/data-sys/session_stats_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix): ../src/data-sys/conference_dao.cpp $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-sys/conference_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(DependSuffix): ../src/data-sys/conference_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(DependSuffix) -MM ../src/data-sys/conference_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(PreprocessSuffix): ../src/data-sys/conference_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(PreprocessSuffix) ../src/data-sys/conference_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix): ../src/data-sys/file_area_dao.cpp $(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-sys/file_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(DependSuffix): ../src/data-sys/file_area_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(DependSuffix) -MM ../src/data-sys/file_area_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/file_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(PreprocessSuffix) ../src/data-sys/file_area_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(ObjectSuffix): ../src/data-sys/grouping_dao.cpp $(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-sys/grouping_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(DependSuffix): ../src/data-sys/grouping_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(DependSuffix) -MM ../src/data-sys/grouping_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(PreprocessSuffix): ../src/data-sys/grouping_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(PreprocessSuffix) ../src/data-sys/grouping_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix): ../src/data-sys/message_area_dao.cpp $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-sys/message_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(DependSuffix): ../src/data-sys/message_area_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(DependSuffix) -MM ../src/data-sys/message_area_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/message_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(PreprocessSuffix) ../src/data-sys/message_area_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(ObjectSuffix): ../src/data-sys/network_dao.cpp $(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-sys/network_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(DependSuffix): ../src/data-sys/network_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(DependSuffix) -MM ../src/data-sys/network_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(PreprocessSuffix): ../src/data-sys/network_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(PreprocessSuffix) ../src/data-sys/network_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix): ../src/data-sys/protocol_dao.cpp $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-sys/protocol_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(DependSuffix): ../src/data-sys/protocol_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(DependSuffix) -MM ../src/data-sys/protocol_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(PreprocessSuffix): ../src/data-sys/protocol_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(PreprocessSuffix) ../src/data-sys/protocol_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix): ../src/data-sys/access_level_dao.cpp $(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-sys/access_level_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(DependSuffix): ../src/data-sys/access_level_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(DependSuffix) -MM ../src/data-sys/access_level_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(PreprocessSuffix): ../src/data-sys/access_level_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(PreprocessSuffix) ../src/data-sys/access_level_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix): ../src/data-sys/db_startup.cpp $(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-sys/db_startup.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(DependSuffix): ../src/data-sys/db_startup.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(DependSuffix) -MM ../src/data-sys/db_startup.cpp

$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(PreprocessSuffix): ../src/data-sys/db_startup.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(PreprocessSuffix) ../src/data-sys/db_startup.cpp

$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(ObjectSuffix): ../src/mods/mod_logon.cpp $(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/mods/mod_logon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(DependSuffix): ../src/mods/mod_logon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(DependSuffix) -MM ../src/mods/mod_logon.cpp

$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(PreprocessSuffix): ../src/mods/mod_logon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(PreprocessSuffix) ../src/mods/mod_logon.cpp

$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(ObjectSuffix): ../src/mods/mod_prelogon.cpp $(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/mods/mod_prelogon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(DependSuffix): ../src/mods/mod_prelogon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(DependSuffix) -MM ../src/mods/mod_prelogon.cpp

$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(PreprocessSuffix): ../src/mods/mod_prelogon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(PreprocessSuffix) ../src/mods/mod_prelogon.cpp

$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(ObjectSuffix): ../src/mods/mod_signup.cpp $(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/mods/mod_signup.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(DependSuffix): ../src/mods/mod_signup.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(DependSuffix) -MM ../src/mods/mod_signup.cpp

$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(PreprocessSuffix): ../src/mods/mod_signup.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(PreprocessSuffix) ../src/mods/mod_signup.cpp

$(IntermediateDirectory)/up_src_mods_mod_sys_config.cpp$(ObjectSuffix): ../src/mods/mod_sys_config.cpp $(IntermediateDirectory)/up_src_mods_mod_sys_config.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/mods/mod_sys_config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_sys_config.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_sys_config.cpp$(DependSuffix): ../src/mods/mod_sys_config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_sys_config.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_sys_config.cpp$(DependSuffix) -MM ../src/mods/mod_sys_config.cpp

$(IntermediateDirectory)/up_src_mods_mod_sys_config.cpp$(PreprocessSuffix): ../src/mods/mod_sys_config.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_sys_config.cpp$(PreprocessSuffix) ../src/mods/mod_sys_config.cpp

$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix): ../src/forms/form_system_config.cpp $(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/forms/form_system_config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(DependSuffix): ../src/forms/form_system_config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(DependSuffix) -MM ../src/forms/form_system_config.cpp

$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(PreprocessSuffix): ../src/forms/form_system_config.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(PreprocessSuffix) ../src/forms/form_system_config.cpp

$(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(ObjectSuffix): ../src/data-app/bbs_list_dao.cpp $(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-app/bbs_list_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(DependSuffix): ../src/data-app/bbs_list_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(DependSuffix) -MM ../src/data-app/bbs_list_dao.cpp

$(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(PreprocessSuffix): ../src/data-app/bbs_list_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(PreprocessSuffix) ../src/data-app/bbs_list_dao.cpp

$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix): ../src/data-app/oneliners_dao.cpp $(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/blue/code/Oblivion2-XRM/src/data-app/oneliners_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(DependSuffix): ../src/data-app/oneliners_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(DependSuffix) -MM ../src/data-app/oneliners_dao.cpp

$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(PreprocessSuffix): ../src/data-app/oneliners_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(PreprocessSuffix) ../src/data-app/oneliners_dao.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


