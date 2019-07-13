##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=xrm-menu-convert
ConfigurationName      :=Debug
WorkspacePath          :=C:/Users/Blue/Desktop/Oblivion2-XRM/win32
ProjectPath            :=C:/Users/Blue/Desktop/Oblivion2-XRM/win32
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Blue
Date                   :=13/07/2019
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
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="xrm-menu-convert.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-5.1.0/mingw32/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../src $(IncludeSwitch)C:/boost64 $(IncludeSwitch)C:/boost64/include $(IncludeSwitch)C:/Users/Blue/Documents/GitHub/yaml-cpp/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)boost_system-mgw51-mt-1_64.dll $(LibrarySwitch)boost_thread-mgw51-mt-1_64.dll $(LibrarySwitch)boost_chrono-mgw51-mt-1_64.dll $(LibrarySwitch)boost_serialization-mgw51-mt-1_64.dll $(LibrarySwitch)boost_locale-mgw51-mt-1_64.dll $(LibrarySwitch)boost_filesystem-mgw51-mt-1_64.dll $(LibrarySwitch)boost_timer-mgw51-mt-1_64.dll $(LibrarySwitch)boost_regex-mgw51-mt-1_64.dll $(LibrarySwitch)iconv $(LibrarySwitch)icuuc.dll $(LibrarySwitch)icuin.dll $(LibrarySwitch)yaml-cpp $(LibrarySwitch)Rpcrt4 
ArLibs                 :=  "libboost_system-mgw51-mt-1_64.dll.a" "libboost_thread-mgw51-mt-1_64.dll.a" "libboost_chrono-mgw51-mt-1_64.dll.a" "libboost_serialization-mgw51-mt-1_64.dll.a" "libboost_locale-mgw51-mt-1_64.dll.a" "libboost_filesystem-mgw51-mt-1_64.dll.a" "libboost_timer-mgw51-mt-1_64.dll.a" "libboost_regex-mgw51-mt-1_64.dll.a" "libiconv.a" "libicuuc.dll.a" "libicuin.dll.a" "yaml-cpp.dll" "Rpcrt4" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)C:/boost64/lib $(LibraryPathSwitch)C:/msys/1.0/icu/dist/lib $(LibraryPathSwitch)C:/Users/Blue/Documents/GitHub/yaml-cpp/build 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW-5.1.0/mingw32/bin/ar.exe rcu
CXX      := C:/MinGW-5.1.0/mingw32/bin/g++.exe
CC       := C:/MinGW-5.1.0/mingw32/bin/gcc.exe
CXXFLAGS :=  -Wfatal-errors -g -O0 -pedantic -W -std=c++14 -Wall -Wno-deprecated -Wno-unused-local-typedefs -isysrootC:\MinGW-5.1.0\include $(Preprocessors)
CFLAGS   :=  -Wfatal-errors -g -O0 -pedantic -W -std=c++14 -Wall -Wno-deprecated -Wno-unused-local-typedefs $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW-5.1.0/mingw32/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/up_xrm-menu-convert_src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data-sys_menu_dao.cpp$(ObjectSuffix) 



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
$(IntermediateDirectory)/up_xrm-menu-convert_src_main.cpp$(ObjectSuffix): ../xrm-menu-convert/src/main.cpp $(IntermediateDirectory)/up_xrm-menu-convert_src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/xrm-menu-convert/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_xrm-menu-convert_src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_xrm-menu-convert_src_main.cpp$(DependSuffix): ../xrm-menu-convert/src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_xrm-menu-convert_src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_xrm-menu-convert_src_main.cpp$(DependSuffix) -MM ../xrm-menu-convert/src/main.cpp

$(IntermediateDirectory)/up_xrm-menu-convert_src_main.cpp$(PreprocessSuffix): ../xrm-menu-convert/src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_xrm-menu-convert_src_main.cpp$(PreprocessSuffix) ../xrm-menu-convert/src/main.cpp

$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix): ../src/communicator.cpp $(IntermediateDirectory)/up_src_communicator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/communicator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_communicator.cpp$(DependSuffix): ../src/communicator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_communicator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_communicator.cpp$(DependSuffix) -MM ../src/communicator.cpp

$(IntermediateDirectory)/up_src_communicator.cpp$(PreprocessSuffix): ../src/communicator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_communicator.cpp$(PreprocessSuffix) ../src/communicator.cpp

$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix): ../src/encoding.cpp $(IntermediateDirectory)/up_src_encoding.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/encoding.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_encoding.cpp$(DependSuffix): ../src/encoding.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_encoding.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_encoding.cpp$(DependSuffix) -MM ../src/encoding.cpp

$(IntermediateDirectory)/up_src_encoding.cpp$(PreprocessSuffix): ../src/encoding.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_encoding.cpp$(PreprocessSuffix) ../src/encoding.cpp

$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix): ../src/common_io.cpp $(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/common_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix): ../src/common_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix) -MM ../src/common_io.cpp

$(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix): ../src/common_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix) ../src/common_io.cpp

$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix): ../src/data-sys/text_prompts_dao.cpp $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/data-sys/text_prompts_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(DependSuffix): ../src/data-sys/text_prompts_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(DependSuffix) -MM ../src/data-sys/text_prompts_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(PreprocessSuffix): ../src/data-sys/text_prompts_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_text_prompts_dao.cpp$(PreprocessSuffix) ../src/data-sys/text_prompts_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix): ../src/data-sys/config_dao.cpp $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/data-sys/config_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix): ../src/data-sys/config_dao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(DependSuffix) -MM ../src/data-sys/config_dao.cpp

$(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix): ../src/data-sys/config_dao.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data-sys_config_dao.cpp$(PreprocessSuffix) ../src/data-sys/config_dao.cpp

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
	$(RM) -r ./Debug/


