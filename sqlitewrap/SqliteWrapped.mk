##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=SqliteWrapped
ConfigurationName      :=Debug
WorkspacePath          :=/Users/admin/code/Oblivion2-XRM/osx
ProjectPath            :=/Users/admin/code/Oblivion2-XRM/sqlitewrap
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Admin
Date                   :=10/02/2018
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
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="SqliteWrapped.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)/usr/local/opt/sqlite/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)sqlitewrap $(LibraryPathSwitch)sqlitewrap/Debug $(LibraryPathSwitch)/usr/local/opt/sqlite/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := clang++
CC       := clang
CXXFLAGS :=  -Wfatal-errors -g -O0 -W -std=c++14 $(Preprocessors)
CFLAGS   :=  -Wfatal-errors -g -O0 -W -std=c++14 $(Preprocessors)
ASFLAGS  := 
AS       := llvm-as


##
## User defined environment variables
##
CodeLiteDir:=/Applications/codelite.app/Contents/SharedSupport/
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
	@$(MakeDirCommand) "/Users/admin/code/Oblivion2-XRM/osx/.build-debug"
	@echo rebuilt > "/Users/admin/code/Oblivion2-XRM/osx/.build-debug/SqliteWrapped"

PostBuild:
	@echo Executing Post Build commands ...
	cat IError.h StderrLog.h SysLogs.h Database.h Query.h > libSqliteWrapped.h
	
	@echo Done

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:
	@echo Executing Pre Build commands ...
	mkdir ./Debug
	cp ./SqliteWrapped.txt ./Debug/SqliteWrapped.txt
	
	@echo Done


##
## Objects
##
$(IntermediateDirectory)/Database.cpp$(ObjectSuffix): Database.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/sqlitewrap/Database.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Database.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Database.cpp$(PreprocessSuffix): Database.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Database.cpp$(PreprocessSuffix) Database.cpp

$(IntermediateDirectory)/Query.cpp$(ObjectSuffix): Query.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/sqlitewrap/Query.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Query.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Query.cpp$(PreprocessSuffix): Query.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Query.cpp$(PreprocessSuffix) Query.cpp

$(IntermediateDirectory)/StderrLog.cpp$(ObjectSuffix): StderrLog.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/sqlitewrap/StderrLog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/StderrLog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/StderrLog.cpp$(PreprocessSuffix): StderrLog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/StderrLog.cpp$(PreprocessSuffix) StderrLog.cpp

$(IntermediateDirectory)/SysLogs.cpp$(ObjectSuffix): SysLogs.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/sqlitewrap/SysLogs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SysLogs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SysLogs.cpp$(PreprocessSuffix): SysLogs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SysLogs.cpp$(PreprocessSuffix) SysLogs.cpp

##
## Clean
##
clean:
	$(RM) -r ./Debug/


