# CMakeList.txt : CMake project for CMakeSLAM

# pscp -r C:\Users\bweibel\source\repos\CMakeSLAM 
# weibel@royal-10.cs.wisc.edu:/u/w/e/weibel/projects/CMakeSLAM

cmake_minimum_required (VERSION 3.8)

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE "../")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
add_compile_options(-std=c++17)

if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra)
endif()

# Slam Source Groups:
###############################################################################

set (SourceGroup_FileMapping
	"SourceFiles/FileMapping.cpp"
	"HeaderFiles/FileMapping.h"
)
source_group("FileMapping" FILES ${SourceGroup_FileMapping})

set (SourceGroup_InstructionFileReader
	"SourceFiles/InstructionFileReader.cpp"
	"HeaderFiles/InstructionFileReader.h"
)
source_group("InstructionFileReader" FILES ${SourceGroup_InstructionFileReader})

set (SourceGroup_InstructionPackage
	"SourceFiles/InstructionPackage.cpp"
	"HeaderFiles/InstructionPackage.h"
)
source_group("InstructionPackage" FILES ${SourceGroup_InstructionPackage})

set (SourceGroup_InstructionParser
	"SourceFiles/InstructionParser.cpp"
	"HeaderFiles/InstructionParser.h"
)
source_group("InstructionParser" FILES ${SourceGroup_InstructionParser})

set (SourceGroup_Transformation
	"SourceFiles/Transformation.cpp"
	"HeaderFiles/Transformation.h"
)
source_group("Transformation" FILES ${SourceGroup_Transformation})

set (SourceGroup_CrashAndBurn
	"SourceFiles/CrashAndBurn.cpp"
	"HeaderFiles/CrashAndBurn.h"
)
source_group("CrashAndBurn" FILES ${SourceGroup_CrashAndBurn})

set (SourceGroup_InputFiles
	"SourceFiles/InputFile.cpp"
	"HeaderFiles/InputFile.h"
)
source_group("InputFile" FILES ${SourceGroup_InputFiles})


# Test Source Groups:
###############################################################################

set (SourceGroup_TransformationTests
	"UnitTestFiles/Test_Sources/TransformationTests.cpp"
)
source_group("TransformationTests" FILES ${SourceGroup_TransformationTests})

set (SourceGroup_InputFileTests
	"UnitTestFiles/Test_Sources/InputFileTests.cpp"
)
source_group("InputFileTests" FILES ${SourceGroup_InputFileTests})

set (SourceGroup_FileMappingTests
	"UnitTestFiles/Test_Sources/FileMappingTests.cpp"
)
source_group("FileMappingTests" FILES ${SourceGroup_FileMappingTests})

set (SourceGroup_InstructionPackageTests
	"UnitTestFiles/Test_Sources/InstructionPackageTests.cpp"
)
source_group("InstructionPackageTests" FILES ${SourceGroup_InstructionPackageTests})


###############################################################################

add_executable (SLAM 
	"SourceFiles/CMakeSLAM.cpp" 
	"HeaderFiles/CMakeSLAM.h"
	${SourceGroup_FileMapping}
	${SourceGroup_InputFiles}
	${SourceGroup_InstructionFileReader}
	${SourceGroup_InstructionPackage}
	${SourceGroup_InstructionParser}
	${SourceGroup_Transformation}
	${SourceGroup_CrashAndBurn}
	)

add_executable (SLAM_Tests
	### test files ###
	"UnitTestFiles/TestMain.h"
	${SourceGroup_FileMappingTests}
	${SourceGroup_InputFileTests}
	${SourceGroup_InstructionPackageTests}
	${SourceGroup_TransformationTests}

	### program files ###
	${SourceGroup_FileMapping}
	${SourceGroup_InputFiles}
	${SourceGroup_InstructionFileReader}
	${SourceGroup_InstructionPackage}
	${SourceGroup_InstructionParser}
	${SourceGroup_Transformation}
	${SourceGroup_CrashAndBurn}
	)

	
target_link_libraries(SLAM_Tests Catch)

add_test(NAME SlamTests COMMAND SLAM_Tests)