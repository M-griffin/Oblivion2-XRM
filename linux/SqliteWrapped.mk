##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=SqliteWrapped
ConfigurationName      :=Debug
WorkspacePath          :=../linux
ProjectPath            :=../linux
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=merc
Date                   :=20/05/23
CodeLitePath           :=/home/merc/.codelite
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
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="SqliteWrapped.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)sqlitewrap $(LibraryPathSwitch)sqlitewrap/Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -Wfatal-errors -g -O0 -W -std=c++11 -Wall -static -Wno-init-self $(Preprocessors)
CFLAGS   :=  -Wfatal-errors -g -O0 -W -std=c++11 -Wall -static -Wno-init-self $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList)
	@$(MakeDirCommand) "../linux/.build-debug"
	@echo rebuilt > "../linux/.build-debug/SqliteWrapped"

PostBuild:
	@echo Executing Post Build commands ...
	cat ../sqlitewrap/IError.h ../sqlitewrap/StderrLog.h ../sqlitewrap/SysLogs.h ../sqlitewrap/Database.h ../sqlitewrap/Query.h > ./Debug/libSqliteWrapped.h
	
	@echo Done

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:
	@echo Executing Pre Build commands ...
	sed -i 's+../+../+' SqliteWrapped.mk
	@echo Done


##
## Objects
##
$(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(ObjectSuffix): ../sqlitewrap/SysLogs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(DependSuffix) -MM ../sqlitewrap/SysLogs.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../sqlitewrap/SysLogs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(PreprocessSuffix): ../sqlitewrap/SysLogs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(PreprocessSuffix) ../sqlitewrap/SysLogs.cpp

$(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(ObjectSuffix): ../sqlitewrap/StderrLog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(DependSuffix) -MM ../sqlitewrap/StderrLog.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../sqlitewrap/StderrLog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(PreprocessSuffix): ../sqlitewrap/StderrLog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(PreprocessSuffix) ../sqlitewrap/StderrLog.cpp

$(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(ObjectSuffix): ../sqlitewrap/Query.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(DependSuffix) -MM ../sqlitewrap/Query.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../sqlitewrap/Query.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(PreprocessSuffix): ../sqlitewrap/Query.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(PreprocessSuffix) ../sqlitewrap/Query.cpp

$(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(ObjectSuffix): ../sqlitewrap/Database.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(DependSuffix) -MM ../sqlitewrap/Database.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "../sqlitewrap/Database.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(PreprocessSuffix): ../sqlitewrap/Database.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(PreprocessSuffix) ../sqlitewrap/Database.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


