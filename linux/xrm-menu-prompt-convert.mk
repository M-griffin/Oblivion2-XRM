##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=xrm-menu-prompt-convert
ConfigurationName      :=Debug
WorkspacePath          := "../linux"
ProjectPath            := "../linux"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Michae Griffin
Date                   :=14/07/19
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
ObjectsFileList        :="xrm-menu-prompt-convert.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../src $(IncludeSwitch)../linux/Debug 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)boost_system $(LibrarySwitch)boost_filesystem $(LibrarySwitch)yaml-cpp $(LibrarySwitch)uuid 
ArLibs                 :=  "boost_system" "boost_filesystem" "yaml-cpp" "uuid" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)../src $(LibraryPathSwitch)../linux/Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -Wfatal-errors -g -O0 -pedantic -W -std=c++11 -Wall -Wno-deprecated -Wno-unused-local-typedefs $(Preprocessors)
CFLAGS   :=  -Wfatal-errors -g -O0 -pedantic -W -std=c++11 -Wall -Wno-deprecated -Wno-unused-local-typedefs $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_encoding.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_communicator.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_prompt_convert.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_config_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_menu_prompt_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(ObjectSuffix) 



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
	sed -i 's+../+../+' xrm-menu-prompt-convert.mk
	@echo Done


##
## Objects
##
$(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix): ../src/common_io.cpp $(IntermediateDirectory)/src_common_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/common_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_common_io.cpp$(DependSuffix): ../src/common_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_common_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_common_io.cpp$(DependSuffix) -MM "../src/common_io.cpp"

$(IntermediateDirectory)/src_common_io.cpp$(PreprocessSuffix): ../src/common_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_common_io.cpp$(PreprocessSuffix) "../src/common_io.cpp"

$(IntermediateDirectory)/src_encoding.cpp$(ObjectSuffix): ../src/encoding.cpp $(IntermediateDirectory)/src_encoding.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/encoding.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_encoding.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_encoding.cpp$(DependSuffix): ../src/encoding.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_encoding.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_encoding.cpp$(DependSuffix) -MM "../src/encoding.cpp"

$(IntermediateDirectory)/src_encoding.cpp$(PreprocessSuffix): ../src/encoding.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_encoding.cpp$(PreprocessSuffix) "../src/encoding.cpp"

$(IntermediateDirectory)/src_communicator.cpp$(ObjectSuffix): ../src/communicator.cpp $(IntermediateDirectory)/src_communicator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/communicator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_communicator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_communicator.cpp$(DependSuffix): ../src/communicator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_communicator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_communicator.cpp$(DependSuffix) -MM "../src/communicator.cpp"

$(IntermediateDirectory)/src_communicator.cpp$(PreprocessSuffix): ../src/communicator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_communicator.cpp$(PreprocessSuffix) "../src/communicator.cpp"

$(IntermediateDirectory)/src_prompt_convert.cpp$(ObjectSuffix): ../xrm-menu-prompt-convert/src/prompt_convert.cpp $(IntermediateDirectory)/src_prompt_convert.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../xrm-menu-prompt-convert/src/prompt_convert.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_prompt_convert.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_prompt_convert.cpp$(DependSuffix): ../xrm-menu-prompt-convert/src/prompt_convert.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_prompt_convert.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_prompt_convert.cpp$(DependSuffix) -MM "../xrm-menu-prompt-convert/src/prompt_convert.cpp"

$(IntermediateDirectory)/src_prompt_convert.cpp$(PreprocessSuffix): ../xrm-menu-prompt-convert/src/prompt_convert.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_prompt_convert.cpp$(PreprocessSuffix) "../xrm-menu-prompt-convert/src/prompt_convert.cpp"

$(IntermediateDirectory)/data-sys_config_dao.cpp$(ObjectSuffix): ../src/data-sys/config_dao.cpp $(IntermediateDirectory)/data-sys_config_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/config_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_config_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_config_dao.cpp$(DependSuffix): ../src/data-sys/config_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_config_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_config_dao.cpp$(DependSuffix) -MM "../src/data-sys/config_dao.cpp"

$(IntermediateDirectory)/data-sys_config_dao.cpp$(PreprocessSuffix): ../src/data-sys/config_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_config_dao.cpp$(PreprocessSuffix) "../src/data-sys/config_dao.cpp"

$(IntermediateDirectory)/data-sys_menu_prompt_dao.cpp$(ObjectSuffix): ../src/data-sys/menu_prompt_dao.cpp $(IntermediateDirectory)/data-sys_menu_prompt_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/menu_prompt_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_menu_prompt_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_menu_prompt_dao.cpp$(DependSuffix): ../src/data-sys/menu_prompt_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_menu_prompt_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_menu_prompt_dao.cpp$(DependSuffix) -MM "../src/data-sys/menu_prompt_dao.cpp"

$(IntermediateDirectory)/data-sys_menu_prompt_dao.cpp$(PreprocessSuffix): ../src/data-sys/menu_prompt_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_menu_prompt_dao.cpp$(PreprocessSuffix) "../src/data-sys/menu_prompt_dao.cpp"

$(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(ObjectSuffix): ../src/data-sys/text_prompts_dao.cpp $(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../src/data-sys/text_prompts_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(DependSuffix): ../src/data-sys/text_prompts_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(DependSuffix) -MM "../src/data-sys/text_prompts_dao.cpp"

$(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(PreprocessSuffix): ../src/data-sys/text_prompts_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-sys_text_prompts_dao.cpp$(PreprocessSuffix) "../src/data-sys/text_prompts_dao.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


