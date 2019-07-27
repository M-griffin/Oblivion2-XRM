##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=SqliteWrapped
ConfigurationName      :=Debug
WorkspacePath          :=../win32
ProjectPath            :=../win32
IntermediateDirectory  :=$(ProjectPath)/Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Blue
Date                   :=27/07/2019
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
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../Debug $(IncludeSwitch)../sqlitewrap $(IncludeSwitch)C:/sqlite3/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)../Debug $(LibraryPathSwitch)C:/sqlite3/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW-5.1.0/mingw32/bin/ar.exe rcu
CXX      := C:/MinGW-5.1.0/mingw32/bin/g++.exe
CC       := C:/MinGW-5.1.0/mingw32/bin/gcc.exe
CXXFLAGS :=  -Wfatal-errors -g -O0 -W -std=c++11 -Wall -static $(Preprocessors)
CFLAGS   :=  -Wfatal-errors -g -O0 -W -std=c++11 -Wall -static $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW-5.1.0/mingw32/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(ObjectSuffix) 



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
	
	type ..\sqlitewrap\IError.h ..\sqlitewrap\StderrLog.h ..\sqlitewrap\SysLogs.h ..\sqlitewrap\Database.h ..\sqlitewrap\Query.h > C:\Users\Blue\Desktop\Oblivion2-XRM\win32/Debug\libSqliteWrapped.h
	@echo Done

MakeIntermediateDirs:
	@$(MakeDirCommand) "$(ProjectPath)/Debug"


$(ProjectPath)/Debug:
	@$(MakeDirCommand) "$(ProjectPath)/Debug"

PreBuild:
	@echo Executing Pre Build commands ...
	sed -i 's+../+../+' SqliteWrapped.mk
	@echo Done


##
## Objects
##
$(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(ObjectSuffix): ../sqlitewrap/Query.cpp $(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../sqlitewrap/Query.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(DependSuffix): ../sqlitewrap/Query.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(DependSuffix) -MM ../sqlitewrap/Query.cpp

$(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(PreprocessSuffix): ../sqlitewrap/Query.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_sqlitewrap_Query.cpp$(PreprocessSuffix) ../sqlitewrap/Query.cpp

$(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(ObjectSuffix): ../sqlitewrap/Database.cpp $(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../sqlitewrap/Database.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(DependSuffix): ../sqlitewrap/Database.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(DependSuffix) -MM ../sqlitewrap/Database.cpp

$(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(PreprocessSuffix): ../sqlitewrap/Database.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_sqlitewrap_Database.cpp$(PreprocessSuffix) ../sqlitewrap/Database.cpp

$(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(ObjectSuffix): ../sqlitewrap/StderrLog.cpp $(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../sqlitewrap/StderrLog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(DependSuffix): ../sqlitewrap/StderrLog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(DependSuffix) -MM ../sqlitewrap/StderrLog.cpp

$(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(PreprocessSuffix): ../sqlitewrap/StderrLog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_sqlitewrap_StderrLog.cpp$(PreprocessSuffix) ../sqlitewrap/StderrLog.cpp

$(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(ObjectSuffix): ../sqlitewrap/SysLogs.cpp $(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../sqlitewrap/SysLogs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(DependSuffix): ../sqlitewrap/SysLogs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(DependSuffix) -MM ../sqlitewrap/SysLogs.cpp

$(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(PreprocessSuffix): ../sqlitewrap/SysLogs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_sqlitewrap_SysLogs.cpp$(PreprocessSuffix) ../sqlitewrap/SysLogs.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(ProjectPath)/Debug/


