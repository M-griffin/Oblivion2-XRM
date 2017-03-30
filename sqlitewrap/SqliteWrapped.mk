##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=SqliteWrapped
ConfigurationName      :=Debug
WorkspacePath          :=C:/Users/Blue/Desktop/Oblivion2-XRM/win32
ProjectPath            :=C:/Users/Blue/Desktop/Oblivion2-XRM/sqlitewrap
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Blue
Date                   :=29/03/2017
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
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="SqliteWrapped.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-5.1.0/mingw32/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)C:/sqlite3/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)sqlite3 
ArLibs                 :=  "libsqlite3.a" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)C:/Users/Blue/Desktop/Test2/sqlitewrap/Debug $(LibraryPathSwitch)C:/sqlite3/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW-5.1.0/mingw32/bin/ar.exe rcu
CXX      := C:/MinGW-5.1.0/mingw32/bin/g++.exe
CC       := C:/MinGW-5.1.0/mingw32/bin/gcc.exe
CXXFLAGS :=  -Wfatal-errors -g -O0 -W -std=c++14 -Wall -static $(Preprocessors)
CFLAGS   :=  -Wfatal-errors -g -O0 -W -std=c++14 -Wall -static $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW-5.1.0/mingw32/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
Objects0=$(IntermediateDirectory)/Database.cpp$(ObjectSuffix) $(IntermediateDirectory)/Query.cpp$(ObjectSuffix) $(IntermediateDirectory)/StderrLog.cpp$(ObjectSuffix) $(IntermediateDirectory)/SysLog.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "C:\Users\Blue\Desktop\Oblivion2-XRM\win32/.build-debug"
	@echo rebuilt > "C:\Users\Blue\Desktop\Oblivion2-XRM\win32/.build-debug/SqliteWrapped"

PostBuild:
	@echo Executing Post Build commands ...
	
	type IError.h StderrLog.h SysLog.h Database.h Query.h > libSqliteWrapped.h
	@echo Done

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/Database.cpp$(ObjectSuffix): Database.cpp $(IntermediateDirectory)/Database.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/sqlitewrap/Database.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Database.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Database.cpp$(DependSuffix): Database.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Database.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Database.cpp$(DependSuffix) -MM Database.cpp

$(IntermediateDirectory)/Database.cpp$(PreprocessSuffix): Database.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Database.cpp$(PreprocessSuffix) Database.cpp

$(IntermediateDirectory)/Query.cpp$(ObjectSuffix): Query.cpp $(IntermediateDirectory)/Query.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/sqlitewrap/Query.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Query.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Query.cpp$(DependSuffix): Query.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Query.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Query.cpp$(DependSuffix) -MM Query.cpp

$(IntermediateDirectory)/Query.cpp$(PreprocessSuffix): Query.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Query.cpp$(PreprocessSuffix) Query.cpp

$(IntermediateDirectory)/StderrLog.cpp$(ObjectSuffix): StderrLog.cpp $(IntermediateDirectory)/StderrLog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/sqlitewrap/StderrLog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/StderrLog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/StderrLog.cpp$(DependSuffix): StderrLog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/StderrLog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/StderrLog.cpp$(DependSuffix) -MM StderrLog.cpp

$(IntermediateDirectory)/StderrLog.cpp$(PreprocessSuffix): StderrLog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/StderrLog.cpp$(PreprocessSuffix) StderrLog.cpp

$(IntermediateDirectory)/SysLog.cpp$(ObjectSuffix): SysLog.cpp $(IntermediateDirectory)/SysLog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/sqlitewrap/SysLog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SysLog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SysLog.cpp$(DependSuffix): SysLog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SysLog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SysLog.cpp$(DependSuffix) -MM SysLog.cpp

$(IntermediateDirectory)/SysLog.cpp$(PreprocessSuffix): SysLog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SysLog.cpp$(PreprocessSuffix) SysLog.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


