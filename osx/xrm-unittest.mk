##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=xrm-unittest
ConfigurationName      :=Debug
WorkspacePath          :=/Users/admin/code/Oblivion2-XRM/osx
ProjectPath            :=/Users/admin/code/Oblivion2-XRM/osx
IntermediateDirectory  :=Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Admin
Date                   :=27/07/2019
CodeLitePath           :="/Users/admin/Library/Application Support/codelite"
LinkerName             :=clang++
SharedObjectLinkerName :=clang++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)TARGET_OS_MAC $(PreprocessorSwitch)WITHOUT_SDL $(PreprocessorSwitch)UNIT_TEST 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="xrm-unittest.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../src $(IncludeSwitch)../sqlitewrap $(IncludeSwitch)./Debug $(IncludeSwitch)/usr/local/opt/sqlite/include $(IncludeSwitch)/usr/local/opt/yaml-cpp/include $(IncludeSwitch)/usr/local/opt/openssl/include $(IncludeSwitch)/usr/local/opt/unittest-cpp/include/UnitTest++ $(IncludeSwitch)/usr/local/opt/libssh/include $(IncludeSwitch)/usr/local/opt/ossp-uuid/include $(IncludeSwitch)../src/sdl2_net $(IncludeSwitch)../src/utf-cpp 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)c++ $(LibrarySwitch)c++abi $(LibrarySwitch)m $(LibrarySwitch)c $(LibrarySwitch)UnitTest++ $(LibrarySwitch)SqliteWrapped $(LibrarySwitch)sqlite3 $(LibrarySwitch)pthread $(LibrarySwitch)ssl $(LibrarySwitch)crypto $(LibrarySwitch)yaml-cpp $(LibrarySwitch)ssh $(LibrarySwitch)uuid 
ArLibs                 :=  "c++" "c++abi" "m" "c" "libUnitTest++.a" "libSqliteWrapped.a" "sqlite3" "pthread" "ssl" "crypto" "yaml-cpp" "libssh" "uuid" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)../src $(LibraryPathSwitch)../sqlitewrap $(LibraryPathSwitch)./Debug $(LibraryPathSwitch)/usr/local/opt/sqlite/lib $(LibraryPathSwitch)/usr/local/opt/yaml-cpp/lib $(LibraryPathSwitch)/usr/local/opt/openssl/lib $(LibraryPathSwitch)/usr/local/opt/unittest-cpp/lib $(LibraryPathSwitch)/usr/local/opt/libssh/lib $(LibraryPathSwitch)/usr/local/opt/ossp-uuid/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := clang++
CC       := clang
CXXFLAGS :=  -O0 -std=c++11 -stdlib=libc++ $(Preprocessors)
CFLAGS   :=  -O0 -std=c++11 -stdlib=libc++ $(Preprocessors)
ASFLAGS  := 
AS       := llvm-as


##
## User defined environment variables
##
CodeLiteDir:=/Applications/codelite.app/Contents/SharedSupport/
Objects0=$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_socket_telnet.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_socket_ssh.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_socket_handler.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_io_service.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_ansi_processor_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_users_dao_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d Debug || $(MakeDirCommand) Debug


$(IntermediateDirectory)/.d:
	@test -d Debug || $(MakeDirCommand) Debug

PreBuild:
	@echo Executing Pre Build commands ...
	
	sed -i '' 's+/Users/admin/code/Oblivion2-XRM/+../+' xrm-unittest.mk
	@echo Done


##
## Objects
##
$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnetUDP.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/sdl2_net/SDLnetUDP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnetUDP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(PreprocessSuffix) ../src/sdl2_net/SDLnetUDP.cpp

$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix): ../src/data-app/oneliners_dao.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/data-app/oneliners_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(PreprocessSuffix): ../src/data-app/oneliners_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(PreprocessSuffix) ../src/data-app/oneliners_dao.cpp

$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix): ../src/forms/form_system_config.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/forms/form_system_config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(PreprocessSuffix): ../src/forms/form_system_config.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(PreprocessSuffix) ../src/forms/form_system_config.cpp

$(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnet.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/sdl2_net/SDLnet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(PreprocessSuffix) ../src/sdl2_net/SDLnet.cpp

$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix): ../src/data-sys/text_prompts_dao.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/data-sys/text_prompts_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(PreprocessSuffix): ../src/data-sys/text_prompts_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(PreprocessSuffix) ../src/data-sys/text_prompts_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix): ../src/data-sys/session_stats_dao.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/data-sys/session_stats_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(PreprocessSuffix): ../src/data-sys/session_stats_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(PreprocessSuffix) ../src/data-sys/session_stats_dao.cpp

$(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnetselect.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/sdl2_net/SDLnetselect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnetselect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(PreprocessSuffix) ../src/sdl2_net/SDLnetselect.cpp

$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix): ../src/data-sys/security_dao.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/data-sys/security_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(PreprocessSuffix): ../src/data-sys/security_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(PreprocessSuffix) ../src/data-sys/security_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix): ../src/data-sys/access_level_dao.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/data-sys/access_level_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(PreprocessSuffix): ../src/data-sys/access_level_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(PreprocessSuffix) ../src/data-sys/access_level_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix): ../src/data-sys/message_area_dao.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/data-sys/message_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/message_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(PreprocessSuffix) ../src/data-sys/message_area_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix): ../src/data-sys/file_area_dao.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/data-sys/file_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/file_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(PreprocessSuffix) ../src/data-sys/file_area_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix): ../src/data-sys/menu_dao.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/data-sys/menu_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(PreprocessSuffix): ../src/data-sys/menu_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(PreprocessSuffix) ../src/data-sys/menu_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix): ../src/data-sys/users_dao.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/data-sys/users_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(PreprocessSuffix): ../src/data-sys/users_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(PreprocessSuffix) ../src/data-sys/users_dao.cpp

$(IntermediateDirectory)/up_src_socket_telnet.cpp$(ObjectSuffix): ../src/socket_telnet.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/socket_telnet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_socket_telnet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_socket_telnet.cpp$(PreprocessSuffix): ../src/socket_telnet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_socket_telnet.cpp$(PreprocessSuffix) ../src/socket_telnet.cpp

$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix): ../src/data-sys/conference_dao.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/data-sys/conference_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(PreprocessSuffix): ../src/data-sys/conference_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(PreprocessSuffix) ../src/data-sys/conference_dao.cpp

$(IntermediateDirectory)/up_src_socket_ssh.cpp$(ObjectSuffix): ../src/socket_ssh.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/socket_ssh.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_socket_ssh.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_socket_ssh.cpp$(PreprocessSuffix): ../src/socket_ssh.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_socket_ssh.cpp$(PreprocessSuffix) ../src/socket_ssh.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/file_area_dao_it.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/xrm-unittest/src/file_area_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/file_area_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(PreprocessSuffix) ../xrm-unittest/src/file_area_dao_it.cpp

$(IntermediateDirectory)/up_src_socket_handler.cpp$(ObjectSuffix): ../src/socket_handler.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/socket_handler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_socket_handler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_socket_handler.cpp$(PreprocessSuffix): ../src/socket_handler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_socket_handler.cpp$(PreprocessSuffix) ../src/socket_handler.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/access_condition_ut.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/xrm-unittest/src/access_condition_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/access_condition_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/access_condition_ut.cpp

$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix): ../src/session_io.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/session_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_session_io.cpp$(PreprocessSuffix): ../src/session_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_session_io.cpp$(PreprocessSuffix) ../src/session_io.cpp

$(IntermediateDirectory)/up_src_io_service.cpp$(ObjectSuffix): ../src/io_service.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/io_service.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_io_service.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_io_service.cpp$(PreprocessSuffix): ../src/io_service.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_io_service.cpp$(PreprocessSuffix) ../src/io_service.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/form_system_config_ut.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/xrm-unittest/src/form_system_config_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/form_system_config_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/form_system_config_ut.cpp

$(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix): ../src/form_manager.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/form_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_form_manager.cpp$(PreprocessSuffix): ../src/form_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_form_manager.cpp$(PreprocessSuffix) ../src/form_manager.cpp

$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnetTCP.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/sdl2_net/SDLnetTCP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnetTCP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(PreprocessSuffix) ../src/sdl2_net/SDLnetTCP.cpp

$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix): ../src/access_condition.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/access_condition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_access_condition.cpp$(PreprocessSuffix): ../src/access_condition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_access_condition.cpp$(PreprocessSuffix) ../src/access_condition.cpp

$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix): ../src/session_manager.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/session_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_session_manager.cpp$(PreprocessSuffix): ../src/session_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_session_manager.cpp$(PreprocessSuffix) ../src/session_manager.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/session_io_ut.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/xrm-unittest/src/session_io_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/session_io_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/session_io_ut.cpp

$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix): ../src/communicator.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/communicator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_communicator.cpp$(PreprocessSuffix): ../src/communicator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_communicator.cpp$(PreprocessSuffix) ../src/communicator.cpp

$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix): ../src/data-sys/protocol_dao.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/data-sys/protocol_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(PreprocessSuffix): ../src/data-sys/protocol_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(PreprocessSuffix) ../src/data-sys/protocol_dao.cpp

$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix): ../src/ansi_processor.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/ansi_processor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ansi_processor.cpp$(PreprocessSuffix): ../src/ansi_processor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ansi_processor.cpp$(PreprocessSuffix) ../src/ansi_processor.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(ObjectSuffix): ../xrm-unittest/src/xrm_test.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/xrm-unittest/src/xrm_test.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(PreprocessSuffix): ../xrm-unittest/src/xrm_test.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(PreprocessSuffix) ../xrm-unittest/src/xrm_test.cpp

$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix): ../src/data-sys/config_dao.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/data-sys/config_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix): ../src/data-sys/config_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix) ../src/data-sys/config_dao.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/common_mock_ut.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/xrm-unittest/src/common_mock_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/common_mock_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/common_mock_ut.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/conference_dao_it.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/xrm-unittest/src/conference_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/conference_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(PreprocessSuffix) ../xrm-unittest/src/conference_dao_it.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_ansi_processor_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/ansi_processor_ut.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/xrm-unittest/src/ansi_processor_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_ansi_processor_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_ansi_processor_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/ansi_processor_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_ansi_processor_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/ansi_processor_ut.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_users_dao_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/users_dao_ut.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/xrm-unittest/src/users_dao_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_users_dao_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_users_dao_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/users_dao_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_users_dao_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/users_dao_ut.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/common_io_ut.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/xrm-unittest/src/common_io_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/common_io_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/common_io_ut.cpp

$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix): ../src/common_io.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/common_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix): ../src/common_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix) ../src/common_io.cpp

$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix): ../src/encoding.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/encoding.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_encoding.cpp$(PreprocessSuffix): ../src/encoding.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_encoding.cpp$(PreprocessSuffix) ../src/encoding.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/message_area_dao_it.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/xrm-unittest/src/message_area_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/message_area_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(PreprocessSuffix) ../xrm-unittest/src/message_area_dao_it.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/oneliners_dao_it.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/xrm-unittest/src/oneliners_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/oneliners_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(PreprocessSuffix) ../xrm-unittest/src/oneliners_dao_it.cpp

##
## Clean
##
clean:
	$(RM) -r Debug/


