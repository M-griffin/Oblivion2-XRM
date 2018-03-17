##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=SqliteWrapped
ConfigurationName      :=Debug
WorkspacePath          := "/home/merc/code/Oblivion2-XRM/linux"
ProjectPath            := "/home/merc/code/Oblivion2-XRM/sqlitewrap"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Michae Griffin
Date                   :=17/03/18
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
CXXFLAGS :=  -Wfatal-errors -g -O0 -W -std=c++14 -Wall -static $(Preprocessors)
CFLAGS   :=  -Wfatal-errors -g -O0 -W -std=c++14 -Wall -static $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/Database.cpp$(ObjectSuffix) $(IntermediateDirectory)/Query.cpp$(ObjectSuffix) $(IntermediateDirectory)/StderrLog.cpp$(ObjectSuffix) $(IntermediateDirectory)/SysLogs.cpp$(ObjectSuffix) 



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
	cat IError.h StderrLog.h SysLogs.h Database.h Query.h > libSqliteWrapped.h
	
	@echo Done

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/Database.cpp$(ObjectSuffix): Database.cpp $(IntermediateDirectory)/Database.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/sqlitewrap/Database.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Database.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Database.cpp$(DependSuffix): Database.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Database.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Database.cpp$(DependSuffix) -MM "Database.cpp"

$(IntermediateDirectory)/Database.cpp$(PreprocessSuffix): Database.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Database.cpp$(PreprocessSuffix) "Database.cpp"

$(IntermediateDirectory)/Query.cpp$(ObjectSuffix): Query.cpp $(IntermediateDirectory)/Query.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/sqlitewrap/Query.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Query.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Query.cpp$(DependSuffix): Query.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Query.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Query.cpp$(DependSuffix) -MM "Query.cpp"

$(IntermediateDirectory)/Query.cpp$(PreprocessSuffix): Query.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Query.cpp$(PreprocessSuffix) "Query.cpp"

$(IntermediateDirectory)/StderrLog.cpp$(ObjectSuffix): StderrLog.cpp $(IntermediateDirectory)/StderrLog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/sqlitewrap/StderrLog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/StderrLog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/StderrLog.cpp$(DependSuffix): StderrLog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/StderrLog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/StderrLog.cpp$(DependSuffix) -MM "StderrLog.cpp"

$(IntermediateDirectory)/StderrLog.cpp$(PreprocessSuffix): StderrLog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/StderrLog.cpp$(PreprocessSuffix) "StderrLog.cpp"

$(IntermediateDirectory)/SysLogs.cpp$(ObjectSuffix): SysLogs.cpp $(IntermediateDirectory)/SysLogs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/Oblivion2-XRM/sqlitewrap/SysLogs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SysLogs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SysLogs.cpp$(DependSuffix): SysLogs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SysLogs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SysLogs.cpp$(DependSuffix) -MM "SysLogs.cpp"

$(IntermediateDirectory)/SysLogs.cpp$(PreprocessSuffix): SysLogs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SysLogs.cpp$(PreprocessSuffix) "SysLogs.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


