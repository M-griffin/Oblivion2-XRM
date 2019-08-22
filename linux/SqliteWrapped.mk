##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=SqliteWrapped
ConfigurationName      :=Debug
WorkspacePath          := "/home/merc/code/Oblivion2-XRM/linux"
ProjectPath            := "/home/merc/code/Oblivion2-XRM/linux"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Michae Griffin
Date                   :=22/08/19
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
CXXFLAGS :=  -Wfatal-errors -g -O0 -W -std=c++11 -Wall -static $(Preprocessors)
CFLAGS   :=  -Wfatal-errors -g -O0 -W -std=c++11 -Wall -static $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/sqlitewrap_Database.cpp$(ObjectSuffix) $(IntermediateDirectory)/sqlitewrap_Query.cpp$(ObjectSuffix) $(IntermediateDirectory)/sqlitewrap_StderrLog.cpp$(ObjectSuffix) $(IntermediateDirectory)/sqlitewrap_SysLogs.cpp$(ObjectSuffix) 



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
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "/home/merc/code/Oblivion2-XRM/linux/.build-debug"
	@echo rebuilt > "/home/merc/code/Oblivion2-XRM/linux/.build-debug/SqliteWrapped"

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
	sed -i 's+/home/merc/code/Oblivion2-XRM/+../+' SqliteWrapped.mk
	@echo Done


##
## Objects
##
$(IntermediateDirectory)/sqlitewrap_Database.cpp$(ObjectSuffix): ../sqlitewrap/Database.cpp $(IntermediateDirectory)/sqlitewrap_Database.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/sqlitewrap/Database.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sqlitewrap_Database.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sqlitewrap_Database.cpp$(DependSuffix): ../sqlitewrap/Database.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sqlitewrap_Database.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/sqlitewrap_Database.cpp$(DependSuffix) -MM "../sqlitewrap/Database.cpp"

$(IntermediateDirectory)/sqlitewrap_Database.cpp$(PreprocessSuffix): ../sqlitewrap/Database.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sqlitewrap_Database.cpp$(PreprocessSuffix) "../sqlitewrap/Database.cpp"

$(IntermediateDirectory)/sqlitewrap_Query.cpp$(ObjectSuffix): ../sqlitewrap/Query.cpp $(IntermediateDirectory)/sqlitewrap_Query.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/sqlitewrap/Query.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sqlitewrap_Query.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sqlitewrap_Query.cpp$(DependSuffix): ../sqlitewrap/Query.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sqlitewrap_Query.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/sqlitewrap_Query.cpp$(DependSuffix) -MM "../sqlitewrap/Query.cpp"

$(IntermediateDirectory)/sqlitewrap_Query.cpp$(PreprocessSuffix): ../sqlitewrap/Query.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sqlitewrap_Query.cpp$(PreprocessSuffix) "../sqlitewrap/Query.cpp"

$(IntermediateDirectory)/sqlitewrap_StderrLog.cpp$(ObjectSuffix): ../sqlitewrap/StderrLog.cpp $(IntermediateDirectory)/sqlitewrap_StderrLog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/sqlitewrap/StderrLog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sqlitewrap_StderrLog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sqlitewrap_StderrLog.cpp$(DependSuffix): ../sqlitewrap/StderrLog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sqlitewrap_StderrLog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/sqlitewrap_StderrLog.cpp$(DependSuffix) -MM "../sqlitewrap/StderrLog.cpp"

$(IntermediateDirectory)/sqlitewrap_StderrLog.cpp$(PreprocessSuffix): ../sqlitewrap/StderrLog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sqlitewrap_StderrLog.cpp$(PreprocessSuffix) "../sqlitewrap/StderrLog.cpp"

$(IntermediateDirectory)/sqlitewrap_SysLogs.cpp$(ObjectSuffix): ../sqlitewrap/SysLogs.cpp $(IntermediateDirectory)/sqlitewrap_SysLogs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/sqlitewrap/SysLogs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sqlitewrap_SysLogs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sqlitewrap_SysLogs.cpp$(DependSuffix): ../sqlitewrap/SysLogs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sqlitewrap_SysLogs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/sqlitewrap_SysLogs.cpp$(DependSuffix) -MM "../sqlitewrap/SysLogs.cpp"

$(IntermediateDirectory)/sqlitewrap_SysLogs.cpp$(PreprocessSuffix): ../sqlitewrap/SysLogs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sqlitewrap_SysLogs.cpp$(PreprocessSuffix) "../sqlitewrap/SysLogs.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


