cmake_minimum_required(VERSION 3.2.0)

# set project name
set(PROJ_NAME CG_Animation)
project(${PROJ_NAME})

# Configuration
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)

# USE Qt5
if(WIN32 AND MSVC_IDE)
	set(USE_QT5 OFF CACHE BOOL "Use Qt 5 for GUI frame")
else()
	set(USE_QT5 ON CACHE BOOL "Use Qt 5 for GUI frame")
endif()

# Set output Director
if(MSVC_IDE)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "./bin")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "./bin")
else(MSVC_IDE)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
endif(MSVC_IDE)

include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/GLInterface)
include_directories(${CMAKE_SOURCE_DIR}/QtFrame)
include_directories(${CMAKE_SOURCE_DIR}/soil/src)

find_path(SOIL_INCLUDE_DIR SOIL.h)

find_library(SOIL_LIBRARY SOIL)
if (SOIL_INCLUDE_DIR AND SOIL_LIBRARY)
	set(SOIL_FOUND TRUE)
	add_definitions(-DHAVE_LIBSIFTGPU=1)
	include_directories(${SOIL_INCLUDE_DIR})
	set(SOIL_LIBRARIES ${SOIL_LIBRARIES} ${SOIL_LIBRARY})
endif ()



#######################################################
# find opengl 
#######################################################
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
  message(STATUS "OpenGL is found.")
  message("${OPENGL_INCLUDE_DIR}")
else()
  message(FATAL_ERROR "OpenGL is not found")
endif()
include_directories(${OPENGL_INCLUDE_DIR})

#######################################################
# find opengl 
#######################################################
IF(NOT GLUT_ROOT_PATH)
	SET(GLUT_ROOT_PATH CACHE PATH "GLUT root path")
ENDIF()
find_package(GLUT REQUIRED)
if(GLUT_FOUND)
  message(STATUS "GLUT is found.")
  message("${GLUT_INCLUDE_DIR}")
else()
  message(FATAL_ERROR "GLUT is not found")
endif()
include_directories(${GLUT_INCLUDE_DIR})
set(LIBS ${LIBS} ${GLUT_LIBRARY})

#######################################################
# project setting
#######################################################
if(USE_QT5)
	# Check Qmake file
	IF(NOT QT_QMAKE_EXECUTABLE)
  	FIND_PROGRAM(QT_QMAKE_EXECUTABLE NAMES qmake qmake5 qmake-qt5
               	PATHS "${QT_SEARCH_PATH}/bin" "$ENV{QTDIR}/bin")
  	#SET(QT_QMAKE_EXECUTABLE ${QT_QMAKE_EXECUTABLE_FINDQT} CACHE PATH "Qt qmake program.")
	ENDIF(NOT QT_QMAKE_EXECUTABLE)
	
	get_filename_component(QT5_BIN_PATH ${QT_QMAKE_EXECUTABLE} DIRECTORY)
	#MESSAGE(${QT5_BIN_PATH})  
	# Get Qt version from QMake
	EXEC_PROGRAM(${QT_QMAKE_EXECUTABLE} ARGS "-query QT_VERSION" OUTPUT_VARIABLE QTVERSION)
	IF(NOT QTVERSION MATCHES "5.*")
    	MESSAGE(FATAL_ERROR "CMake was unable to find Qt5, put qmake in your path or set QTDIR/QT_QMAKE_EXECUTABLE.")
  	#RETURN()
	ENDIF(NOT QTVERSION MATCHES "5.*")
	
	# Get QT_HOST_PREFIX
	EXEC_PROGRAM(${QT_QMAKE_EXECUTABLE} ARGS "-query QT_HOST_PREFIX" OUTPUT_VARIABLE QTHOSTPREFIX)
	SET(CMAKE_PREFIX_PATH ${QTHOSTPREFIX})
	#MESSAGE(FATAL_ERROR ${QTHOSTPREFIX})
	
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTOUIC ON)
	set(CMAKE_AUTORCC ON)
	
	find_package(Qt5Widgets)
	find_package(Qt5Gui)
	find_package(Qt5Core)
	find_package(Qt5OpenGL)
	
	set(LIBS ${LIBS} Qt5::Widgets Qt5::Gui Qt5::Core Qt5::OpenGL)
	file(GLOB GUI_FILES ${CMAKE_SOURCE_DIR}/QtFrame/*.h 
					${CMAKE_SOURCE_DIR}/QtFrame/*.cpp 
					)

	message(STATUS "GUI source files : ${GUI_FILES}")
	message(STATUS "Qt5 GUI is activated")
else() #MFC

	file(GLOB GUI_FILES ${CMAKE_SOURCE_DIR}/MFCFrame/*.h 
					${CMAKE_SOURCE_DIR}/MFCFrame/*.cpp 
					${CMAKE_SOURCE_DIR}/MFCFrame/*.rc 
					${CMAKE_SOURCE_DIR}/MFCFrame/*.rc2 
					)
	message(STATUS "GUI source files : ${GUI_FILES}")
	message(STATUS "MFC GUI is activated")
	add_definitions(-D_AFXDLL)
	set(CMAKE_MFC_FLAG 2)
endif()

# Disable SAFESEH
if(MSVC_IDE)
SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
SET (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
endif()

# APPLE setting
if(APPLE)
add_definitions(-D_APPLE_OSX)
endif()

# find GLInterface files
file(GLOB GLINTERFACE_FILES ${CMAKE_SOURCE_DIR}/GLInterface/*.h 
					${CMAKE_SOURCE_DIR}/GLInterface/*.cpp)
 
message(STATUS "GLInterface source files : ${GLINTERFACE_FILES}")

# Add executable
ADD_LIBRARY( GLInterface ${GLINTERFACE_FILES} )
target_link_libraries( GLInterface ${OPENGL_LIBRARIES})

 
# Add executable
ADD_EXECUTABLE(${PROJ_NAME} MACOSX_BUNDLE ${GUI_FILES} )
#set_target_properties(${PROJ_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)

IF(NOT USE_QT5)
IF(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
    set_target_properties(${PROJ_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    set_target_properties(${PROJ_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
  endif()
endif()
endif()

# link set
target_link_libraries(${PROJ_NAME} ${SOIL_LIBRARIES})
target_link_libraries( ${PROJ_NAME} 
                       GLInterface
					   ${LIBS}
					   )

