##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=xrm-unittest
ConfigurationName      :=Debug
WorkspacePath          := "../linux"
ProjectPath            := "../linux"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Michae Griffin
Date                   :=26/07/19
CodeLitePath           :="/home/merc/.codelite"
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
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../src $(IncludeSwitch)../sqlitewrap $(IncludeSwitch)../linux/Debug $(IncludeSwitch)/usr/include/unittest++ $(IncludeSwitch)/usr/include/UnitTest++ $(IncludeSwitch)../src/sdl2_net $(IncludeSwitch)../src/utf-cpp $(IncludeSwitch)../src/utf8 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)UnitTest++ $(LibrarySwitch)SqliteWrapped $(LibrarySwitch)sqlite3 $(LibrarySwitch)pthread $(LibrarySwitch)ssl $(LibrarySwitch)crypto $(LibrarySwitch)yaml-cpp $(LibrarySwitch)ssh $(LibrarySwitch)uuid 
ArLibs                 :=  "libUnitTest++.a" "libSqliteWrapped.a" "sqlite3" "pthread" "ssl" "crypto" "yaml-cpp" "libssh" "uuid" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)$(UNIT_TEST_PP_SRC_DIR)/Debug $(LibraryPathSwitch)../linux/Debug $(LibraryPathSwitch)../sqlitewrap $(LibraryPathSwitch)../src 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -std=c++11 -Wno-psabi $(Preprocessors)
CFLAGS   :=  -g -std=c++11 -Wno-psabi $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_ansi_processor.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_session_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_session_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_access_condition.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_form_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_io_service.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_communicator.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_socket_handler.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_socket_ssh.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_socket_telnet.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_encoding.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_access_condition_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_ansi_processor_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_common_io_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_common_mock_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_conference_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_file_area_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_form_system_config_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_message_area_dao_it.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_oneliners_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_session_io_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_xrm_test.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_config_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_conference_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_menu_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_file_area_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_message_area_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_access_level_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_protocol_dao.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/data-sys_security_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_users_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/forms_form_system_config.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-app_oneliners_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/sdl2_net_SDLnet.cpp$(ObjectSuffix) $(IntermediateDirectory)/sdl2_net_SDLnetselect.cpp$(ObjectSuffix) $(IntermediateDirectory)/sdl2_net_SDLnetTCP.cpp$(ObjectSuffix) $(IntermediateDirectory)/sdl2_net_SDLnetUDP.cpp$(ObjectSuffix) \
	



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
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:
	@echo Executing Pre Build commands ...
	sed -i 's+../+../+' xrm-unittest.mk
	@echo Done


##
## Objects
##
$(IntermediateDirectory)/src_ansi_processor.cpp$(ObjectSuffix): ../src/ansi_processor.cpp $(IntermediateDirectory)/src_ansi_processor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/ansi_processor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ansi_processor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ansi_processor.cpp$(DependSuffix): ../src/ansi_processor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_ansi_processor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_ansi_processor.cpp$(DependSuffix) -MM "../src/ansi_processor.cpp"

$(IntermediateDirectory)/src_ansi_processor.cpp$(PreprocessSuffix): ../src/ansi_processor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ansi_processor.cpp$(PreprocessSuffix) "../src/ansi_processor.cpp"

$(IntermediateDirectory)/src_session_manager.cpp$(ObjectSuffix): ../src/session_manager.cpp $(IntermediateDirectory)/src_session_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/session_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_session_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_session_manager.cpp$(DependSuffix): ../src/session_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_session_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_session_manager.cpp$(DependSuffix) -MM "../src/session_manager.cpp"

$(IntermediateDirectory)/src_session_manager.cpp$(PreprocessSuffix): ../src/session_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_session_manager.cpp$(PreprocessSuffix) "../src/session_manager.cpp"

$(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix): ../src/common_io.cpp $(IntermediateDirectory)/src_common_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/common_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_common_io.cpp$(DependSuffix): ../src/common_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_common_io.cpp$(DependSuffix) -MM "../src/common_io.cpp"

$(IntermediateDirectory)/src_common_io.cpp$(PreprocessSuffix): ../src/common_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_common_io.cpp$(PreprocessSuffix) "../src/common_io.cpp"

$(IntermediateDirectory)/src_session_io.cpp$(ObjectSuffix): ../src/session_io.cpp $(IntermediateDirectory)/src_session_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/session_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_session_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_session_io.cpp$(DependSuffix): ../src/session_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_session_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_session_io.cpp$(DependSuffix) -MM "../src/session_io.cpp"

$(IntermediateDirectory)/src_session_io.cpp$(PreprocessSuffix): ../src/session_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_session_io.cpp$(PreprocessSuffix) "../src/session_io.cpp"

$(IntermediateDirectory)/src_access_condition.cpp$(ObjectSuffix): ../src/access_condition.cpp $(IntermediateDirectory)/src_access_condition.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/access_condition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_access_condition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_access_condition.cpp$(DependSuffix): ../src/access_condition.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_access_condition.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_access_condition.cpp$(DependSuffix) -MM "../src/access_condition.cpp"

$(IntermediateDirectory)/src_access_condition.cpp$(PreprocessSuffix): ../src/access_condition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_access_condition.cpp$(PreprocessSuffix) "../src/access_condition.cpp"

$(IntermediateDirectory)/src_form_manager.cpp$(ObjectSuffix): ../src/form_manager.cpp $(IntermediateDirectory)/src_form_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/form_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_form_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_form_manager.cpp$(DependSuffix): ../src/form_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_form_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_form_manager.cpp$(DependSuffix) -MM "../src/form_manager.cpp"

$(IntermediateDirectory)/src_form_manager.cpp$(PreprocessSuffix): ../src/form_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_form_manager.cpp$(PreprocessSuffix) "../src/form_manager.cpp"

$(IntermediateDirectory)/src_io_service.cpp$(ObjectSuffix): ../src/io_service.cpp $(IntermediateDirectory)/src_io_service.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/io_service.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_io_service.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_io_service.cpp$(DependSuffix): ../src/io_service.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_io_service.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_io_service.cpp$(DependSuffix) -MM "../src/io_service.cpp"

$(IntermediateDirectory)/src_io_service.cpp$(PreprocessSuffix): ../src/io_service.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_io_service.cpp$(PreprocessSuffix) "../src/io_service.cpp"

$(IntermediateDirectory)/src_communicator.cpp$(ObjectSuffix): ../src/communicator.cpp $(IntermediateDirectory)/src_communicator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/communicator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_communicator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_communicator.cpp$(DependSuffix): ../src/communicator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_communicator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_communicator.cpp$(DependSuffix) -MM "../src/communicator.cpp"

$(IntermediateDirectory)/src_communicator.cpp$(PreprocessSuffix): ../src/communicator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_communicator.cpp$(PreprocessSuffix) "../src/communicator.cpp"

$(IntermediateDirectory)/src_socket_handler.cpp$(ObjectSuffix): ../src/socket_handler.cpp $(IntermediateDirectory)/src_socket_handler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/socket_handler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_socket_handler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_socket_handler.cpp$(DependSuffix): ../src/socket_handler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_socket_handler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_socket_handler.cpp$(DependSuffix) -MM "../src/socket_handler.cpp"

$(IntermediateDirectory)/src_socket_handler.cpp$(PreprocessSuffix): ../src/socket_handler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_socket_handler.cpp$(PreprocessSuffix) "../src/socket_handler.cpp"

$(IntermediateDirectory)/src_socket_ssh.cpp$(ObjectSuffix): ../src/socket_ssh.cpp $(IntermediateDirectory)/src_socket_ssh.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/socket_ssh.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_socket_ssh.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_socket_ssh.cpp$(DependSuffix): ../src/socket_ssh.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_socket_ssh.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_socket_ssh.cpp$(DependSuffix) -MM "../src/socket_ssh.cpp"

$(IntermediateDirectory)/src_socket_ssh.cpp$(PreprocessSuffix): ../src/socket_ssh.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_socket_ssh.cpp$(PreprocessSuffix) "../src/socket_ssh.cpp"

$(IntermediateDirectory)/src_socket_telnet.cpp$(ObjectSuffix): ../src/socket_telnet.cpp $(IntermediateDirectory)/src_socket_telnet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/socket_telnet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_socket_telnet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_socket_telnet.cpp$(DependSuffix): ../src/socket_telnet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_socket_telnet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_socket_telnet.cpp$(DependSuffix) -MM "../src/socket_telnet.cpp"

$(IntermediateDirectory)/src_socket_telnet.cpp$(PreprocessSuffix): ../src/socket_telnet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_socket_telnet.cpp$(PreprocessSuffix) "../src/socket_telnet.cpp"

$(IntermediateDirectory)/src_encoding.cpp$(ObjectSuffix): ../src/encoding.cpp $(IntermediateDirectory)/src_encoding.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/encoding.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_encoding.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_encoding.cpp$(DependSuffix): ../src/encoding.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_encoding.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_encoding.cpp$(DependSuffix) -MM "../src/encoding.cpp"

$(IntermediateDirectory)/src_encoding.cpp$(PreprocessSuffix): ../src/encoding.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_encoding.cpp$(PreprocessSuffix) "../src/encoding.cpp"

$(IntermediateDirectory)/src_access_condition_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/access_condition_ut.cpp $(IntermediateDirectory)/src_access_condition_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/access_condition_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_access_condition_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_access_condition_ut.cpp$(DependSuffix): ../xrm-unittest/src/access_condition_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_access_condition_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_access_condition_ut.cpp$(DependSuffix) -MM "../xrm-unittest/src/access_condition_ut.cpp"

$(IntermediateDirectory)/src_access_condition_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/access_condition_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_access_condition_ut.cpp$(PreprocessSuffix) "../xrm-unittest/src/access_condition_ut.cpp"

$(IntermediateDirectory)/src_ansi_processor_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/ansi_processor_ut.cpp $(IntermediateDirectory)/src_ansi_processor_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/ansi_processor_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ansi_processor_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ansi_processor_ut.cpp$(DependSuffix): ../xrm-unittest/src/ansi_processor_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_ansi_processor_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_ansi_processor_ut.cpp$(DependSuffix) -MM "../xrm-unittest/src/ansi_processor_ut.cpp"

$(IntermediateDirectory)/src_ansi_processor_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/ansi_processor_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ansi_processor_ut.cpp$(PreprocessSuffix) "../xrm-unittest/src/ansi_processor_ut.cpp"

$(IntermediateDirectory)/src_common_io_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/common_io_ut.cpp $(IntermediateDirectory)/src_common_io_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/common_io_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_common_io_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_common_io_ut.cpp$(DependSuffix): ../xrm-unittest/src/common_io_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_common_io_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_common_io_ut.cpp$(DependSuffix) -MM "../xrm-unittest/src/common_io_ut.cpp"

$(IntermediateDirectory)/src_common_io_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/common_io_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_common_io_ut.cpp$(PreprocessSuffix) "../xrm-unittest/src/common_io_ut.cpp"

$(IntermediateDirectory)/src_common_mock_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/common_mock_ut.cpp $(IntermediateDirectory)/src_common_mock_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/common_mock_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_common_mock_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_common_mock_ut.cpp$(DependSuffix): ../xrm-unittest/src/common_mock_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_common_mock_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_common_mock_ut.cpp$(DependSuffix) -MM "../xrm-unittest/src/common_mock_ut.cpp"

$(IntermediateDirectory)/src_common_mock_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/common_mock_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_common_mock_ut.cpp$(PreprocessSuffix) "../xrm-unittest/src/common_mock_ut.cpp"

$(IntermediateDirectory)/src_conference_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/conference_dao_it.cpp $(IntermediateDirectory)/src_conference_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/conference_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_conference_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_conference_dao_it.cpp$(DependSuffix): ../xrm-unittest/src/conference_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_conference_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_conference_dao_it.cpp$(DependSuffix) -MM "../xrm-unittest/src/conference_dao_it.cpp"

$(IntermediateDirectory)/src_conference_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/conference_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_conference_dao_it.cpp$(PreprocessSuffix) "../xrm-unittest/src/conference_dao_it.cpp"

$(IntermediateDirectory)/src_file_area_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/file_area_dao_it.cpp $(IntermediateDirectory)/src_file_area_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/file_area_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_file_area_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_file_area_dao_it.cpp$(DependSuffix): ../xrm-unittest/src/file_area_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_file_area_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_file_area_dao_it.cpp$(DependSuffix) -MM "../xrm-unittest/src/file_area_dao_it.cpp"

$(IntermediateDirectory)/src_file_area_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/file_area_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_file_area_dao_it.cpp$(PreprocessSuffix) "../xrm-unittest/src/file_area_dao_it.cpp"

$(IntermediateDirectory)/src_form_system_config_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/form_system_config_ut.cpp $(IntermediateDirectory)/src_form_system_config_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/form_system_config_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_form_system_config_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_form_system_config_ut.cpp$(DependSuffix): ../xrm-unittest/src/form_system_config_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_form_system_config_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_form_system_config_ut.cpp$(DependSuffix) -MM "../xrm-unittest/src/form_system_config_ut.cpp"

$(IntermediateDirectory)/src_form_system_config_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/form_system_config_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_form_system_config_ut.cpp$(PreprocessSuffix) "../xrm-unittest/src/form_system_config_ut.cpp"

$(IntermediateDirectory)/src_message_area_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/message_area_dao_it.cpp $(IntermediateDirectory)/src_message_area_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/message_area_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_message_area_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_message_area_dao_it.cpp$(DependSuffix): ../xrm-unittest/src/message_area_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_message_area_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_message_area_dao_it.cpp$(DependSuffix) -MM "../xrm-unittest/src/message_area_dao_it.cpp"

$(IntermediateDirectory)/src_message_area_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/message_area_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_message_area_dao_it.cpp$(PreprocessSuffix) "../xrm-unittest/src/message_area_dao_it.cpp"

$(IntermediateDirectory)/src_oneliners_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/oneliners_dao_it.cpp $(IntermediateDirectory)/src_oneliners_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/oneliners_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_oneliners_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_oneliners_dao_it.cpp$(DependSuffix): ../xrm-unittest/src/oneliners_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_oneliners_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_oneliners_dao_it.cpp$(DependSuffix) -MM "../xrm-unittest/src/oneliners_dao_it.cpp"

$(IntermediateDirectory)/src_oneliners_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/oneliners_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_oneliners_dao_it.cpp$(PreprocessSuffix) "../xrm-unittest/src/oneliners_dao_it.cpp"

$(IntermediateDirectory)/src_session_io_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/session_io_ut.cpp $(IntermediateDirectory)/src_session_io_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/session_io_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_session_io_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_session_io_ut.cpp$(DependSuffix): ../xrm-unittest/src/session_io_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_session_io_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_session_io_ut.cpp$(DependSuffix) -MM "../xrm-unittest/src/session_io_ut.cpp"

$(IntermediateDirectory)/src_session_io_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/session_io_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_session_io_ut.cpp$(PreprocessSuffix) "../xrm-unittest/src/session_io_ut.cpp"

$(IntermediateDirectory)/src_xrm_test.cpp$(ObjectSuffix): ../xrm-unittest/src/xrm_test.cpp $(IntermediateDirectory)/src_xrm_test.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/xrm_test.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_xrm_test.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_xrm_test.cpp$(DependSuffix): ../xrm-unittest/src/xrm_test.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_xrm_test.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_xrm_test.cpp$(DependSuffix) -MM "../xrm-unittest/src/xrm_test.cpp"

$(IntermediateDirectory)/src_xrm_test.cpp$(PreprocessSuffix): ../xrm-unittest/src/xrm_test.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_xrm_test.cpp$(PreprocessSuffix) "../xrm-unittest/src/xrm_test.cpp"

$(IntermediateDirectory)/data-sys_config_dao.cpp$(ObjectSuffix): ../src/data-sys/config_dao.cpp $(IntermediateDirectory)/data-sys_config_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/config_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_config_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_config_dao.cpp$(DependSuffix): ../src/data-sys/config_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_config_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_config_dao.cpp$(DependSuffix) -MM "../src/data-sys/config_dao.cpp"

$(IntermediateDirectory)/data-sys_config_dao.cpp$(PreprocessSuffix): ../src/data-sys/config_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_config_dao.cpp$(PreprocessSuffix) "../src/data-sys/config_dao.cpp"

$(IntermediateDirectory)/data-sys_conference_dao.cpp$(ObjectSuffix): ../src/data-sys/conference_dao.cpp $(IntermediateDirectory)/data-sys_conference_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/conference_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_conference_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_conference_dao.cpp$(DependSuffix): ../src/data-sys/conference_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_conference_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_conference_dao.cpp$(DependSuffix) -MM "../src/data-sys/conference_dao.cpp"

$(IntermediateDirectory)/data-sys_conference_dao.cpp$(PreprocessSuffix): ../src/data-sys/conference_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_conference_dao.cpp$(PreprocessSuffix) "../src/data-sys/conference_dao.cpp"

$(IntermediateDirectory)/data-sys_menu_dao.cpp$(ObjectSuffix): ../src/data-sys/menu_dao.cpp $(IntermediateDirectory)/data-sys_menu_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/menu_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_menu_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_menu_dao.cpp$(DependSuffix): ../src/data-sys/menu_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_menu_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_menu_dao.cpp$(DependSuffix) -MM "../src/data-sys/menu_dao.cpp"

$(IntermediateDirectory)/data-sys_menu_dao.cpp$(PreprocessSuffix): ../src/data-sys/menu_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_menu_dao.cpp$(PreprocessSuffix) "../src/data-sys/menu_dao.cpp"

$(IntermediateDirectory)/data-sys_file_area_dao.cpp$(ObjectSuffix): ../src/data-sys/file_area_dao.cpp $(IntermediateDirectory)/data-sys_file_area_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/file_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_file_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_file_area_dao.cpp$(DependSuffix): ../src/data-sys/file_area_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_file_area_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_file_area_dao.cpp$(DependSuffix) -MM "../src/data-sys/file_area_dao.cpp"

$(IntermediateDirectory)/data-sys_file_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/file_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_file_area_dao.cpp$(PreprocessSuffix) "../src/data-sys/file_area_dao.cpp"

$(IntermediateDirectory)/data-sys_message_area_dao.cpp$(ObjectSuffix): ../src/data-sys/message_area_dao.cpp $(IntermediateDirectory)/data-sys_message_area_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/message_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_message_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_message_area_dao.cpp$(DependSuffix): ../src/data-sys/message_area_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_message_area_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_message_area_dao.cpp$(DependSuffix) -MM "../src/data-sys/message_area_dao.cpp"

$(IntermediateDirectory)/data-sys_message_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/message_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_message_area_dao.cpp$(PreprocessSuffix) "../src/data-sys/message_area_dao.cpp"

$(IntermediateDirectory)/data-sys_access_level_dao.cpp$(ObjectSuffix): ../src/data-sys/access_level_dao.cpp $(IntermediateDirectory)/data-sys_access_level_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/access_level_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_access_level_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_access_level_dao.cpp$(DependSuffix): ../src/data-sys/access_level_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_access_level_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_access_level_dao.cpp$(DependSuffix) -MM "../src/data-sys/access_level_dao.cpp"

$(IntermediateDirectory)/data-sys_access_level_dao.cpp$(PreprocessSuffix): ../src/data-sys/access_level_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_access_level_dao.cpp$(PreprocessSuffix) "../src/data-sys/access_level_dao.cpp"

$(IntermediateDirectory)/data-sys_protocol_dao.cpp$(ObjectSuffix): ../src/data-sys/protocol_dao.cpp $(IntermediateDirectory)/data-sys_protocol_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/protocol_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_protocol_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_protocol_dao.cpp$(DependSuffix): ../src/data-sys/protocol_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_protocol_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_protocol_dao.cpp$(DependSuffix) -MM "../src/data-sys/protocol_dao.cpp"

$(IntermediateDirectory)/data-sys_protocol_dao.cpp$(PreprocessSuffix): ../src/data-sys/protocol_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_protocol_dao.cpp$(PreprocessSuffix) "../src/data-sys/protocol_dao.cpp"

$(IntermediateDirectory)/data-sys_security_dao.cpp$(ObjectSuffix): ../src/data-sys/security_dao.cpp $(IntermediateDirectory)/data-sys_security_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/security_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_security_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_security_dao.cpp$(DependSuffix): ../src/data-sys/security_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_security_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_security_dao.cpp$(DependSuffix) -MM "../src/data-sys/security_dao.cpp"

$(IntermediateDirectory)/data-sys_security_dao.cpp$(PreprocessSuffix): ../src/data-sys/security_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_security_dao.cpp$(PreprocessSuffix) "../src/data-sys/security_dao.cpp"

$(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(ObjectSuffix): ../src/data-sys/session_stats_dao.cpp $(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/session_stats_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(DependSuffix): ../src/data-sys/session_stats_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(DependSuffix) -MM "../src/data-sys/session_stats_dao.cpp"

$(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(PreprocessSuffix): ../src/data-sys/session_stats_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(PreprocessSuffix) "../src/data-sys/session_stats_dao.cpp"

$(IntermediateDirectory)/data-sys_users_dao.cpp$(ObjectSuffix): ../src/data-sys/users_dao.cpp $(IntermediateDirectory)/data-sys_users_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/users_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_users_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_users_dao.cpp$(DependSuffix): ../src/data-sys/users_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_users_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_users_dao.cpp$(DependSuffix) -MM "../src/data-sys/users_dao.cpp"

$(IntermediateDirectory)/data-sys_users_dao.cpp$(PreprocessSuffix): ../src/data-sys/users_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_users_dao.cpp$(PreprocessSuffix) "../src/data-sys/users_dao.cpp"

$(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(ObjectSuffix): ../src/data-sys/text_prompts_dao.cpp $(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/text_prompts_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(DependSuffix): ../src/data-sys/text_prompts_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(DependSuffix) -MM "../src/data-sys/text_prompts_dao.cpp"

$(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(PreprocessSuffix): ../src/data-sys/text_prompts_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(PreprocessSuffix) "../src/data-sys/text_prompts_dao.cpp"

$(IntermediateDirectory)/forms_form_system_config.cpp$(ObjectSuffix): ../src/forms/form_system_config.cpp $(IntermediateDirectory)/forms_form_system_config.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/forms/form_system_config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/forms_form_system_config.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/forms_form_system_config.cpp$(DependSuffix): ../src/forms/form_system_config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/forms_form_system_config.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/forms_form_system_config.cpp$(DependSuffix) -MM "../src/forms/form_system_config.cpp"

$(IntermediateDirectory)/forms_form_system_config.cpp$(PreprocessSuffix): ../src/forms/form_system_config.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/forms_form_system_config.cpp$(PreprocessSuffix) "../src/forms/form_system_config.cpp"

$(IntermediateDirectory)/data-app_oneliners_dao.cpp$(ObjectSuffix): ../src/data-app/oneliners_dao.cpp $(IntermediateDirectory)/data-app_oneliners_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-app/oneliners_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-app_oneliners_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-app_oneliners_dao.cpp$(DependSuffix): ../src/data-app/oneliners_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-app_oneliners_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-app_oneliners_dao.cpp$(DependSuffix) -MM "../src/data-app/oneliners_dao.cpp"

$(IntermediateDirectory)/data-app_oneliners_dao.cpp$(PreprocessSuffix): ../src/data-app/oneliners_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-app_oneliners_dao.cpp$(PreprocessSuffix) "../src/data-app/oneliners_dao.cpp"

$(IntermediateDirectory)/sdl2_net_SDLnet.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnet.cpp $(IntermediateDirectory)/sdl2_net_SDLnet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/sdl2_net/SDLnet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdl2_net_SDLnet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdl2_net_SDLnet.cpp$(DependSuffix): ../src/sdl2_net/SDLnet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sdl2_net_SDLnet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/sdl2_net_SDLnet.cpp$(DependSuffix) -MM "../src/sdl2_net/SDLnet.cpp"

$(IntermediateDirectory)/sdl2_net_SDLnet.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdl2_net_SDLnet.cpp$(PreprocessSuffix) "../src/sdl2_net/SDLnet.cpp"

$(IntermediateDirectory)/sdl2_net_SDLnetselect.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnetselect.cpp $(IntermediateDirectory)/sdl2_net_SDLnetselect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/sdl2_net/SDLnetselect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdl2_net_SDLnetselect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdl2_net_SDLnetselect.cpp$(DependSuffix): ../src/sdl2_net/SDLnetselect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sdl2_net_SDLnetselect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/sdl2_net_SDLnetselect.cpp$(DependSuffix) -MM "../src/sdl2_net/SDLnetselect.cpp"

$(IntermediateDirectory)/sdl2_net_SDLnetselect.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnetselect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdl2_net_SDLnetselect.cpp$(PreprocessSuffix) "../src/sdl2_net/SDLnetselect.cpp"

$(IntermediateDirectory)/sdl2_net_SDLnetTCP.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnetTCP.cpp $(IntermediateDirectory)/sdl2_net_SDLnetTCP.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/sdl2_net/SDLnetTCP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdl2_net_SDLnetTCP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdl2_net_SDLnetTCP.cpp$(DependSuffix): ../src/sdl2_net/SDLnetTCP.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sdl2_net_SDLnetTCP.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/sdl2_net_SDLnetTCP.cpp$(DependSuffix) -MM "../src/sdl2_net/SDLnetTCP.cpp"

$(IntermediateDirectory)/sdl2_net_SDLnetTCP.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnetTCP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdl2_net_SDLnetTCP.cpp$(PreprocessSuffix) "../src/sdl2_net/SDLnetTCP.cpp"

$(IntermediateDirectory)/sdl2_net_SDLnetUDP.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnetUDP.cpp $(IntermediateDirectory)/sdl2_net_SDLnetUDP.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/sdl2_net/SDLnetUDP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdl2_net_SDLnetUDP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdl2_net_SDLnetUDP.cpp$(DependSuffix): ../src/sdl2_net/SDLnetUDP.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sdl2_net_SDLnetUDP.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/sdl2_net_SDLnetUDP.cpp$(DependSuffix) -MM "../src/sdl2_net/SDLnetUDP.cpp"

$(IntermediateDirectory)/sdl2_net_SDLnetUDP.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnetUDP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdl2_net_SDLnetUDP.cpp$(PreprocessSuffix) "../src/sdl2_net/SDLnetUDP.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


