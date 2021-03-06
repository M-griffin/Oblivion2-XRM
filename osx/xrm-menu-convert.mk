##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=xrm-menu-convert
ConfigurationName      :=Debug
WorkspacePath          :=/Users/admin/code/Oblivion2-XRM/osx
ProjectPath            :=/Users/admin/code/Oblivion2-XRM/osx
IntermediateDirectory  :=./Debug
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
Preprocessors          :=$(PreprocessorSwitch)TARGET_OS_MAC 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="xrm-menu-convert.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../src $(IncludeSwitch)/usr/local/opt/boost/include $(IncludeSwitch)/usr/local/opt/yaml-cpp/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)boost_system-mt $(LibrarySwitch)boost_filesystem-mt $(LibrarySwitch)yaml-cpp 
ArLibs                 :=  "boost_system-mt" "boost_filesystem-mt" "yaml-cpp" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/usr/local/opt/boost/lib $(LibraryPathSwitch)/usr/local/opt/yaml-cpp/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := clang++
CC       := clang
CXXFLAGS :=  -Wfatal-errors -g -O0 -pedantic -W -std=c++11 $(Preprocessors)
CFLAGS   :=  -Wfatal-errors -g -O0 -pedantic -W -std=c++11 $(Preprocessors)
ASFLAGS  := 
AS       := llvm-as


##
## User defined environment variables
##
CodeLiteDir:=/Applications/codelite.app/Contents/SharedSupport/
Objects0=$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_xrm-menu-convert_src_menu_convert.cpp$(ObjectSuffix) 



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
	sed -i '' 's+/Users/admin/code/Oblivion2-XRM/+../+' xrm-menu-convert.mk
	@echo Done


##
## Objects
##
$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix): ../src/data-sys/text_prompts_dao.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/data-sys/text_prompts_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(PreprocessSuffix): ../src/data-sys/text_prompts_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(PreprocessSuffix) ../src/data-sys/text_prompts_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix): ../src/data-sys/config_dao.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/data-sys/config_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix): ../src/data-sys/config_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix) ../src/data-sys/config_dao.cpp

$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix): ../src/communicator.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/communicator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_communicator.cpp$(PreprocessSuffix): ../src/communicator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_communicator.cpp$(PreprocessSuffix) ../src/communicator.cpp

$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix): ../src/data-sys/menu_dao.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/data-sys/menu_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(PreprocessSuffix): ../src/data-sys/menu_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(PreprocessSuffix) ../src/data-sys/menu_dao.cpp

$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix): ../src/encoding.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/encoding.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_encoding.cpp$(PreprocessSuffix): ../src/encoding.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_encoding.cpp$(PreprocessSuffix) ../src/encoding.cpp

$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix): ../src/common_io.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/src/common_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix): ../src/common_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix) ../src/common_io.cpp

$(IntermediateDirectory)/up_xrm-menu-convert_src_menu_convert.cpp$(ObjectSuffix): ../xrm-menu-convert/src/menu_convert.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/xrm-menu-convert/src/menu_convert.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-menu-convert_src_menu_convert.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-menu-convert_src_menu_convert.cpp$(PreprocessSuffix): ../xrm-menu-convert/src/menu_convert.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-menu-convert_src_menu_convert.cpp$(PreprocessSuffix) ../xrm-menu-convert/src/menu_convert.cpp

##
## Clean
##
clean:
	$(RM) -r ./Debug/


