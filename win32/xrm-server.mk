##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=xrm-server
ConfigurationName      :=Debug
WorkspacePath          :=../win32
ProjectPath            :=../win32
IntermediateDirectory  :=./Debug
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
Preprocessors          :=$(PreprocessorSwitch)WITHOUT_SDL 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="xrm-server.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-5.1.0/mingw32/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../win32/Debug $(IncludeSwitch)../src $(IncludeSwitch)../sqlitewrap $(IncludeSwitch)../src/sql2_net $(IncludeSwitch)C:/sqlite3/include $(IncludeSwitch)C:/Users/Blue/Documents/GitHub/yaml-cpp/include $(IncludeSwitch)C:/OpenSSL-Win32/include $(IncludeSwitch)C:/libssh-0.7.2/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)mingw32 $(LibrarySwitch)ws2_32 $(LibrarySwitch)wsock32 $(LibrarySwitch)ssleay32 $(LibrarySwitch)eay32 $(LibrarySwitch)ssh $(LibrarySwitch)iphlpapi $(LibrarySwitch)SqliteWrapped $(LibrarySwitch)sqlite3 $(LibrarySwitch)yaml-cpp $(LibrarySwitch)Rpcrt4 $(LibrarySwitch)iconv 
ArLibs                 :=  "mingw32" "libws2_32.a" "libwsock32.a" "ssleay32.a" "libeay32.a" "libssh" "libiphlpapi.a" "libSqliteWrapped.a" "libsqlite3.a" "yaml-cpp.dll" "Rpcrt4" "libiconv.a" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)../win32/Debug $(LibraryPathSwitch)../sqlitewrap/Debug $(LibraryPathSwitch)C:/sqlite3/lib $(LibraryPathSwitch)mingw32/lib $(LibraryPathSwitch)C:/OpenSSL-Win32/lib/MinGW $(LibraryPathSwitch)C:/OpenSSL-Win32/lib/ $(LibraryPathSwitch)C:/Users/Blue/Documents/GitHub/yaml-cpp/build $(LibraryPathSwitch)C:/libz-1.2.7-1-mingw32-dev/lib $(LibraryPathSwitch)C:/libssh-0.7.2\lib 

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
Objects0=$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_system_config.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_file_lister.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_mods_mod_file_area_editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_message_base_editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_file_editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_menu_stats_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_mods_mod_message_reader.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_session_data.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_socket_telnet.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_protocol_editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_xrm_server.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_menu_system.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_io_service.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_encryption.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/up_src_socket_ssh.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_socket_handler.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_telnet_decoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_sdl2_net_SDLnet.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetselect.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_process_win.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_state_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_form_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_menu_base.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_sdl2_net_SDLnetTCP.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_sdl2_net_SDLnetUDP.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:
	@echo Executing Pre Build commands ...
	sed -i 's+../+../+' xrm-server.mk
	@echo Done


##
## Objects
##
$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix): ../src/data-app/oneliners_dao.cpp $(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-app/oneliners_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(DependSuffix): ../src/data-app/oneliners_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(DependSuffix) -MM ../src/data-app/oneliners_dao.cpp

$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(PreprocessSuffix): ../src/data-app/oneliners_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(PreprocessSuffix) ../src/data-app/oneliners_dao.cpp

$(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(ObjectSuffix): ../src/data-app/bbs_list_dao.cpp $(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-app/bbs_list_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(DependSuffix): ../src/data-app/bbs_list_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(DependSuffix) -MM ../src/data-app/bbs_list_dao.cpp

$(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(PreprocessSuffix): ../src/data-app/bbs_list_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-app_bbs_list_dao.cpp$(PreprocessSuffix) ../src/data-app/bbs_list_dao.cpp

$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix): ../src/forms/form_system_config.cpp $(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/forms/form_system_config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(DependSuffix): ../src/forms/form_system_config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(DependSuffix) -MM ../src/forms/form_system_config.cpp

$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(PreprocessSuffix): ../src/forms/form_system_config.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(PreprocessSuffix) ../src/forms/form_system_config.cpp

$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(ObjectSuffix): ../src/mods/mod_signup.cpp $(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_signup.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(DependSuffix): ../src/mods/mod_signup.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(DependSuffix) -MM ../src/mods/mod_signup.cpp

$(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(PreprocessSuffix): ../src/mods/mod_signup.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_signup.cpp$(PreprocessSuffix) ../src/mods/mod_signup.cpp

$(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(ObjectSuffix): ../src/mods/mod_menu_editor.cpp $(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_menu_editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(DependSuffix): ../src/mods/mod_menu_editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(DependSuffix) -MM ../src/mods/mod_menu_editor.cpp

$(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(PreprocessSuffix): ../src/mods/mod_menu_editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_menu_editor.cpp$(PreprocessSuffix) ../src/mods/mod_menu_editor.cpp

$(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(ObjectSuffix): ../src/mods/mod_user_editor.cpp $(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_user_editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(DependSuffix): ../src/mods/mod_user_editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(DependSuffix) -MM ../src/mods/mod_user_editor.cpp

$(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(PreprocessSuffix): ../src/mods/mod_user_editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_user_editor.cpp$(PreprocessSuffix) ../src/mods/mod_user_editor.cpp

$(IntermediateDirectory)/up_src_mods_mod_system_config.cpp$(ObjectSuffix): ../src/mods/mod_system_config.cpp $(IntermediateDirectory)/up_src_mods_mod_system_config.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_system_config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_system_config.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_system_config.cpp$(DependSuffix): ../src/mods/mod_system_config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_system_config.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_system_config.cpp$(DependSuffix) -MM ../src/mods/mod_system_config.cpp

$(IntermediateDirectory)/up_src_mods_mod_system_config.cpp$(PreprocessSuffix): ../src/mods/mod_system_config.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_system_config.cpp$(PreprocessSuffix) ../src/mods/mod_system_config.cpp

$(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(ObjectSuffix): ../src/mods/mod_message_editor.cpp $(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_message_editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(DependSuffix): ../src/mods/mod_message_editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(DependSuffix) -MM ../src/mods/mod_message_editor.cpp

$(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(PreprocessSuffix): ../src/mods/mod_message_editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_message_editor.cpp$(PreprocessSuffix) ../src/mods/mod_message_editor.cpp

$(IntermediateDirectory)/up_src_mods_mod_file_lister.cpp$(ObjectSuffix): ../src/mods/mod_file_lister.cpp $(IntermediateDirectory)/up_src_mods_mod_file_lister.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_file_lister.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_file_lister.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_file_lister.cpp$(DependSuffix): ../src/mods/mod_file_lister.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_file_lister.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_file_lister.cpp$(DependSuffix) -MM ../src/mods/mod_file_lister.cpp

$(IntermediateDirectory)/up_src_mods_mod_file_lister.cpp$(PreprocessSuffix): ../src/mods/mod_file_lister.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_file_lister.cpp$(PreprocessSuffix) ../src/mods/mod_file_lister.cpp

$(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(ObjectSuffix): ../src/mods/mod_level_editor.cpp $(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_level_editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(DependSuffix): ../src/mods/mod_level_editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(DependSuffix) -MM ../src/mods/mod_level_editor.cpp

$(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(PreprocessSuffix): ../src/mods/mod_level_editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_level_editor.cpp$(PreprocessSuffix) ../src/mods/mod_level_editor.cpp

$(IntermediateDirectory)/up_src_mods_mod_file_area_editor.cpp$(ObjectSuffix): ../src/mods/mod_file_area_editor.cpp $(IntermediateDirectory)/up_src_mods_mod_file_area_editor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_file_area_editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_file_area_editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_file_area_editor.cpp$(DependSuffix): ../src/mods/mod_file_area_editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_file_area_editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_file_area_editor.cpp$(DependSuffix) -MM ../src/mods/mod_file_area_editor.cpp

$(IntermediateDirectory)/up_src_mods_mod_file_area_editor.cpp$(PreprocessSuffix): ../src/mods/mod_file_area_editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_file_area_editor.cpp$(PreprocessSuffix) ../src/mods/mod_file_area_editor.cpp

$(IntermediateDirectory)/up_src_mods_mod_message_base_editor.cpp$(ObjectSuffix): ../src/mods/mod_message_base_editor.cpp $(IntermediateDirectory)/up_src_mods_mod_message_base_editor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_message_base_editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_message_base_editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_message_base_editor.cpp$(DependSuffix): ../src/mods/mod_message_base_editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_message_base_editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_message_base_editor.cpp$(DependSuffix) -MM ../src/mods/mod_message_base_editor.cpp

$(IntermediateDirectory)/up_src_mods_mod_message_base_editor.cpp$(PreprocessSuffix): ../src/mods/mod_message_base_editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_message_base_editor.cpp$(PreprocessSuffix) ../src/mods/mod_message_base_editor.cpp

$(IntermediateDirectory)/up_src_mods_mod_file_editor.cpp$(ObjectSuffix): ../src/mods/mod_file_editor.cpp $(IntermediateDirectory)/up_src_mods_mod_file_editor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_file_editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_file_editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_file_editor.cpp$(DependSuffix): ../src/mods/mod_file_editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_file_editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_file_editor.cpp$(DependSuffix) -MM ../src/mods/mod_file_editor.cpp

$(IntermediateDirectory)/up_src_mods_mod_file_editor.cpp$(PreprocessSuffix): ../src/mods/mod_file_editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_file_editor.cpp$(PreprocessSuffix) ../src/mods/mod_file_editor.cpp

$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix): ../src/data-sys/db_startup.cpp $(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/db_startup.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(DependSuffix): ../src/data-sys/db_startup.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(DependSuffix) -MM ../src/data-sys/db_startup.cpp

$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(PreprocessSuffix): ../src/data-sys/db_startup.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(PreprocessSuffix) ../src/data-sys/db_startup.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_stats_dao.cpp$(ObjectSuffix): ../src/data-sys/menu_stats_dao.cpp $(IntermediateDirectory)/up_src_data-sys_menu_stats_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/menu_stats_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_menu_stats_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_menu_stats_dao.cpp$(DependSuffix): ../src/data-sys/menu_stats_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_menu_stats_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_menu_stats_dao.cpp$(DependSuffix) -MM ../src/data-sys/menu_stats_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_stats_dao.cpp$(PreprocessSuffix): ../src/data-sys/menu_stats_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_menu_stats_dao.cpp$(PreprocessSuffix) ../src/data-sys/menu_stats_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix): ../src/data-sys/access_level_dao.cpp $(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/access_level_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(DependSuffix): ../src/data-sys/access_level_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(DependSuffix) -MM ../src/data-sys/access_level_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(PreprocessSuffix): ../src/data-sys/access_level_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_access_level_dao.cpp$(PreprocessSuffix) ../src/data-sys/access_level_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix): ../src/data-sys/protocol_dao.cpp $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/protocol_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(DependSuffix): ../src/data-sys/protocol_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(DependSuffix) -MM ../src/data-sys/protocol_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(PreprocessSuffix): ../src/data-sys/protocol_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(PreprocessSuffix) ../src/data-sys/protocol_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(ObjectSuffix): ../src/data-sys/network_dao.cpp $(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/network_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(DependSuffix): ../src/data-sys/network_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(DependSuffix) -MM ../src/data-sys/network_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(PreprocessSuffix): ../src/data-sys/network_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_network_dao.cpp$(PreprocessSuffix) ../src/data-sys/network_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix): ../src/data-sys/message_area_dao.cpp $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/message_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(DependSuffix): ../src/data-sys/message_area_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(DependSuffix) -MM ../src/data-sys/message_area_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/message_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(PreprocessSuffix) ../src/data-sys/message_area_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix): ../src/data-sys/file_area_dao.cpp $(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/file_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(DependSuffix): ../src/data-sys/file_area_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(DependSuffix) -MM ../src/data-sys/file_area_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/file_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(PreprocessSuffix) ../src/data-sys/file_area_dao.cpp

$(IntermediateDirectory)/up_src_mods_mod_message_reader.cpp$(ObjectSuffix): ../src/mods/mod_message_reader.cpp $(IntermediateDirectory)/up_src_mods_mod_message_reader.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_message_reader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_message_reader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_message_reader.cpp$(DependSuffix): ../src/mods/mod_message_reader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_message_reader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_message_reader.cpp$(DependSuffix) -MM ../src/mods/mod_message_reader.cpp

$(IntermediateDirectory)/up_src_mods_mod_message_reader.cpp$(PreprocessSuffix): ../src/mods/mod_message_reader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_message_reader.cpp$(PreprocessSuffix) ../src/mods/mod_message_reader.cpp

$(IntermediateDirectory)/up_src_session_data.cpp$(ObjectSuffix): ../src/session_data.cpp $(IntermediateDirectory)/up_src_session_data.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/session_data.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_session_data.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_session_data.cpp$(DependSuffix): ../src/session_data.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_session_data.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_session_data.cpp$(DependSuffix) -MM ../src/session_data.cpp

$(IntermediateDirectory)/up_src_session_data.cpp$(PreprocessSuffix): ../src/session_data.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_session_data.cpp$(PreprocessSuffix) ../src/session_data.cpp

$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix): ../src/data-sys/text_prompts_dao.cpp $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/text_prompts_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(DependSuffix): ../src/data-sys/text_prompts_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(DependSuffix) -MM ../src/data-sys/text_prompts_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(PreprocessSuffix): ../src/data-sys/text_prompts_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(PreprocessSuffix) ../src/data-sys/text_prompts_dao.cpp

$(IntermediateDirectory)/up_src_socket_telnet.cpp$(ObjectSuffix): ../src/socket_telnet.cpp $(IntermediateDirectory)/up_src_socket_telnet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/socket_telnet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_socket_telnet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_socket_telnet.cpp$(DependSuffix): ../src/socket_telnet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_socket_telnet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_socket_telnet.cpp$(DependSuffix) -MM ../src/socket_telnet.cpp

$(IntermediateDirectory)/up_src_socket_telnet.cpp$(PreprocessSuffix): ../src/socket_telnet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_socket_telnet.cpp$(PreprocessSuffix) ../src/socket_telnet.cpp

$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix): ../src/common_io.cpp $(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/common_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix): ../src/common_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix) -MM ../src/common_io.cpp

$(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix): ../src/common_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix) ../src/common_io.cpp

$(IntermediateDirectory)/up_src_mods_mod_protocol_editor.cpp$(ObjectSuffix): ../src/mods/mod_protocol_editor.cpp $(IntermediateDirectory)/up_src_mods_mod_protocol_editor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_protocol_editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_protocol_editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_protocol_editor.cpp$(DependSuffix): ../src/mods/mod_protocol_editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_protocol_editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_protocol_editor.cpp$(DependSuffix) -MM ../src/mods/mod_protocol_editor.cpp

$(IntermediateDirectory)/up_src_mods_mod_protocol_editor.cpp$(PreprocessSuffix): ../src/mods/mod_protocol_editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_protocol_editor.cpp$(PreprocessSuffix) ../src/mods/mod_protocol_editor.cpp

$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix): ../src/ansi_processor.cpp $(IntermediateDirectory)/up_src_ansi_processor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/ansi_processor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ansi_processor.cpp$(DependSuffix): ../src/ansi_processor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ansi_processor.cpp$(DependSuffix) -MM ../src/ansi_processor.cpp

$(IntermediateDirectory)/up_src_ansi_processor.cpp$(PreprocessSuffix): ../src/ansi_processor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ansi_processor.cpp$(PreprocessSuffix) ../src/ansi_processor.cpp

$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix): ../src/data-sys/config_dao.cpp $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/config_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix): ../src/data-sys/config_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix) -MM ../src/data-sys/config_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix): ../src/data-sys/config_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix) ../src/data-sys/config_dao.cpp

$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(ObjectSuffix): ../src/mods/mod_prelogon.cpp $(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_prelogon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(DependSuffix): ../src/mods/mod_prelogon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(DependSuffix) -MM ../src/mods/mod_prelogon.cpp

$(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(PreprocessSuffix): ../src/mods/mod_prelogon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_prelogon.cpp$(PreprocessSuffix) ../src/mods/mod_prelogon.cpp

$(IntermediateDirectory)/up_src_xrm_server.cpp$(ObjectSuffix): ../src/xrm_server.cpp $(IntermediateDirectory)/up_src_xrm_server.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/xrm_server.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_xrm_server.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_xrm_server.cpp$(DependSuffix): ../src/xrm_server.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_xrm_server.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_xrm_server.cpp$(DependSuffix) -MM ../src/xrm_server.cpp

$(IntermediateDirectory)/up_src_xrm_server.cpp$(PreprocessSuffix): ../src/xrm_server.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_xrm_server.cpp$(PreprocessSuffix) ../src/xrm_server.cpp

$(IntermediateDirectory)/up_src_menu_system.cpp$(ObjectSuffix): ../src/menu_system.cpp $(IntermediateDirectory)/up_src_menu_system.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/menu_system.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_menu_system.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_menu_system.cpp$(DependSuffix): ../src/menu_system.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_menu_system.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_menu_system.cpp$(DependSuffix) -MM ../src/menu_system.cpp

$(IntermediateDirectory)/up_src_menu_system.cpp$(PreprocessSuffix): ../src/menu_system.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_menu_system.cpp$(PreprocessSuffix) ../src/menu_system.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(ObjectSuffix): ../src/data-sys/menu_prompt_dao.cpp $(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/menu_prompt_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(DependSuffix): ../src/data-sys/menu_prompt_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(DependSuffix) -MM ../src/data-sys/menu_prompt_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(PreprocessSuffix): ../src/data-sys/menu_prompt_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_menu_prompt_dao.cpp$(PreprocessSuffix) ../src/data-sys/menu_prompt_dao.cpp

$(IntermediateDirectory)/up_src_io_service.cpp$(ObjectSuffix): ../src/io_service.cpp $(IntermediateDirectory)/up_src_io_service.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/io_service.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_io_service.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_io_service.cpp$(DependSuffix): ../src/io_service.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_io_service.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_io_service.cpp$(DependSuffix) -MM ../src/io_service.cpp

$(IntermediateDirectory)/up_src_io_service.cpp$(PreprocessSuffix): ../src/io_service.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_io_service.cpp$(PreprocessSuffix) ../src/io_service.cpp

$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix): ../src/encoding.cpp $(IntermediateDirectory)/up_src_encoding.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/encoding.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_encoding.cpp$(DependSuffix): ../src/encoding.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_encoding.cpp$(DependSuffix) -MM ../src/encoding.cpp

$(IntermediateDirectory)/up_src_encoding.cpp$(PreprocessSuffix): ../src/encoding.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_encoding.cpp$(PreprocessSuffix) ../src/encoding.cpp

$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix): ../src/communicator.cpp $(IntermediateDirectory)/up_src_communicator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/communicator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_communicator.cpp$(DependSuffix): ../src/communicator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_communicator.cpp$(DependSuffix) -MM ../src/communicator.cpp

$(IntermediateDirectory)/up_src_communicator.cpp$(PreprocessSuffix): ../src/communicator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_communicator.cpp$(PreprocessSuffix) ../src/communicator.cpp

$(IntermediateDirectory)/up_src_encryption.cpp$(ObjectSuffix): ../src/encryption.cpp $(IntermediateDirectory)/up_src_encryption.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/encryption.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_encryption.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_encryption.cpp$(DependSuffix): ../src/encryption.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_encryption.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_encryption.cpp$(DependSuffix) -MM ../src/encryption.cpp

$(IntermediateDirectory)/up_src_encryption.cpp$(PreprocessSuffix): ../src/encryption.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_encryption.cpp$(PreprocessSuffix) ../src/encryption.cpp

$(IntermediateDirectory)/up_src_socket_ssh.cpp$(ObjectSuffix): ../src/socket_ssh.cpp $(IntermediateDirectory)/up_src_socket_ssh.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/socket_ssh.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_socket_ssh.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_socket_ssh.cpp$(DependSuffix): ../src/socket_ssh.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_socket_ssh.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_socket_ssh.cpp$(DependSuffix) -MM ../src/socket_ssh.cpp

$(IntermediateDirectory)/up_src_socket_ssh.cpp$(PreprocessSuffix): ../src/socket_ssh.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_socket_ssh.cpp$(PreprocessSuffix) ../src/socket_ssh.cpp

$(IntermediateDirectory)/up_src_socket_handler.cpp$(ObjectSuffix): ../src/socket_handler.cpp $(IntermediateDirectory)/up_src_socket_handler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/socket_handler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_socket_handler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_socket_handler.cpp$(DependSuffix): ../src/socket_handler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_socket_handler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_socket_handler.cpp$(DependSuffix) -MM ../src/socket_handler.cpp

$(IntermediateDirectory)/up_src_socket_handler.cpp$(PreprocessSuffix): ../src/socket_handler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_socket_handler.cpp$(PreprocessSuffix) ../src/socket_handler.cpp

$(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(ObjectSuffix): ../src/data-sys/grouping_dao.cpp $(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/grouping_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(DependSuffix): ../src/data-sys/grouping_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(DependSuffix) -MM ../src/data-sys/grouping_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(PreprocessSuffix): ../src/data-sys/grouping_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_grouping_dao.cpp$(PreprocessSuffix) ../src/data-sys/grouping_dao.cpp

$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix): ../src/session_manager.cpp $(IntermediateDirectory)/up_src_session_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/session_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_session_manager.cpp$(DependSuffix): ../src/session_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_session_manager.cpp$(DependSuffix) -MM ../src/session_manager.cpp

$(IntermediateDirectory)/up_src_session_manager.cpp$(PreprocessSuffix): ../src/session_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_session_manager.cpp$(PreprocessSuffix) ../src/session_manager.cpp

$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix): ../src/access_condition.cpp $(IntermediateDirectory)/up_src_access_condition.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/access_condition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_access_condition.cpp$(DependSuffix): ../src/access_condition.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_access_condition.cpp$(DependSuffix) -MM ../src/access_condition.cpp

$(IntermediateDirectory)/up_src_access_condition.cpp$(PreprocessSuffix): ../src/access_condition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_access_condition.cpp$(PreprocessSuffix) ../src/access_condition.cpp

$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(ObjectSuffix): ../src/telnet_decoder.cpp $(IntermediateDirectory)/up_src_telnet_decoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/telnet_decoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(DependSuffix): ../src/telnet_decoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(DependSuffix) -MM ../src/telnet_decoder.cpp

$(IntermediateDirectory)/up_src_telnet_decoder.cpp$(PreprocessSuffix): ../src/telnet_decoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_telnet_decoder.cpp$(PreprocessSuffix) ../src/telnet_decoder.cpp

$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix): ../src/data-sys/conference_dao.cpp $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/conference_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(DependSuffix): ../src/data-sys/conference_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(DependSuffix) -MM ../src/data-sys/conference_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(PreprocessSuffix): ../src/data-sys/conference_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(PreprocessSuffix) ../src/data-sys/conference_dao.cpp

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

$(IntermediateDirectory)/up_src_process_win.cpp$(ObjectSuffix): ../src/process_win.cpp $(IntermediateDirectory)/up_src_process_win.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/process_win.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_process_win.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_process_win.cpp$(DependSuffix): ../src/process_win.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_process_win.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_process_win.cpp$(DependSuffix) -MM ../src/process_win.cpp

$(IntermediateDirectory)/up_src_process_win.cpp$(PreprocessSuffix): ../src/process_win.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_process_win.cpp$(PreprocessSuffix) ../src/process_win.cpp

$(IntermediateDirectory)/up_src_state_manager.cpp$(ObjectSuffix): ../src/state_manager.cpp $(IntermediateDirectory)/up_src_state_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/state_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_state_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_state_manager.cpp$(DependSuffix): ../src/state_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_state_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_state_manager.cpp$(DependSuffix) -MM ../src/state_manager.cpp

$(IntermediateDirectory)/up_src_state_manager.cpp$(PreprocessSuffix): ../src/state_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_state_manager.cpp$(PreprocessSuffix) ../src/state_manager.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix): ../src/data-sys/menu_dao.cpp $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/menu_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(DependSuffix): ../src/data-sys/menu_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(DependSuffix) -MM ../src/data-sys/menu_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(PreprocessSuffix): ../src/data-sys/menu_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(PreprocessSuffix) ../src/data-sys/menu_dao.cpp

$(IntermediateDirectory)/up_src_mods_mod_form_manager.cpp$(ObjectSuffix): ../src/mods/mod_form_manager.cpp $(IntermediateDirectory)/up_src_mods_mod_form_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_form_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_form_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_form_manager.cpp$(DependSuffix): ../src/mods/mod_form_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_form_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_form_manager.cpp$(DependSuffix) -MM ../src/mods/mod_form_manager.cpp

$(IntermediateDirectory)/up_src_mods_mod_form_manager.cpp$(PreprocessSuffix): ../src/mods/mod_form_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_form_manager.cpp$(PreprocessSuffix) ../src/mods/mod_form_manager.cpp

$(IntermediateDirectory)/up_src_menu_base.cpp$(ObjectSuffix): ../src/menu_base.cpp $(IntermediateDirectory)/up_src_menu_base.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/menu_base.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_menu_base.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_menu_base.cpp$(DependSuffix): ../src/menu_base.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_menu_base.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_menu_base.cpp$(DependSuffix) -MM ../src/menu_base.cpp

$(IntermediateDirectory)/up_src_menu_base.cpp$(PreprocessSuffix): ../src/menu_base.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_menu_base.cpp$(PreprocessSuffix) ../src/menu_base.cpp

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

$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix): ../src/session_io.cpp $(IntermediateDirectory)/up_src_session_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/session_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_session_io.cpp$(DependSuffix): ../src/session_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_session_io.cpp$(DependSuffix) -MM ../src/session_io.cpp

$(IntermediateDirectory)/up_src_session_io.cpp$(PreprocessSuffix): ../src/session_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_session_io.cpp$(PreprocessSuffix) ../src/session_io.cpp

$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix): ../src/data-sys/users_dao.cpp $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/users_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(DependSuffix): ../src/data-sys/users_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(DependSuffix) -MM ../src/data-sys/users_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(PreprocessSuffix): ../src/data-sys/users_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(PreprocessSuffix) ../src/data-sys/users_dao.cpp

$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(ObjectSuffix): ../src/mods/mod_logon.cpp $(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/mods/mod_logon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(DependSuffix): ../src/mods/mod_logon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(DependSuffix) -MM ../src/mods/mod_logon.cpp

$(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(PreprocessSuffix): ../src/mods/mod_logon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mods_mod_logon.cpp$(PreprocessSuffix) ../src/mods/mod_logon.cpp

$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix): ../src/data-sys/session_stats_dao.cpp $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/session_stats_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(DependSuffix): ../src/data-sys/session_stats_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(DependSuffix) -MM ../src/data-sys/session_stats_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(PreprocessSuffix): ../src/data-sys/session_stats_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(PreprocessSuffix) ../src/data-sys/session_stats_dao.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


