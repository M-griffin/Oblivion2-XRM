##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=xrm-server
ConfigurationName      :=Debug
WorkspacePath          := "C:\Users\Blue\Desktop\Test2"
ProjectPath            := "C:\Users\Blue\Desktop\Test2"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Blue
Date                   :=3/24/2016
CodeLitePath           :="C:\Program Files (x86)\CodeLite"
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
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="xrm-server.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-5.1.0/mingw32/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)C:/boost/include/boost-1_58 $(IncludeSwitch)C:/boost_1_58_0/boost $(IncludeSwitch)C:/boost/include $(IncludeSwitch)C:/Users/Blue/Desktop/Test2/sqlitewrap $(IncludeSwitch)C:/sqlite3/include $(IncludeSwitch)C:/OpenSSL-Win32/include $(IncludeSwitch)C:/Users/Blue/Documents/GitHub/yaml-cpp/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)boost_system-mgw51-mt-1_58 $(LibrarySwitch)boost_thread-mgw51-mt-1_58 $(LibrarySwitch)boost_chrono-mgw51-mt-1_58 $(LibrarySwitch)boost_serialization-mgw51-mt-1_58 $(LibrarySwitch)boost_locale-mgw51-mt-1_58 $(LibrarySwitch)boost_filesystem-mgw51-mt-1_58 $(LibrarySwitch)boost_timer-mgw51-mt-1_58 $(LibrarySwitch)boost_regex-mgw51-mt-1_58 $(LibrarySwitch)ws2_32 $(LibrarySwitch)wsock32 $(LibrarySwitch)iconv $(LibrarySwitch)icuuc.dll $(LibrarySwitch)icuin.dll $(LibrarySwitch)SqliteWrapped $(LibrarySwitch)sqlite3 $(LibrarySwitch)ssleay32 $(LibrarySwitch)eay32 $(LibrarySwitch)yaml-cpp 
ArLibs                 :=  "libboost_system-mgw51-mt-1_58" "libboost_thread-mgw51-mt-1_58" "libboost_chrono-mgw51-mt-1_58" "libboost_serialization-mgw51-mt-1_58" "libboost_locale-mgw51-mt-1_58" "libboost_filesystem-mgw51-mt-1_58" "libboost_timer-mgw51-mt-1_58" "libboost_regex-mgw51-mt-1_58" "libws2_32.a" "libwsock32.a" "libiconv.a" "libicuuc.dll.a" "libicuin.dll.a" "libSqliteWrapped.a" "libsqlite3.a" "ssleay32.a" "libeay32.a" "yaml-cpp.dll" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)C:/boost/lib $(LibraryPathSwitch)C:/msys/1.0/icu/source/lib $(LibraryPathSwitch)C:/sqlite3/lib $(LibraryPathSwitch)C:/Users/Blue/Desktop/Test2/sqlitewrap/Debug $(LibraryPathSwitch)C:/OpenSSL-Win32/lib/MinGW $(LibraryPathSwitch)C:/OpenSSL-Win32/lib/ $(LibraryPathSwitch)C:/Users/Blue/Documents/GitHub/yaml-cpp/build 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW-5.1.0/mingw32/bin/ar.exe rcu
CXX      := C:/MinGW-5.1.0/mingw32/bin/g++.exe
CC       := C:/MinGW-5.1.0/mingw32/bin/gcc.exe
CXXFLAGS :=  -Wfatal-errors -g -O0 -pedantic -W -std=c++14 -Wall -Wno-deprecated -Wno-unused-local-typedefs $(Preprocessors)
CFLAGS   :=  -Wfatal-errors -g -O0 -pedantic -W -std=c++14 -Wall -Wno-deprecated -Wno-unused-local-typedefs $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW-5.1.0/mingw32/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
UNIT_TEST_PP_SRC_DIR:=C:\UnitTest++-1.3
Objects0=$(IntermediateDirectory)/src_ansi_processor.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_communicator.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_menu_system.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_menu_system_editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_menu_system_prompts.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_session_data.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_session_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_telnet_decoder.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_state_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_session_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_state_system.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_state_chat.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_data_config_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_data_users_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_data_text_prompts_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_mods_mod_logon.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_ansi_processor.cpp$(ObjectSuffix): src/ansi_processor.cpp $(IntermediateDirectory)/src_ansi_processor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/ansi_processor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ansi_processor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ansi_processor.cpp$(DependSuffix): src/ansi_processor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_ansi_processor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_ansi_processor.cpp$(DependSuffix) -MM "src/ansi_processor.cpp"

$(IntermediateDirectory)/src_ansi_processor.cpp$(PreprocessSuffix): src/ansi_processor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ansi_processor.cpp$(PreprocessSuffix) "src/ansi_processor.cpp"

$(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix): src/common_io.cpp $(IntermediateDirectory)/src_common_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/common_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_common_io.cpp$(DependSuffix): src/common_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_common_io.cpp$(DependSuffix) -MM "src/common_io.cpp"

$(IntermediateDirectory)/src_common_io.cpp$(PreprocessSuffix): src/common_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_common_io.cpp$(PreprocessSuffix) "src/common_io.cpp"

$(IntermediateDirectory)/src_communicator.cpp$(ObjectSuffix): src/communicator.cpp $(IntermediateDirectory)/src_communicator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/communicator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_communicator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_communicator.cpp$(DependSuffix): src/communicator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_communicator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_communicator.cpp$(DependSuffix) -MM "src/communicator.cpp"

$(IntermediateDirectory)/src_communicator.cpp$(PreprocessSuffix): src/communicator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_communicator.cpp$(PreprocessSuffix) "src/communicator.cpp"

$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix): src/main.cpp $(IntermediateDirectory)/src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main.cpp$(DependSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main.cpp$(DependSuffix) -MM "src/main.cpp"

$(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix): src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix) "src/main.cpp"

$(IntermediateDirectory)/src_menu_system.cpp$(ObjectSuffix): src/menu_system.cpp $(IntermediateDirectory)/src_menu_system.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/menu_system.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_menu_system.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_menu_system.cpp$(DependSuffix): src/menu_system.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_menu_system.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_menu_system.cpp$(DependSuffix) -MM "src/menu_system.cpp"

$(IntermediateDirectory)/src_menu_system.cpp$(PreprocessSuffix): src/menu_system.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_menu_system.cpp$(PreprocessSuffix) "src/menu_system.cpp"

$(IntermediateDirectory)/src_menu_system_editor.cpp$(ObjectSuffix): src/menu_system_editor.cpp $(IntermediateDirectory)/src_menu_system_editor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/menu_system_editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_menu_system_editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_menu_system_editor.cpp$(DependSuffix): src/menu_system_editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_menu_system_editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_menu_system_editor.cpp$(DependSuffix) -MM "src/menu_system_editor.cpp"

$(IntermediateDirectory)/src_menu_system_editor.cpp$(PreprocessSuffix): src/menu_system_editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_menu_system_editor.cpp$(PreprocessSuffix) "src/menu_system_editor.cpp"

$(IntermediateDirectory)/src_menu_system_prompts.cpp$(ObjectSuffix): src/menu_system_prompts.cpp $(IntermediateDirectory)/src_menu_system_prompts.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/menu_system_prompts.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_menu_system_prompts.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_menu_system_prompts.cpp$(DependSuffix): src/menu_system_prompts.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_menu_system_prompts.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_menu_system_prompts.cpp$(DependSuffix) -MM "src/menu_system_prompts.cpp"

$(IntermediateDirectory)/src_menu_system_prompts.cpp$(PreprocessSuffix): src/menu_system_prompts.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_menu_system_prompts.cpp$(PreprocessSuffix) "src/menu_system_prompts.cpp"

$(IntermediateDirectory)/src_session_data.cpp$(ObjectSuffix): src/session_data.cpp $(IntermediateDirectory)/src_session_data.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/session_data.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_session_data.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_session_data.cpp$(DependSuffix): src/session_data.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_session_data.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_session_data.cpp$(DependSuffix) -MM "src/session_data.cpp"

$(IntermediateDirectory)/src_session_data.cpp$(PreprocessSuffix): src/session_data.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_session_data.cpp$(PreprocessSuffix) "src/session_data.cpp"

$(IntermediateDirectory)/src_session_io.cpp$(ObjectSuffix): src/session_io.cpp $(IntermediateDirectory)/src_session_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/session_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_session_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_session_io.cpp$(DependSuffix): src/session_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_session_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_session_io.cpp$(DependSuffix) -MM "src/session_io.cpp"

$(IntermediateDirectory)/src_session_io.cpp$(PreprocessSuffix): src/session_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_session_io.cpp$(PreprocessSuffix) "src/session_io.cpp"

$(IntermediateDirectory)/src_telnet_decoder.cpp$(ObjectSuffix): src/telnet_decoder.cpp $(IntermediateDirectory)/src_telnet_decoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/telnet_decoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_telnet_decoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_telnet_decoder.cpp$(DependSuffix): src/telnet_decoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_telnet_decoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_telnet_decoder.cpp$(DependSuffix) -MM "src/telnet_decoder.cpp"

$(IntermediateDirectory)/src_telnet_decoder.cpp$(PreprocessSuffix): src/telnet_decoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_telnet_decoder.cpp$(PreprocessSuffix) "src/telnet_decoder.cpp"

$(IntermediateDirectory)/src_state_manager.cpp$(ObjectSuffix): src/state_manager.cpp $(IntermediateDirectory)/src_state_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/state_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_state_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_state_manager.cpp$(DependSuffix): src/state_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_state_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_state_manager.cpp$(DependSuffix) -MM "src/state_manager.cpp"

$(IntermediateDirectory)/src_state_manager.cpp$(PreprocessSuffix): src/state_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_state_manager.cpp$(PreprocessSuffix) "src/state_manager.cpp"

$(IntermediateDirectory)/src_session_manager.cpp$(ObjectSuffix): src/session_manager.cpp $(IntermediateDirectory)/src_session_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/session_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_session_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_session_manager.cpp$(DependSuffix): src/session_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_session_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_session_manager.cpp$(DependSuffix) -MM "src/session_manager.cpp"

$(IntermediateDirectory)/src_session_manager.cpp$(PreprocessSuffix): src/session_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_session_manager.cpp$(PreprocessSuffix) "src/session_manager.cpp"

$(IntermediateDirectory)/src_state_system.cpp$(ObjectSuffix): src/state_system.cpp $(IntermediateDirectory)/src_state_system.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/state_system.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_state_system.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_state_system.cpp$(DependSuffix): src/state_system.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_state_system.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_state_system.cpp$(DependSuffix) -MM "src/state_system.cpp"

$(IntermediateDirectory)/src_state_system.cpp$(PreprocessSuffix): src/state_system.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_state_system.cpp$(PreprocessSuffix) "src/state_system.cpp"

$(IntermediateDirectory)/src_state_chat.cpp$(ObjectSuffix): src/state_chat.cpp $(IntermediateDirectory)/src_state_chat.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/state_chat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_state_chat.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_state_chat.cpp$(DependSuffix): src/state_chat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_state_chat.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_state_chat.cpp$(DependSuffix) -MM "src/state_chat.cpp"

$(IntermediateDirectory)/src_state_chat.cpp$(PreprocessSuffix): src/state_chat.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_state_chat.cpp$(PreprocessSuffix) "src/state_chat.cpp"

$(IntermediateDirectory)/src_data_config_dao.cpp$(ObjectSuffix): src/data/config_dao.cpp $(IntermediateDirectory)/src_data_config_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/data/config_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_data_config_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_data_config_dao.cpp$(DependSuffix): src/data/config_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_data_config_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_data_config_dao.cpp$(DependSuffix) -MM "src/data/config_dao.cpp"

$(IntermediateDirectory)/src_data_config_dao.cpp$(PreprocessSuffix): src/data/config_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_data_config_dao.cpp$(PreprocessSuffix) "src/data/config_dao.cpp"

$(IntermediateDirectory)/src_data_users_dao.cpp$(ObjectSuffix): src/data/users_dao.cpp $(IntermediateDirectory)/src_data_users_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/data/users_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_data_users_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_data_users_dao.cpp$(DependSuffix): src/data/users_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_data_users_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_data_users_dao.cpp$(DependSuffix) -MM "src/data/users_dao.cpp"

$(IntermediateDirectory)/src_data_users_dao.cpp$(PreprocessSuffix): src/data/users_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_data_users_dao.cpp$(PreprocessSuffix) "src/data/users_dao.cpp"

$(IntermediateDirectory)/src_data_text_prompts_dao.cpp$(ObjectSuffix): src/data/text_prompts_dao.cpp $(IntermediateDirectory)/src_data_text_prompts_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/data/text_prompts_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_data_text_prompts_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_data_text_prompts_dao.cpp$(DependSuffix): src/data/text_prompts_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_data_text_prompts_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_data_text_prompts_dao.cpp$(DependSuffix) -MM "src/data/text_prompts_dao.cpp"

$(IntermediateDirectory)/src_data_text_prompts_dao.cpp$(PreprocessSuffix): src/data/text_prompts_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_data_text_prompts_dao.cpp$(PreprocessSuffix) "src/data/text_prompts_dao.cpp"

$(IntermediateDirectory)/src_mods_mod_logon.cpp$(ObjectSuffix): src/mods/mod_logon.cpp $(IntermediateDirectory)/src_mods_mod_logon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Test2/src/mods/mod_logon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_mods_mod_logon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_mods_mod_logon.cpp$(DependSuffix): src/mods/mod_logon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_mods_mod_logon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_mods_mod_logon.cpp$(DependSuffix) -MM "src/mods/mod_logon.cpp"

$(IntermediateDirectory)/src_mods_mod_logon.cpp$(PreprocessSuffix): src/mods/mod_logon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_mods_mod_logon.cpp$(PreprocessSuffix) "src/mods/mod_logon.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


