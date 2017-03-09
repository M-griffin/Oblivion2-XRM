##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased
##
## Debug
ProjectName            :=xrm-menu-prompt-convert
ConfigurationName      :=Debug
WorkspacePath          :=C:/Users/Blue/Desktop/Oblivion2-XRM/win32
ProjectPath            :=C:/Users/Blue/Desktop/Oblivion2-XRM/xrm-menu-prompt-convert
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Blue
Date                   :=03/01/2017
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
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../src $(IncludeSwitch)C:/boost/include/boost-1_58 $(IncludeSwitch)C:/boost_1_58_0/boost $(IncludeSwitch)C:/boost/include $(IncludeSwitch)C:/Users/Blue/Desktop/Test2/sqlitewrap $(IncludeSwitch)C:/sqlite3/include $(IncludeSwitch)C:/OpenSSL-Win32/include $(IncludeSwitch)C:/Users/Blue/Documents/GitHub/yaml-cpp/include $(IncludeSwitch)C:\Users\Blue\Downloads\icu_53_1_mingw_builds_4_9_1_posix_dwarf_32_devel\icu53_1\include
IncludePCH             :=
RcIncludePath          :=
Libs                   := $(LibrarySwitch)boost_system-mgw51-mt-1_58 $(LibrarySwitch)boost_thread-mgw51-mt-1_58 $(LibrarySwitch)boost_chrono-mgw51-mt-1_58 $(LibrarySwitch)boost_serialization-mgw51-mt-1_58 $(LibrarySwitch)boost_locale-mgw51-mt-1_58 $(LibrarySwitch)boost_filesystem-mgw51-mt-1_58 $(LibrarySwitch)boost_timer-mgw51-mt-1_58 $(LibrarySwitch)boost_regex-mgw51-mt-1_58 $(LibrarySwitch)ws2_32 $(LibrarySwitch)wsock32 $(LibrarySwitch)iconv $(LibrarySwitch)icuuc.dll $(LibrarySwitch)icuin.dll $(LibrarySwitch)SqliteWrapped $(LibrarySwitch)sqlite3 $(LibrarySwitch)ssleay32 $(LibrarySwitch)eay32 $(LibrarySwitch)yaml-cpp
ArLibs                 :=  "libboost_system-mgw51-mt-1_58" "libboost_thread-mgw51-mt-1_58" "libboost_chrono-mgw51-mt-1_58" "libboost_serialization-mgw51-mt-1_58" "libboost_locale-mgw51-mt-1_58" "libboost_filesystem-mgw51-mt-1_58" "libboost_timer-mgw51-mt-1_58" "libboost_regex-mgw51-mt-1_58" "libws2_32.a" "libwsock32.a" "libiconv.a" "libicuuc.dll.a" "libicuin.dll.a" "libSqliteWrapped.a" "libsqlite3.a" "ssleay32.a" "libeay32.a" "yaml-cpp.dll"
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)C:/boost/lib $(LibraryPathSwitch)C:/msys/1.0/icu/source/lib $(LibraryPathSwitch)C:/sqlite3/lib $(LibraryPathSwitch)C:/Users/Blue/Desktop/Test2/sqlitewrap/Debug $(LibraryPathSwitch)C:/OpenSSL-Win32/lib/MinGW $(LibraryPathSwitch)C:/OpenSSL-Win32/lib/ $(LibraryPathSwitch)C:/Users/Blue/Documents/GitHub/yaml-cpp/build

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW-5.1.0/mingw32/bin/ar.exe rcu
CXX      := C:/MinGW-5.1.0/mingw32/bin/g++.exe
CC       := C:/MinGW-5.1.0/mingw32/bin/gcc.exe
CXXFLAGS :=  -Wfatal-errors -g -O0 -pedantic -W -std=c++14 -Wall -Wno-deprecated -Wno-unused-local-typedefs $(Preprocessors)
CFLAGS   :=  -Wfatal-errors -g -O0 -pedantic -W -std=c++14 -Wall -Wno-deprecated -Wno-unused-local-typedefs $(Preprocessors)
ASFLAGS  :=
AS       := C:/MinGW-5.1.0/mingw32/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
Objects0=$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data_config_dao.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_data_menu_dao.cpp$(ObjectSuffix)



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
$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix): src/main.cpp $(IntermediateDirectory)/src_main.cpp$(DependSuffix)
    $(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/xrm-menu-convert/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main.cpp$(DependSuffix): src/main.cpp
    @$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main.cpp$(DependSuffix) -MM src/main.cpp

$(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix): src/main.cpp
    $(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix)src/main.cpp

$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix): ../src/common_io.cpp $(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix)
    $(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/common_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix): ../src/common_io.cpp
    @$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_common_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_common_io.cpp$(DependSuffix) -MM ../src/common_io.cpp

$(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix): ../src/common_io.cpp
    $(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_common_io.cpp$(PreprocessSuffix)../src/common_io.cpp

$(IntermediateDirectory)/up_src_data_config_dao.cpp$(ObjectSuffix): ../src/data/config_dao.cpp $(IntermediateDirectory)/up_src_data_config_dao.cpp$(DependSuffix)
    $(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/data/config_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data_config_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data_config_dao.cpp$(DependSuffix): ../src/data/config_dao.cpp
    @$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data_config_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data_config_dao.cpp$(DependSuffix) -MM ../src/data/config_dao.cpp

$(IntermediateDirectory)/up_src_data_config_dao.cpp$(PreprocessSuffix): ../src/data/config_dao.cpp
    $(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data_config_dao.cpp$(PreprocessSuffix)../src/data/config_dao.cpp

$(IntermediateDirectory)/up_src_data_menu_dao.cpp$(ObjectSuffix): ../src/data/menu_dao.cpp $(IntermediateDirectory)/up_src_data_menu_dao.cpp$(DependSuffix)
    $(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Blue/Desktop/Oblivion2-XRM/src/data/menu_dao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_data_menu_dao.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_data_menu_dao.cpp$(DependSuffix): ../src/data/menu_dao.cpp
    @$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_data_menu_dao.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_data_menu_dao.cpp$(DependSuffix) -MM ../src/data/menu_dao.cpp

$(IntermediateDirectory)/up_src_data_menu_dao.cpp$(PreprocessSuffix): ../src/data/menu_dao.cpp
    $(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_data_menu_dao.cpp$(PreprocessSuffix)../src/data/menu_dao.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
    $(RM) -r ./Debug/


