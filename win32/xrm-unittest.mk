##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=xrm-unittest
ConfigurationName      :=Debug
WorkspacePath          :=../win32
ProjectPath            :=../win32
IntermediateDirectory  :=Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Blue
Date                   :=27/07/2019
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/MinGW-5.1.0/mingw32/bin/g++.exe
SharedObjectLinkerName :=C:/MinGW-5.1.0/mingw32/bin/g++.exe -shared -fPIC
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
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-5.1.0/mingw32/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../win32/Debug $(IncludeSwitch)../src $(IncludeSwitch)../sqlitewrap $(IncludeSwitch)C:/UnitTest++-1.3/src $(IncludeSwitch)C:/sqlite3/include $(IncludeSwitch)C:/Users/Blue/Desktop/Test2/sqlitewrap $(IncludeSwitch)C:/OpenSSL-Win32/include $(IncludeSwitch)C:/Users/Blue/Documents/GitHub/yaml-cpp/include $(IncludeSwitch)C:/libssh-0.7.2/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)mingw32 $(LibrarySwitch)ws2_32 $(LibrarySwitch)wsock32 $(LibrarySwitch)ssleay32 $(LibrarySwitch)eay32 $(LibrarySwitch)ssh $(LibrarySwitch)iphlpapi $(LibrarySwitch)SqliteWrapped $(LibrarySwitch)sqlite3 $(LibrarySwitch)yaml-cpp $(LibrarySwitch)UnitTest++ $(LibrarySwitch)Rpcrt4 
ArLibs                 :=  "mingw32" "libws2_32.a" "libwsock32.a" "ssleay32.a" "libeay32.a" "libssh" "libiphlpapi.a" "libSqliteWrapped.a" "libsqlite3.a" "yaml-cpp.dll" "UnitTest++" "Rpcrt4" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)../win32/Debug $(LibraryPathSwitch)../src $(LibraryPathSwitch)../sqlitewrap/Debug $(LibraryPathSwitch)C:/sqlite3/lib $(LibraryPathSwitch)mingw32/lib $(LibraryPathSwitch)C:/UnitTest++-1.3/Debug $(LibraryPathSwitch)C:/OpenSSL-Win32/lib/MinGW $(LibraryPathSwitch)C:/OpenSSL-Win32/lib/ $(LibraryPathSwitch)C:/Users/Blue/Documents/GitHub/yaml-cpp/build $(LibraryPathSwitch)C:/libssh-0.7.2\lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW-5.1.0/mingw32/bin/ar.exe rcu
CXX      := C:/MinGW-5.1.0/mingw32/bin/g++.exe
CC       := C:/MinGW-5.1.0/mingw32/bin/gcc.exe
CXXFLAGS :=  -Wfatal-errors -g -O0 -pedantic -W -std=c++11 -Wall $(Preprocessors)
CFLAGS   :=  -Wfatal-errors -g -O0 -pedantic -W -std=c++11 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW-5.1.0/mingw32/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_socket_telnet.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_socket_handler.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_socket_ssh.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_users_dao_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_xrm-unittest_src_ansi_processor_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_io_service.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "Debug"

PreBuild:
	@echo Executing Pre Build commands ...
	sed -i 's+../+../+' xrm-unittest.mk
	@echo Done


##
## Objects
##
$(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnet.cpp $(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/sdl2_net/SDLnet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(DependSuffix): ../src/sdl2_net/SDLnet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(DependSuffix) -MM ../src/sdl2_net/SDLnet.cpp

$(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(PreprocessSuffix) ../src/sdl2_net/SDLnet.cpp

$(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnetselect.cpp $(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/sdl2_net/SDLnetselect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(DependSuffix): ../src/sdl2_net/SDLnetselect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(DependSuffix) -MM ../src/sdl2_net/SDLnetselect.cpp

$(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnetselect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(PreprocessSuffix) ../src/sdl2_net/SDLnetselect.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/form_system_config_ut.cpp $(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/form_system_config_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(DependSuffix): ../xrm-unittest/src/form_system_config_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(DependSuffix) -MM ../xrm-unittest/src/form_system_config_ut.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/form_system_config_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_form_system_config_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/form_system_config_ut.cpp

$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix): ../src/communicator.cpp $(IntermediateDirectory)/up_src_communicator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/communicator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_communicator.cpp$(DependSuffix): ../src/communicator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_communicator.cpp$(DependSuffix) -MM ../src/communicator.cpp

$(IntermediateDirectory)/up_src_communicator.cpp$(PreprocessSuffix): ../src/communicator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_communicator.cpp$(PreprocessSuffix) ../src/communicator.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/conference_dao_it.cpp $(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/conference_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(DependSuffix): ../xrm-unittest/src/conference_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(DependSuffix) -MM ../xrm-unittest/src/conference_dao_it.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/conference_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_conference_dao_it.cpp$(PreprocessSuffix) ../xrm-unittest/src/conference_dao_it.cpp

$(IntermediateDirectory)/up_src_socket_telnet.cpp$(ObjectSuffix): ../src/socket_telnet.cpp $(IntermediateDirectory)/up_src_socket_telnet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/socket_telnet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_socket_telnet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_socket_telnet.cpp$(DependSuffix): ../src/socket_telnet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_socket_telnet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_socket_telnet.cpp$(DependSuffix) -MM ../src/socket_telnet.cpp

$(IntermediateDirectory)/up_src_socket_telnet.cpp$(PreprocessSuffix): ../src/socket_telnet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_socket_telnet.cpp$(PreprocessSuffix) ../src/socket_telnet.cpp

$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix): ../src/data-sys/db_startup.cpp $(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/db_startup.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(DependSuffix): ../src/data-sys/db_startup.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(DependSuffix) -MM ../src/data-sys/db_startup.cpp

$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(PreprocessSuffix): ../src/data-sys/db_startup.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(PreprocessSuffix) ../src/data-sys/db_startup.cpp

$(IntermediateDirectory)/up_src_socket_handler.cpp$(ObjectSuffix): ../src/socket_handler.cpp $(IntermediateDirectory)/up_src_socket_handler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/socket_handler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_socket_handler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_socket_handler.cpp$(DependSuffix): ../src/socket_handler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_socket_handler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_socket_handler.cpp$(DependSuffix) -MM ../src/socket_handler.cpp

$(IntermediateDirectory)/up_src_socket_handler.cpp$(PreprocessSuffix): ../src/socket_handler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_socket_handler.cpp$(PreprocessSuffix) ../src/socket_handler.cpp

$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix): ../src/data-sys/text_prompts_dao.cpp $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/text_prompts_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(DependSuffix): ../src/data-sys/text_prompts_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(DependSuffix) -MM ../src/data-sys/text_prompts_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(PreprocessSuffix): ../src/data-sys/text_prompts_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(PreprocessSuffix) ../src/data-sys/text_prompts_dao.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/file_area_dao_it.cpp $(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/file_area_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(DependSuffix): ../xrm-unittest/src/file_area_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(DependSuffix) -MM ../xrm-unittest/src/file_area_dao_it.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/file_area_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_file_area_dao_it.cpp$(PreprocessSuffix) ../xrm-unittest/src/file_area_dao_it.cpp

$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix): ../src/access_condition.cpp $(IntermediateDirectory)/up_src_access_condition.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/access_condition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_access_condition.cpp$(DependSuffix): ../src/access_condition.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_access_condition.cpp$(DependSuffix) -MM ../src/access_condition.cpp

$(IntermediateDirectory)/up_src_access_condition.cpp$(PreprocessSuffix): ../src/access_condition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_access_condition.cpp$(PreprocessSuffix) ../src/access_condition.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/oneliners_dao_it.cpp $(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/oneliners_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(DependSuffix): ../xrm-unittest/src/oneliners_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(DependSuffix) -MM ../xrm-unittest/src/oneliners_dao_it.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/oneliners_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_oneliners_dao_it.cpp$(PreprocessSuffix) ../xrm-unittest/src/oneliners_dao_it.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/session_io_ut.cpp $(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/session_io_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(DependSuffix): ../xrm-unittest/src/session_io_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(DependSuffix) -MM ../xrm-unittest/src/session_io_ut.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/session_io_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_session_io_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/session_io_ut.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(ObjectSuffix): ../xrm-unittest/src/xrm_test.cpp $(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/xrm_test.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(DependSuffix): ../xrm-unittest/src/xrm_test.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(DependSuffix) -MM ../xrm-unittest/src/xrm_test.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(PreprocessSuffix): ../xrm-unittest/src/xrm_test.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_xrm_test.cpp$(PreprocessSuffix) ../xrm-unittest/src/xrm_test.cpp

$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix): ../src/encoding.cpp $(IntermediateDirectory)/up_src_encoding.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/encoding.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_encoding.cpp$(DependSuffix): ../src/encoding.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_encoding.cpp$(DependSuffix) -MM ../src/encoding.cpp

$(IntermediateDirectory)/up_src_encoding.cpp$(PreprocessSuffix): ../src/encoding.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_encoding.cpp$(PreprocessSuffix) ../src/encoding.cpp

$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix): ../src/common_io.cpp $(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/common_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix): ../src/common_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix) -MM ../src/common_io.cpp

$(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix): ../src/common_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix) ../src/common_io.cpp

$(IntermediateDirectory)/up_src_socket_ssh.cpp$(ObjectSuffix): ../src/socket_ssh.cpp $(IntermediateDirectory)/up_src_socket_ssh.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/socket_ssh.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_socket_ssh.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_socket_ssh.cpp$(DependSuffix): ../src/socket_ssh.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_socket_ssh.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_socket_ssh.cpp$(DependSuffix) -MM ../src/socket_ssh.cpp

$(IntermediateDirectory)/up_src_socket_ssh.cpp$(PreprocessSuffix): ../src/socket_ssh.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_socket_ssh.cpp$(PreprocessSuffix) ../src/socket_ssh.cpp

$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix): ../src/ansi_processor.cpp $(IntermediateDirectory)/up_src_ansi_processor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/ansi_processor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ansi_processor.cpp$(DependSuffix): ../src/ansi_processor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ansi_processor.cpp$(DependSuffix) -MM ../src/ansi_processor.cpp

$(IntermediateDirectory)/up_src_ansi_processor.cpp$(PreprocessSuffix): ../src/ansi_processor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ansi_processor.cpp$(PreprocessSuffix) ../src/ansi_processor.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_users_dao_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/users_dao_ut.cpp $(IntermediateDirectory)/up_xrm-unittest_src_users_dao_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/users_dao_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_users_dao_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_users_dao_ut.cpp$(DependSuffix): ../xrm-unittest/src/users_dao_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_users_dao_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_users_dao_ut.cpp$(DependSuffix) -MM ../xrm-unittest/src/users_dao_ut.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_users_dao_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/users_dao_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_users_dao_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/users_dao_ut.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/common_mock_ut.cpp $(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/common_mock_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(DependSuffix): ../xrm-unittest/src/common_mock_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(DependSuffix) -MM ../xrm-unittest/src/common_mock_ut.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/common_mock_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_common_mock_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/common_mock_ut.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_ansi_processor_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/ansi_processor_ut.cpp $(IntermediateDirectory)/up_xrm-unittest_src_ansi_processor_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/ansi_processor_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_ansi_processor_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_ansi_processor_ut.cpp$(DependSuffix): ../xrm-unittest/src/ansi_processor_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_ansi_processor_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_ansi_processor_ut.cpp$(DependSuffix) -MM ../xrm-unittest/src/ansi_processor_ut.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_ansi_processor_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/ansi_processor_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_ansi_processor_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/ansi_processor_ut.cpp

$(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix): ../src/form_manager.cpp $(IntermediateDirectory)/up_src_form_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/form_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_form_manager.cpp$(DependSuffix): ../src/form_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_form_manager.cpp$(DependSuffix) -MM ../src/form_manager.cpp

$(IntermediateDirectory)/up_src_form_manager.cpp$(PreprocessSuffix): ../src/form_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_form_manager.cpp$(PreprocessSuffix) ../src/form_manager.cpp

$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix): ../src/data-sys/security_dao.cpp $(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/security_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(DependSuffix): ../src/data-sys/security_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(DependSuffix) -MM ../src/data-sys/security_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(PreprocessSuffix): ../src/data-sys/security_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(PreprocessSuffix) ../src/data-sys/security_dao.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(ObjectSuffix): ../xrm-unittest/src/message_area_dao_it.cpp $(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/message_area_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(DependSuffix): ../xrm-unittest/src/message_area_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(DependSuffix) -MM ../xrm-unittest/src/message_area_dao_it.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(PreprocessSuffix): ../xrm-unittest/src/message_area_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_message_area_dao_it.cpp$(PreprocessSuffix) ../xrm-unittest/src/message_area_dao_it.cpp

$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix): ../src/data-sys/conference_dao.cpp $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/conference_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(DependSuffix): ../src/data-sys/conference_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(DependSuffix) -MM ../src/data-sys/conference_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(PreprocessSuffix): ../src/data-sys/conference_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(PreprocessSuffix) ../src/data-sys/conference_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix): ../src/data-sys/message_area_dao.cpp $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/message_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(DependSuffix): ../src/data-sys/message_area_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(DependSuffix) -MM ../src/data-sys/message_area_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/message_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(PreprocessSuffix) ../src/data-sys/message_area_dao.cpp

$(IntermediateDirectory)/up_src_io_service.cpp$(ObjectSuffix): ../src/io_service.cpp $(IntermediateDirectory)/up_src_io_service.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/io_service.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_io_service.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_io_service.cpp$(DependSuffix): ../src/io_service.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_io_service.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_io_service.cpp$(DependSuffix) -MM ../src/io_service.cpp

$(IntermediateDirectory)/up_src_io_service.cpp$(PreprocessSuffix): ../src/io_service.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_io_service.cpp$(PreprocessSuffix) ../src/io_service.cpp

$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix): ../src/data-sys/access_level_dao.cpp $(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/access_level_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(DependSuffix): ../src/data-sys/access_level_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(DependSuffix) -MM ../src/data-sys/access_level_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(PreprocessSuffix): ../src/data-sys/access_level_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(PreprocessSuffix) ../src/data-sys/access_level_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix): ../src/data-sys/config_dao.cpp $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/config_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix): ../src/data-sys/config_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix) -MM ../src/data-sys/config_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix): ../src/data-sys/config_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix) ../src/data-sys/config_dao.cpp

$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix): ../src/session_io.cpp $(IntermediateDirectory)/up_src_session_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/session_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_session_io.cpp$(DependSuffix): ../src/session_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_session_io.cpp$(DependSuffix) -MM ../src/session_io.cpp

$(IntermediateDirectory)/up_src_session_io.cpp$(PreprocessSuffix): ../src/session_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_session_io.cpp$(PreprocessSuffix) ../src/session_io.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/access_condition_ut.cpp $(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/access_condition_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(DependSuffix): ../xrm-unittest/src/access_condition_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(DependSuffix) -MM ../xrm-unittest/src/access_condition_ut.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/access_condition_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_access_condition_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/access_condition_ut.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix): ../src/data-sys/menu_dao.cpp $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/menu_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(DependSuffix): ../src/data-sys/menu_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(DependSuffix) -MM ../src/data-sys/menu_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(PreprocessSuffix): ../src/data-sys/menu_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(PreprocessSuffix) ../src/data-sys/menu_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix): ../src/data-sys/file_area_dao.cpp $(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/file_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(DependSuffix): ../src/data-sys/file_area_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(DependSuffix) -MM ../src/data-sys/file_area_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/file_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(PreprocessSuffix) ../src/data-sys/file_area_dao.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(ObjectSuffix): ../xrm-unittest/src/common_io_ut.cpp $(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-unittest/src/common_io_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(DependSuffix): ../xrm-unittest/src/common_io_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(DependSuffix) -MM ../xrm-unittest/src/common_io_ut.cpp

$(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(PreprocessSuffix): ../xrm-unittest/src/common_io_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-unittest_src_common_io_ut.cpp$(PreprocessSuffix) ../xrm-unittest/src/common_io_ut.cpp

$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix): ../src/data-sys/users_dao.cpp $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/users_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(DependSuffix): ../src/data-sys/users_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(DependSuffix) -MM ../src/data-sys/users_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(PreprocessSuffix): ../src/data-sys/users_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(PreprocessSuffix) ../src/data-sys/users_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix): ../src/data-sys/protocol_dao.cpp $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/protocol_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(DependSuffix): ../src/data-sys/protocol_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(DependSuffix) -MM ../src/data-sys/protocol_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(PreprocessSuffix): ../src/data-sys/protocol_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(PreprocessSuffix) ../src/data-sys/protocol_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix): ../src/data-sys/session_stats_dao.cpp $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/session_stats_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(DependSuffix): ../src/data-sys/session_stats_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(DependSuffix) -MM ../src/data-sys/session_stats_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(PreprocessSuffix): ../src/data-sys/session_stats_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(PreprocessSuffix) ../src/data-sys/session_stats_dao.cpp

$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix): ../src/forms/form_system_config.cpp $(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/forms/form_system_config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(DependSuffix): ../src/forms/form_system_config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(DependSuffix) -MM ../src/forms/form_system_config.cpp

$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(PreprocessSuffix): ../src/forms/form_system_config.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(PreprocessSuffix) ../src/forms/form_system_config.cpp

$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix): ../src/data-app/oneliners_dao.cpp $(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-app/oneliners_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(DependSuffix): ../src/data-app/oneliners_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(DependSuffix) -MM ../src/data-app/oneliners_dao.cpp

$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(PreprocessSuffix): ../src/data-app/oneliners_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(PreprocessSuffix) ../src/data-app/oneliners_dao.cpp

$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix): ../src/session_manager.cpp $(IntermediateDirectory)/up_src_session_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/session_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_session_manager.cpp$(DependSuffix): ../src/session_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_session_manager.cpp$(DependSuffix) -MM ../src/session_manager.cpp

$(IntermediateDirectory)/up_src_session_manager.cpp$(PreprocessSuffix): ../src/session_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_session_manager.cpp$(PreprocessSuffix) ../src/session_manager.cpp

$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnetTCP.cpp $(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/sdl2_net/SDLnetTCP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(DependSuffix): ../src/sdl2_net/SDLnetTCP.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(DependSuffix) -MM ../src/sdl2_net/SDLnetTCP.cpp

$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnetTCP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(PreprocessSuffix) ../src/sdl2_net/SDLnetTCP.cpp

$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(ObjectSuffix): ../src/sdl2_net/SDLnetUDP.cpp $(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/sdl2_net/SDLnetUDP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(DependSuffix): ../src/sdl2_net/SDLnetUDP.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(DependSuffix) -MM ../src/sdl2_net/SDLnetUDP.cpp

$(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(PreprocessSuffix): ../src/sdl2_net/SDLnetUDP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(PreprocessSuffix) ../src/sdl2_net/SDLnetUDP.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r Debug/


