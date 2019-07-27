##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=SqliteWrapped
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
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="SqliteWrapped.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch).. $(IncludeSwitch)./Debug $(IncludeSwitch)/usr/local/opt/sqlite/include 
IncludePCH             := 
RcIncludePath          := $(IncludeSwitch). $(IncludeSwitch).. $(IncludeSwitch)./Debug 
Libs                   := $(LibrarySwitch)sqlite3 
ArLibs                 :=  "sqlite3" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch).. $(LibraryPathSwitch)./Debug $(LibraryPathSwitch)/usr/local/opt/sqlite/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := clang++
CC       := clang
CXXFLAGS :=  -Wfatal-errors -g -O0 -std=c++11 $(Preprocessors)
CFLAGS   :=  -Wfatal-errors -g -O0 -std=c++11 $(Preprocessors)
ASFLAGS  := 
AS       := llvm-as


##
## User defined environment variables
##
CodeLiteDir:=/Applications/codelite.app/Contents/SharedSupport/
Objects0=$(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(ObjectSuffix) 



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
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)
	@$(MakeDirCommand) "/Users/admin/code/Oblivion2-XRM/osx/.build-debug"
	@echo rebuilt > "/Users/admin/code/Oblivion2-XRM/osx/.build-debug/SqliteWrapped"

PostBuild:
	@echo Executing Post Build commands ...
	cat ../sqlitewrap/IError.h ../sqlitewrap/StderrLog.h ../sqlitewrap/SysLogs.h ../sqlitewrap/Database.h ../sqlitewrap/Query.h > ./Debug/libSqliteWrapped.h
	@echo Done

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:
	@echo Executing Pre Build commands ...
	sed -i '' 's+/Users/admin/code/Oblivion2-XRM/+../+' SqliteWrapped.mk
	@echo Done


##
## Objects
##
$(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(ObjectSuffix): ../sqlitewrap/Database.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/sqlitewrap/Database.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(PreprocessSuffix): ../sqlitewrap/Database.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(PreprocessSuffix) ../sqlitewrap/Database.cpp

$(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(ObjectSuffix): ../sqlitewrap/Query.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/sqlitewrap/Query.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(PreprocessSuffix): ../sqlitewrap/Query.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(PreprocessSuffix) ../sqlitewrap/Query.cpp

$(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(ObjectSuffix): ../sqlitewrap/SysLogs.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/sqlitewrap/SysLogs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(PreprocessSuffix): ../sqlitewrap/SysLogs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(PreprocessSuffix) ../sqlitewrap/SysLogs.cpp

$(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(ObjectSuffix): ../sqlitewrap/StderrLog.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/admin/code/Oblivion2-XRM/sqlitewrap/StderrLog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(PreprocessSuffix): ../sqlitewrap/StderrLog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(PreprocessSuffix) ../sqlitewrap/StderrLog.cpp

##
## Clean
##
clean:
	$(RM) -r ./Debug/


