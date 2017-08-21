##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=xrm-unittest
ConfigurationName      :=Debug
WorkspacePath          := "/home/merc/code/Oblivion2-XRM/linux"
ProjectPath            := "/home/merc/code/Oblivion2-XRM/xrm-unittest"
IntermediateDirectory  :=Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Michae Griffin
Date                   :=20/08/17
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
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="xrm-unittest.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../src $(IncludeSwitch)../sqlitewrap 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)UnitTest++ $(LibrarySwitch)boost_system $(LibrarySwitch)boost_thread $(LibrarySwitch)boost_chrono $(LibrarySwitch)boost_locale $(LibrarySwitch)boost_filesystem $(LibrarySwitch)boost_timer $(LibrarySwitch)boost_regex $(LibrarySwitch)boost_date_time $(LibrarySwitch)SqliteWrapped $(LibrarySwitch)sqlite3 $(LibrarySwitch)pthread $(LibrarySwitch)ssl $(LibrarySwitch)crypto $(LibrarySwitch)yaml-cpp 
ArLibs                 :=  "libUnitTest++.a" "boost_system" "boost_thread" "boost_chrono" "boost_locale" "boost_filesystem" "boost_timer" "boost_regex" "boost_date_time" "libSqliteWrapped.a" "sqlite3" "pthread" "ssl" "crypto" "yaml-cpp" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)$(UNIT_TEST_PP_SRC_DIR)/Debug $(LibraryPathSwitch)../sqlitewrap/Debug $(LibraryPathSwitch)../sqlitewrap $(LibraryPathSwitch)../src 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -std=c++11 -Wno-deprecated -Wno-unused-local-typedefs $(Preprocessors)
CFLAGS   :=  -g -std=c++11 -Wno-deprecated -Wno-unused-local-typedefs $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/common_io_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/session_io_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/ansi_processor_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_ansi_processor.cpp$(ObjectSuffix) $(IntermediateDirectory)/users_dao_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_session_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_session_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/access_condition_ut.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_access_condition.cpp$(ObjectSuffix) $(IntermediateDirectory)/form_system_config_ut.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_form_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/conference_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/file_area_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/message_area_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/oneliners_dao_it.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_config_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_conference_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_menu_dao.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/data-sys_file_area_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_message_area_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_access_level_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_protocol_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_security_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_users_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/forms_form_system_config.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-app_oneliners_dao.cpp$(ObjectSuffix) 



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


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/xrm-unittest/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/common_io_ut.cpp$(ObjectSuffix): common_io_ut.cpp $(IntermediateDirectory)/common_io_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/xrm-unittest/common_io_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/common_io_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/common_io_ut.cpp$(DependSuffix): common_io_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/common_io_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/common_io_ut.cpp$(DependSuffix) -MM "common_io_ut.cpp"

$(IntermediateDirectory)/common_io_ut.cpp$(PreprocessSuffix): common_io_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/common_io_ut.cpp$(PreprocessSuffix) "common_io_ut.cpp"

$(IntermediateDirectory)/session_io_ut.cpp$(ObjectSuffix): session_io_ut.cpp $(IntermediateDirectory)/session_io_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/xrm-unittest/session_io_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/session_io_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/session_io_ut.cpp$(DependSuffix): session_io_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/session_io_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/session_io_ut.cpp$(DependSuffix) -MM "session_io_ut.cpp"

$(IntermediateDirectory)/session_io_ut.cpp$(PreprocessSuffix): session_io_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/session_io_ut.cpp$(PreprocessSuffix) "session_io_ut.cpp"

$(IntermediateDirectory)/ansi_processor_ut.cpp$(ObjectSuffix): ansi_processor_ut.cpp $(IntermediateDirectory)/ansi_processor_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/xrm-unittest/ansi_processor_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ansi_processor_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ansi_processor_ut.cpp$(DependSuffix): ansi_processor_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ansi_processor_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ansi_processor_ut.cpp$(DependSuffix) -MM "ansi_processor_ut.cpp"

$(IntermediateDirectory)/ansi_processor_ut.cpp$(PreprocessSuffix): ansi_processor_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ansi_processor_ut.cpp$(PreprocessSuffix) "ansi_processor_ut.cpp"

$(IntermediateDirectory)/src_ansi_processor.cpp$(ObjectSuffix): ../src/ansi_processor.cpp $(IntermediateDirectory)/src_ansi_processor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/ansi_processor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ansi_processor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ansi_processor.cpp$(DependSuffix): ../src/ansi_processor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_ansi_processor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_ansi_processor.cpp$(DependSuffix) -MM "../src/ansi_processor.cpp"

$(IntermediateDirectory)/src_ansi_processor.cpp$(PreprocessSuffix): ../src/ansi_processor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ansi_processor.cpp$(PreprocessSuffix) "../src/ansi_processor.cpp"

$(IntermediateDirectory)/users_dao_ut.cpp$(ObjectSuffix): users_dao_ut.cpp $(IntermediateDirectory)/users_dao_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/xrm-unittest/users_dao_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/users_dao_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/users_dao_ut.cpp$(DependSuffix): users_dao_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/users_dao_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/users_dao_ut.cpp$(DependSuffix) -MM "users_dao_ut.cpp"

$(IntermediateDirectory)/users_dao_ut.cpp$(PreprocessSuffix): users_dao_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/users_dao_ut.cpp$(PreprocessSuffix) "users_dao_ut.cpp"

$(IntermediateDirectory)/src_session_manager.cpp$(ObjectSuffix): ../src/session_manager.cpp $(IntermediateDirectory)/src_session_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/session_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_session_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_session_manager.cpp$(DependSuffix): ../src/session_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_session_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_session_manager.cpp$(DependSuffix) -MM "../src/session_manager.cpp"

$(IntermediateDirectory)/src_session_manager.cpp$(PreprocessSuffix): ../src/session_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_session_manager.cpp$(PreprocessSuffix) "../src/session_manager.cpp"

$(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix): ../src/common_io.cpp $(IntermediateDirectory)/src_common_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/common_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_common_io.cpp$(DependSuffix): ../src/common_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_common_io.cpp$(DependSuffix) -MM "../src/common_io.cpp"

$(IntermediateDirectory)/src_common_io.cpp$(PreprocessSuffix): ../src/common_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_common_io.cpp$(PreprocessSuffix) "../src/common_io.cpp"

$(IntermediateDirectory)/src_session_io.cpp$(ObjectSuffix): ../src/session_io.cpp $(IntermediateDirectory)/src_session_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/session_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_session_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_session_io.cpp$(DependSuffix): ../src/session_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_session_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_session_io.cpp$(DependSuffix) -MM "../src/session_io.cpp"

$(IntermediateDirectory)/src_session_io.cpp$(PreprocessSuffix): ../src/session_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_session_io.cpp$(PreprocessSuffix) "../src/session_io.cpp"

$(IntermediateDirectory)/access_condition_ut.cpp$(ObjectSuffix): access_condition_ut.cpp $(IntermediateDirectory)/access_condition_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/xrm-unittest/access_condition_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/access_condition_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/access_condition_ut.cpp$(DependSuffix): access_condition_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/access_condition_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/access_condition_ut.cpp$(DependSuffix) -MM "access_condition_ut.cpp"

$(IntermediateDirectory)/access_condition_ut.cpp$(PreprocessSuffix): access_condition_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/access_condition_ut.cpp$(PreprocessSuffix) "access_condition_ut.cpp"

$(IntermediateDirectory)/src_access_condition.cpp$(ObjectSuffix): ../src/access_condition.cpp $(IntermediateDirectory)/src_access_condition.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/access_condition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_access_condition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_access_condition.cpp$(DependSuffix): ../src/access_condition.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_access_condition.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_access_condition.cpp$(DependSuffix) -MM "../src/access_condition.cpp"

$(IntermediateDirectory)/src_access_condition.cpp$(PreprocessSuffix): ../src/access_condition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_access_condition.cpp$(PreprocessSuffix) "../src/access_condition.cpp"

$(IntermediateDirectory)/form_system_config_ut.cpp$(ObjectSuffix): form_system_config_ut.cpp $(IntermediateDirectory)/form_system_config_ut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/xrm-unittest/form_system_config_ut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/form_system_config_ut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/form_system_config_ut.cpp$(DependSuffix): form_system_config_ut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/form_system_config_ut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/form_system_config_ut.cpp$(DependSuffix) -MM "form_system_config_ut.cpp"

$(IntermediateDirectory)/form_system_config_ut.cpp$(PreprocessSuffix): form_system_config_ut.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/form_system_config_ut.cpp$(PreprocessSuffix) "form_system_config_ut.cpp"

$(IntermediateDirectory)/src_form_manager.cpp$(ObjectSuffix): ../src/form_manager.cpp $(IntermediateDirectory)/src_form_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/form_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_form_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_form_manager.cpp$(DependSuffix): ../src/form_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_form_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_form_manager.cpp$(DependSuffix) -MM "../src/form_manager.cpp"

$(IntermediateDirectory)/src_form_manager.cpp$(PreprocessSuffix): ../src/form_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_form_manager.cpp$(PreprocessSuffix) "../src/form_manager.cpp"

$(IntermediateDirectory)/conference_dao_it.cpp$(ObjectSuffix): conference_dao_it.cpp $(IntermediateDirectory)/conference_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/xrm-unittest/conference_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/conference_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/conference_dao_it.cpp$(DependSuffix): conference_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/conference_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/conference_dao_it.cpp$(DependSuffix) -MM "conference_dao_it.cpp"

$(IntermediateDirectory)/conference_dao_it.cpp$(PreprocessSuffix): conference_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/conference_dao_it.cpp$(PreprocessSuffix) "conference_dao_it.cpp"

$(IntermediateDirectory)/file_area_dao_it.cpp$(ObjectSuffix): file_area_dao_it.cpp $(IntermediateDirectory)/file_area_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/xrm-unittest/file_area_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/file_area_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/file_area_dao_it.cpp$(DependSuffix): file_area_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/file_area_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/file_area_dao_it.cpp$(DependSuffix) -MM "file_area_dao_it.cpp"

$(IntermediateDirectory)/file_area_dao_it.cpp$(PreprocessSuffix): file_area_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/file_area_dao_it.cpp$(PreprocessSuffix) "file_area_dao_it.cpp"

$(IntermediateDirectory)/message_area_dao_it.cpp$(ObjectSuffix): message_area_dao_it.cpp $(IntermediateDirectory)/message_area_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/xrm-unittest/message_area_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/message_area_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/message_area_dao_it.cpp$(DependSuffix): message_area_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/message_area_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/message_area_dao_it.cpp$(DependSuffix) -MM "message_area_dao_it.cpp"

$(IntermediateDirectory)/message_area_dao_it.cpp$(PreprocessSuffix): message_area_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/message_area_dao_it.cpp$(PreprocessSuffix) "message_area_dao_it.cpp"

$(IntermediateDirectory)/oneliners_dao_it.cpp$(ObjectSuffix): oneliners_dao_it.cpp $(IntermediateDirectory)/oneliners_dao_it.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/xrm-unittest/oneliners_dao_it.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/oneliners_dao_it.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/oneliners_dao_it.cpp$(DependSuffix): oneliners_dao_it.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/oneliners_dao_it.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/oneliners_dao_it.cpp$(DependSuffix) -MM "oneliners_dao_it.cpp"

$(IntermediateDirectory)/oneliners_dao_it.cpp$(PreprocessSuffix): oneliners_dao_it.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/oneliners_dao_it.cpp$(PreprocessSuffix) "oneliners_dao_it.cpp"

$(IntermediateDirectory)/data-sys_config_dao.cpp$(ObjectSuffix): ../src/data-sys/config_dao.cpp $(IntermediateDirectory)/data-sys_config_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/data-sys/config_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_config_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_config_dao.cpp$(DependSuffix): ../src/data-sys/config_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_config_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_config_dao.cpp$(DependSuffix) -MM "../src/data-sys/config_dao.cpp"

$(IntermediateDirectory)/data-sys_config_dao.cpp$(PreprocessSuffix): ../src/data-sys/config_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_config_dao.cpp$(PreprocessSuffix) "../src/data-sys/config_dao.cpp"

$(IntermediateDirectory)/data-sys_conference_dao.cpp$(ObjectSuffix): ../src/data-sys/conference_dao.cpp $(IntermediateDirectory)/data-sys_conference_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/data-sys/conference_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_conference_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_conference_dao.cpp$(DependSuffix): ../src/data-sys/conference_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_conference_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_conference_dao.cpp$(DependSuffix) -MM "../src/data-sys/conference_dao.cpp"

$(IntermediateDirectory)/data-sys_conference_dao.cpp$(PreprocessSuffix): ../src/data-sys/conference_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_conference_dao.cpp$(PreprocessSuffix) "../src/data-sys/conference_dao.cpp"

$(IntermediateDirectory)/data-sys_menu_dao.cpp$(ObjectSuffix): ../src/data-sys/menu_dao.cpp $(IntermediateDirectory)/data-sys_menu_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/data-sys/menu_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_menu_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_menu_dao.cpp$(DependSuffix): ../src/data-sys/menu_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_menu_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_menu_dao.cpp$(DependSuffix) -MM "../src/data-sys/menu_dao.cpp"

$(IntermediateDirectory)/data-sys_menu_dao.cpp$(PreprocessSuffix): ../src/data-sys/menu_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_menu_dao.cpp$(PreprocessSuffix) "../src/data-sys/menu_dao.cpp"

$(IntermediateDirectory)/data-sys_file_area_dao.cpp$(ObjectSuffix): ../src/data-sys/file_area_dao.cpp $(IntermediateDirectory)/data-sys_file_area_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/data-sys/file_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_file_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_file_area_dao.cpp$(DependSuffix): ../src/data-sys/file_area_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_file_area_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_file_area_dao.cpp$(DependSuffix) -MM "../src/data-sys/file_area_dao.cpp"

$(IntermediateDirectory)/data-sys_file_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/file_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_file_area_dao.cpp$(PreprocessSuffix) "../src/data-sys/file_area_dao.cpp"

$(IntermediateDirectory)/data-sys_message_area_dao.cpp$(ObjectSuffix): ../src/data-sys/message_area_dao.cpp $(IntermediateDirectory)/data-sys_message_area_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/data-sys/message_area_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_message_area_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_message_area_dao.cpp$(DependSuffix): ../src/data-sys/message_area_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_message_area_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_message_area_dao.cpp$(DependSuffix) -MM "../src/data-sys/message_area_dao.cpp"

$(IntermediateDirectory)/data-sys_message_area_dao.cpp$(PreprocessSuffix): ../src/data-sys/message_area_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_message_area_dao.cpp$(PreprocessSuffix) "../src/data-sys/message_area_dao.cpp"

$(IntermediateDirectory)/data-sys_access_level_dao.cpp$(ObjectSuffix): ../src/data-sys/access_level_dao.cpp $(IntermediateDirectory)/data-sys_access_level_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/data-sys/access_level_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_access_level_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_access_level_dao.cpp$(DependSuffix): ../src/data-sys/access_level_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_access_level_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_access_level_dao.cpp$(DependSuffix) -MM "../src/data-sys/access_level_dao.cpp"

$(IntermediateDirectory)/data-sys_access_level_dao.cpp$(PreprocessSuffix): ../src/data-sys/access_level_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_access_level_dao.cpp$(PreprocessSuffix) "../src/data-sys/access_level_dao.cpp"

$(IntermediateDirectory)/data-sys_protocol_dao.cpp$(ObjectSuffix): ../src/data-sys/protocol_dao.cpp $(IntermediateDirectory)/data-sys_protocol_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/data-sys/protocol_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_protocol_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_protocol_dao.cpp$(DependSuffix): ../src/data-sys/protocol_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_protocol_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_protocol_dao.cpp$(DependSuffix) -MM "../src/data-sys/protocol_dao.cpp"

$(IntermediateDirectory)/data-sys_protocol_dao.cpp$(PreprocessSuffix): ../src/data-sys/protocol_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_protocol_dao.cpp$(PreprocessSuffix) "../src/data-sys/protocol_dao.cpp"

$(IntermediateDirectory)/data-sys_security_dao.cpp$(ObjectSuffix): ../src/data-sys/security_dao.cpp $(IntermediateDirectory)/data-sys_security_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/data-sys/security_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_security_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_security_dao.cpp$(DependSuffix): ../src/data-sys/security_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_security_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_security_dao.cpp$(DependSuffix) -MM "../src/data-sys/security_dao.cpp"

$(IntermediateDirectory)/data-sys_security_dao.cpp$(PreprocessSuffix): ../src/data-sys/security_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_security_dao.cpp$(PreprocessSuffix) "../src/data-sys/security_dao.cpp"

$(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(ObjectSuffix): ../src/data-sys/session_stats_dao.cpp $(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/data-sys/session_stats_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(DependSuffix): ../src/data-sys/session_stats_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(DependSuffix) -MM "../src/data-sys/session_stats_dao.cpp"

$(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(PreprocessSuffix): ../src/data-sys/session_stats_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_session_stats_dao.cpp$(PreprocessSuffix) "../src/data-sys/session_stats_dao.cpp"

$(IntermediateDirectory)/data-sys_users_dao.cpp$(ObjectSuffix): ../src/data-sys/users_dao.cpp $(IntermediateDirectory)/data-sys_users_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/data-sys/users_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_users_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_users_dao.cpp$(DependSuffix): ../src/data-sys/users_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_users_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_users_dao.cpp$(DependSuffix) -MM "../src/data-sys/users_dao.cpp"

$(IntermediateDirectory)/data-sys_users_dao.cpp$(PreprocessSuffix): ../src/data-sys/users_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_users_dao.cpp$(PreprocessSuffix) "../src/data-sys/users_dao.cpp"

$(IntermediateDirectory)/forms_form_system_config.cpp$(ObjectSuffix): ../src/forms/form_system_config.cpp $(IntermediateDirectory)/forms_form_system_config.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/forms/form_system_config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/forms_form_system_config.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/forms_form_system_config.cpp$(DependSuffix): ../src/forms/form_system_config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/forms_form_system_config.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/forms_form_system_config.cpp$(DependSuffix) -MM "../src/forms/form_system_config.cpp"

$(IntermediateDirectory)/forms_form_system_config.cpp$(PreprocessSuffix): ../src/forms/form_system_config.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/forms_form_system_config.cpp$(PreprocessSuffix) "../src/forms/form_system_config.cpp"

$(IntermediateDirectory)/data-app_oneliners_dao.cpp$(ObjectSuffix): ../src/data-app/oneliners_dao.cpp $(IntermediateDirectory)/data-app_oneliners_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/src/data-app/oneliners_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-app_oneliners_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-app_oneliners_dao.cpp$(DependSuffix): ../src/data-app/oneliners_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-app_oneliners_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-app_oneliners_dao.cpp$(DependSuffix) -MM "../src/data-app/oneliners_dao.cpp"

$(IntermediateDirectory)/data-app_oneliners_dao.cpp$(PreprocessSuffix): ../src/data-app/oneliners_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-app_oneliners_dao.cpp$(PreprocessSuffix) "../src/data-app/oneliners_dao.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r Debug/


