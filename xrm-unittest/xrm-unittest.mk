##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=xrm-unittest
ConfigurationName      :=Debug
WorkspacePath          :=C:/Users/Blue/Desktop/Oblivion2-XRM/win32
ProjectPath            :=C:/Users/Blue/Desktop/Oblivion2-XRM/xrm-unittest
IntermediateDirectory  :=Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Blue
Date                   :=11/02/2018
CodeLitePath           :="C:/Program Files (x86)/CodeLite"
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
ObjectsFileList        :="xrm-unittest.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-5.1.0/mingw32/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../src $(IncludeSwitch)../sqlitewrap $(IncludeSwitch)C:/UnitTest++-1.3/src $(IncludeSwitch)C:/boost64/include $(IncludeSwitch)C:/sqlite3/include $(IncludeSwitch)C:/Users/Blue/Desktop/Test2/sqlitewrap $(IncludeSwitch)C:/OpenSSL-Win32/include $(IncludeSwitch)C:/Users/Blue/Documents/GitHub/yaml-cpp/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)boost_system-mgw51-mt-1_64.dll $(LibrarySwitch)boost_thread-mgw51-mt-1_64.dll $(LibrarySwitch)boost_chrono-mgw51-mt-1_64.dll $(LibrarySwitch)boost_serialization-mgw51-mt-1_64.dll $(LibrarySwitch)boost_filesystem-mgw51-mt-1_64.dll $(LibrarySwitch)boost_timer-mgw51-mt-1_64.dll $(LibrarySwitch)ws2_32 $(LibrarySwitch)wsock32 $(LibrarySwitch)SqliteWrapped $(LibrarySwitch)sqlite3 $(LibrarySwitch)UnitTest++ $(LibrarySwitch)ssleay32 $(LibrarySwitch)eay32 $(LibrarySwitch)yaml-cpp 
ArLibs                 :=  "libboost_system-mgw51-mt-1_64.dll.a" "libboost_thread-mgw51-mt-1_64.dll.a" "libboost_chrono-mgw51-mt-1_64.dll.a" "libboost_serialization-mgw51-mt-1_64.dll.a" "libboost_filesystem-mgw51-mt-1_64.dll.a" "libboost_timer-mgw51-mt-1_64.dll.a" "libws2_32.a" "libwsock32.a" "libSqliteWrapped.a" "libsqlite3.a" "libUnitTest++.a" "ssleay32.a" "libeay32.a" "yaml-cpp.dll" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)../src $(LibraryPathSwitch)../sqlitewrap/Debug $(LibraryPathSwitch)C:/boost64/lib $(LibraryPathSwitch)C:/msys/1.0/icu/dist/lib $(LibraryPathSwitch)C:/sqlite3/lib $(LibraryPathSwitch)C:/UnitTest++-1.3/Debug $(LibraryPathSwitch)C:/OpenSSL-Win32/lib/MinGW $(LibraryPathSwitch)C:/OpenSSL-Win32/lib/ $(LibraryPathSwitch)C:/Users/Blue/Documents/GitHub/yaml-cpp/build 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW-5.1.0/mingw32/bin/ar.exe rcu
CXX      := C:/MinGW-5.1.0/mingw32/bin/g++.exe
CC       := C:/MinGW-5.1.0/mingw32/bin/gcc.exe
CXXFLAGS :=  -g -std=c++11 -Wno-deprecated -Wno-unused-local-typedefs $(Preprocessors)
CFLAGS   :=  -g -std=c++11 -Wno-deprecated -Wno-unused-local-typedefs $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW-5.1.0/mingw32/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
Objects0=$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/ansi_processor_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/form_system_config_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/common_io_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix) $(IntermediateDirectory)/session_io_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/users_dao_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/oneliners_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/access_condition_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/file_area_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/message_area_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/conference_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) 



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


##
## Objects
##
$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix): ../src/data-app/oneliners_dao.cpp $(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/data-app/oneliners_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(DependSuffix): ../src/data-app/oneliners_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(DependSuffix) -MM ../src/data-app/oneliners_dao.cpp

$(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(PreprocessSuffix): ../src/data-app/oneliners_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-app_oneliners_dao.cpp$(PreprocessSuffix) ../src/data-app/oneliners_dao.cpp

$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix): ../src/forms/form_system_config.cpp $(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/forms/form_system_config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(DependSuffix): ../src/forms/form_system_config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(DependSuffix) -MM ../src/forms/form_system_config.cpp

$(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(PreprocessSuffix): ../src/forms/form_system_config.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_forms_form_system_config.cpp$(PreprocessSuffix) ../src/forms/form_system_config.cpp

$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix): ../src/data-sys/session_stats_dao.cpp $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/data-sys/session_stats_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(DependSuffix): ../src/data-sys/session_stats_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(DependSuffix) -MM ../src/data-sys/session_stats_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(PreprocessSuffix): ../src/data-sys/session_stats_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_session_stats_dao.cpp$(PreprocessSuffix) ../src/data-sys/session_stats_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix): ../src/data-sys/protocol_dao.cpp $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/data-sys/protocol_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(DependSuffix): ../src/data-sys/protocol_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(DependSuffix) -MM ../src/data-sys/protocol_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(PreprocessSuffix): ../src/data-sys/protocol_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_protocol_dao.cpp$(PreprocessSuffix) ../src/data-sys/protocol_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix): ../src/data-sys/users_dao.cpp $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/data-sys/users_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(DependSuffix): ../src/data-sys/users_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(DependSuffix) -MM ../src/data-sys/users_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(PreprocessSuffix): ../src/data-sys/users_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_users_dao.cpp$(PreprocessSuffix) ../src/data-sys/users_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix): ../src/data-sys/file_area_dao.cpp $(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/data-sys/file_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(DependSuffix): ../src/data-sys/file_area_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(DependSuffix) -MM ../src/data-sys/file_area_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/file_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_file_area_dao.cpp$(PreprocessSuffix) ../src/data-sys/file_area_dao.cpp

$(IntermediateDirectory)/ansi_processor_ut.cpp$(ObjectSuffix): ansi_processor_ut.cpp $(IntermediateDirectory)/ansi_processor_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/xrm-unittest/ansi_processor_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ansi_processor_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ansi_processor_ut.cpp$(DependSuffix): ansi_processor_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ansi_processor_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ansi_processor_ut.cpp$(DependSuffix) -MM ansi_processor_ut.cpp

$(IntermediateDirectory)/ansi_processor_ut.cpp$(PreprocessSuffix): ansi_processor_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ansi_processor_ut.cpp$(PreprocessSuffix) ansi_processor_ut.cpp

$(IntermediateDirectory)/form_system_config_ut.cpp$(ObjectSuffix): form_system_config_ut.cpp $(IntermediateDirectory)/form_system_config_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/xrm-unittest/form_system_config_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/form_system_config_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/form_system_config_ut.cpp$(DependSuffix): form_system_config_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/form_system_config_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/form_system_config_ut.cpp$(DependSuffix) -MM form_system_config_ut.cpp

$(IntermediateDirectory)/form_system_config_ut.cpp$(PreprocessSuffix): form_system_config_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/form_system_config_ut.cpp$(PreprocessSuffix) form_system_config_ut.cpp

$(IntermediateDirectory)/common_io_ut.cpp$(ObjectSuffix): common_io_ut.cpp $(IntermediateDirectory)/common_io_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/xrm-unittest/common_io_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/common_io_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/common_io_ut.cpp$(DependSuffix): common_io_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/common_io_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/common_io_ut.cpp$(DependSuffix) -MM common_io_ut.cpp

$(IntermediateDirectory)/common_io_ut.cpp$(PreprocessSuffix): common_io_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/common_io_ut.cpp$(PreprocessSuffix) common_io_ut.cpp

$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix): ../src/access_condition.cpp $(IntermediateDirectory)/up_src_access_condition.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/access_condition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_access_condition.cpp$(DependSuffix): ../src/access_condition.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_access_condition.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_access_condition.cpp$(DependSuffix) -MM ../src/access_condition.cpp

$(IntermediateDirectory)/up_src_access_condition.cpp$(PreprocessSuffix): ../src/access_condition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_access_condition.cpp$(PreprocessSuffix) ../src/access_condition.cpp

$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix): ../src/ansi_processor.cpp $(IntermediateDirectory)/up_src_ansi_processor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/ansi_processor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ansi_processor.cpp$(DependSuffix): ../src/ansi_processor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ansi_processor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ansi_processor.cpp$(DependSuffix) -MM ../src/ansi_processor.cpp

$(IntermediateDirectory)/up_src_ansi_processor.cpp$(PreprocessSuffix): ../src/ansi_processor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ansi_processor.cpp$(PreprocessSuffix) ../src/ansi_processor.cpp

$(IntermediateDirectory)/session_io_ut.cpp$(ObjectSuffix): session_io_ut.cpp $(IntermediateDirectory)/session_io_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/xrm-unittest/session_io_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/session_io_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/session_io_ut.cpp$(DependSuffix): session_io_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/session_io_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/session_io_ut.cpp$(DependSuffix) -MM session_io_ut.cpp

$(IntermediateDirectory)/session_io_ut.cpp$(PreprocessSuffix): session_io_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/session_io_ut.cpp$(PreprocessSuffix) session_io_ut.cpp

$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix): ../src/data-sys/db_startup.cpp $(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/data-sys/db_startup.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(DependSuffix): ../src/data-sys/db_startup.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(DependSuffix) -MM ../src/data-sys/db_startup.cpp

$(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(PreprocessSuffix): ../src/data-sys/db_startup.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_db_startup.cpp$(PreprocessSuffix) ../src/data-sys/db_startup.cpp

$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix): ../src/data-sys/conference_dao.cpp $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/data-sys/conference_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(DependSuffix): ../src/data-sys/conference_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(DependSuffix) -MM ../src/data-sys/conference_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(PreprocessSuffix): ../src/data-sys/conference_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_conference_dao.cpp$(PreprocessSuffix) ../src/data-sys/conference_dao.cpp

$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/xrm-unittest/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix): ../src/common_io.cpp $(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/common_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix): ../src/common_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix) -MM ../src/common_io.cpp

$(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix): ../src/common_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix) ../src/common_io.cpp

$(IntermediateDirectory)/users_dao_ut.cpp$(ObjectSuffix): users_dao_ut.cpp $(IntermediateDirectory)/users_dao_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/xrm-unittest/users_dao_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/users_dao_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/users_dao_ut.cpp$(DependSuffix): users_dao_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/users_dao_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/users_dao_ut.cpp$(DependSuffix) -MM users_dao_ut.cpp

$(IntermediateDirectory)/users_dao_ut.cpp$(PreprocessSuffix): users_dao_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/users_dao_ut.cpp$(PreprocessSuffix) users_dao_ut.cpp

$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix): ../src/session_io.cpp $(IntermediateDirectory)/up_src_session_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/session_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_session_io.cpp$(DependSuffix): ../src/session_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_session_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_session_io.cpp$(DependSuffix) -MM ../src/session_io.cpp

$(IntermediateDirectory)/up_src_session_io.cpp$(PreprocessSuffix): ../src/session_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_session_io.cpp$(PreprocessSuffix) ../src/session_io.cpp

$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix): ../src/session_manager.cpp $(IntermediateDirectory)/up_src_session_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/session_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_session_manager.cpp$(DependSuffix): ../src/session_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_session_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_session_manager.cpp$(DependSuffix) -MM ../src/session_manager.cpp

$(IntermediateDirectory)/up_src_session_manager.cpp$(PreprocessSuffix): ../src/session_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_session_manager.cpp$(PreprocessSuffix) ../src/session_manager.cpp

$(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix): ../src/form_manager.cpp $(IntermediateDirectory)/up_src_form_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/form_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_form_manager.cpp$(DependSuffix): ../src/form_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_form_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_form_manager.cpp$(DependSuffix) -MM ../src/form_manager.cpp

$(IntermediateDirectory)/up_src_form_manager.cpp$(PreprocessSuffix): ../src/form_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_form_manager.cpp$(PreprocessSuffix) ../src/form_manager.cpp

$(IntermediateDirectory)/oneliners_dao_it.cpp$(ObjectSuffix): oneliners_dao_it.cpp $(IntermediateDirectory)/oneliners_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/xrm-unittest/oneliners_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/oneliners_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/oneliners_dao_it.cpp$(DependSuffix): oneliners_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/oneliners_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/oneliners_dao_it.cpp$(DependSuffix) -MM oneliners_dao_it.cpp

$(IntermediateDirectory)/oneliners_dao_it.cpp$(PreprocessSuffix): oneliners_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/oneliners_dao_it.cpp$(PreprocessSuffix) oneliners_dao_it.cpp

$(IntermediateDirectory)/access_condition_ut.cpp$(ObjectSuffix): access_condition_ut.cpp $(IntermediateDirectory)/access_condition_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/xrm-unittest/access_condition_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/access_condition_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/access_condition_ut.cpp$(DependSuffix): access_condition_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/access_condition_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/access_condition_ut.cpp$(DependSuffix) -MM access_condition_ut.cpp

$(IntermediateDirectory)/access_condition_ut.cpp$(PreprocessSuffix): access_condition_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/access_condition_ut.cpp$(PreprocessSuffix) access_condition_ut.cpp

$(IntermediateDirectory)/file_area_dao_it.cpp$(ObjectSuffix): file_area_dao_it.cpp $(IntermediateDirectory)/file_area_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/xrm-unittest/file_area_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/file_area_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/file_area_dao_it.cpp$(DependSuffix): file_area_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/file_area_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/file_area_dao_it.cpp$(DependSuffix) -MM file_area_dao_it.cpp

$(IntermediateDirectory)/file_area_dao_it.cpp$(PreprocessSuffix): file_area_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/file_area_dao_it.cpp$(PreprocessSuffix) file_area_dao_it.cpp

$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix): ../src/data-sys/message_area_dao.cpp $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/data-sys/message_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(DependSuffix): ../src/data-sys/message_area_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(DependSuffix) -MM ../src/data-sys/message_area_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/message_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_message_area_dao.cpp$(PreprocessSuffix) ../src/data-sys/message_area_dao.cpp

$(IntermediateDirectory)/message_area_dao_it.cpp$(ObjectSuffix): message_area_dao_it.cpp $(IntermediateDirectory)/message_area_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/xrm-unittest/message_area_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/message_area_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/message_area_dao_it.cpp$(DependSuffix): message_area_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/message_area_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/message_area_dao_it.cpp$(DependSuffix) -MM message_area_dao_it.cpp

$(IntermediateDirectory)/message_area_dao_it.cpp$(PreprocessSuffix): message_area_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/message_area_dao_it.cpp$(PreprocessSuffix) message_area_dao_it.cpp

$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix): ../src/data-sys/security_dao.cpp $(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/data-sys/security_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(DependSuffix): ../src/data-sys/security_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(DependSuffix) -MM ../src/data-sys/security_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(PreprocessSuffix): ../src/data-sys/security_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_security_dao.cpp$(PreprocessSuffix) ../src/data-sys/security_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix): ../src/data-sys/config_dao.cpp $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/data-sys/config_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix): ../src/data-sys/config_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix) -MM ../src/data-sys/config_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix): ../src/data-sys/config_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix) ../src/data-sys/config_dao.cpp

$(IntermediateDirectory)/conference_dao_it.cpp$(ObjectSuffix): conference_dao_it.cpp $(IntermediateDirectory)/conference_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/xrm-unittest/conference_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/conference_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/conference_dao_it.cpp$(DependSuffix): conference_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/conference_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/conference_dao_it.cpp$(DependSuffix) -MM conference_dao_it.cpp

$(IntermediateDirectory)/conference_dao_it.cpp$(PreprocessSuffix): conference_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/conference_dao_it.cpp$(PreprocessSuffix) conference_dao_it.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix): ../src/data-sys/menu_dao.cpp $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/data-sys/menu_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(DependSuffix): ../src/data-sys/menu_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(DependSuffix) -MM ../src/data-sys/menu_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(PreprocessSuffix): ../src/data-sys/menu_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(PreprocessSuffix) ../src/data-sys/menu_dao.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r Debug/


