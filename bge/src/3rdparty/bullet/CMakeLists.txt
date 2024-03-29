cmake_minimum_required(VERSION 2.4)


#this line has to appear before 'PROJECT' in order to be able to disable incremental linking
SET(MSVC_INCREMENTAL_DEFAULT ON)

PROJECT(BULLET_PHYSICS)
SET(BULLET_VERSION 2.76)

IF (NOT CMAKE_BUILD_TYPE)
# SET(CMAKE_BUILD_TYPE "Debug")
 SET(CMAKE_BUILD_TYPE "Release")
ENDIF (NOT CMAKE_BUILD_TYPE)


OPTION(USE_DOUBLE_PRECISION "Use double precision"	OFF)
OPTION(USE_GRAPHICAL_BENCHMARK "Use Graphical Benchmark" ON)
OPTION(USE_MULTITHREADED_BENCHMARK "Use Multithreaded Benchmark" OFF)

IF (USE_MULTITHREADED_BENCHMARK)
  ADD_DEFINITIONS(-DUSE_PARALLEL_SOLVER_BENCHMARK -DUSE_PARALLEL_DISPATCHER_BENCHMARK)
ENDIF()

OPTION(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC Runtime Library DLL (/MD or /MDd)" ON)
OPTION(USE_MSVC_INCREMENTAL_LINKING "Use MSVC Incremental Linking" OFF)



#SET(CMAKE_EXE_LINKER_FLAGS_INIT    "/STACK:10000000 /INCREMENTAL:NO")
#SET(CMAKE_EXE_LINKER_FLAGS    "/STACK:10000000 /INCREMENTAL:NO")

#MESSAGE("MSVC_INCREMENTAL_YES_FLAG"+${MSVC_INCREMENTAL_YES_FLAG})


IF(MSVC)
  IF (NOT USE_MSVC_INCREMENTAL_LINKING)
    #MESSAGE("MSVC_INCREMENTAL_DEFAULT"+${MSVC_INCREMENTAL_DEFAULT})
    SET( MSVC_INCREMENTAL_YES_FLAG "/INCREMENTAL:NO")

    STRING(REPLACE "INCREMENTAL:YES" "INCREMENTAL:NO" replacementFlags ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
    SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "/INCREMENTAL:NO ${replacementFlags}" )
    MESSAGE("CMAKE_EXE_LINKER_FLAGS_DEBUG=${CMAKE_EXE_LINKER_FLAGS_DEBUG}")

#		STRING(REPLACE "INCREMENTAL:YES" "INCREMENTAL:NO" replacementFlags2 ${CMAKE_EXE_LINKER_FLAGS})
#		SET(CMAKE_EXE_LINKER_FLAGS ${replacementFlag2})
#		STRING(REPLACE "INCREMENTAL:YES" "" replacementFlags3 ${CMAKE_EXTRA_LINK_FLAGS})
#		SET(CMAKE_EXTRA_LINK_FLAGS ${replacementFlag3})


    STRING(REPLACE "INCREMENTAL:YES" "INCREMENTAL:NO" replacementFlags3 ${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO})
    SET(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO ${replacementFlags3})
    SET(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "/INCREMENTAL:NO ${replacementFlags3}" )

  ENDIF (NOT USE_MSVC_INCREMENTAL_LINKING)

  IF (NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
    #We statically link to reduce dependancies
    FOREACH(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      IF(${flag_var} MATCHES "/MD")
        STRING(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      ENDIF(${flag_var} MATCHES "/MD")
      IF(${flag_var} MATCHES "/MDd")
        STRING(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
      ENDIF(${flag_var} MATCHES "/MDd")
    ENDFOREACH(flag_var)
  ENDIF (NOT USE_MSVC_RUNTIME_LIBRARY_DLL)

  OPTION(USE_MSVC_SSE "Use MSVC /arch:sse option"	ON)
  IF (USE_MSVC_SSE)
    ADD_DEFINITIONS(/arch:SSE)
  ENDIF()
  OPTION(USE_MSVC_FAST_FLOATINGPOINT "Use MSVC /fp:fast option"	ON)
  IF (USE_MSVC_FAST_FLOATINGPOINT)
    ADD_DEFINITIONS(/fp:fast)
  ENDIF()
ENDIF(MSVC)



IF (WIN32)
OPTION(INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES "Create MSVC projectfiles that can be distributed" OFF)

IF (INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)
  SET (LIBRARY_OUTPUT_PATH ${BULLET_PHYSICS_SOURCE_DIR}/lib CACHE PATH "Single output directory for building all libraries.")
  SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BULLET_PHYSICS_SOURCE_DIR})
  SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BULLET_PHYSICS_SOURCE_DIR})
  SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BULLET_PHYSICS_SOURCE_DIR})
  SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${BULLET_PHYSICS_SOURCE_DIR})
  SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BULLET_PHYSICS_SOURCE_DIR})
ELSE()
  SET (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE PATH "Single output directory for building all libraries.")
ENDIF()

OPTION(INTERNAL_CREATE_MSVC_RELATIVE_PATH_PROJECTFILES "Create MSVC projectfiles with relative paths" OFF)
OPTION(INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES "Add MSVC postfix for executable names (_Debug)" OFF)


IF (INTERNAL_CREATE_MSVC_RELATIVE_PATH_PROJECTFILES)
SET(CMAKE_SUPPRESS_REGENERATION  1)
SET(CMAKE_USE_RELATIVE_PATHS 1)
ENDIF(INTERNAL_CREATE_MSVC_RELATIVE_PATH_PROJECTFILES)

ENDIF (WIN32)

OPTION(INTERNAL_UPDATE_SERIALIZATION_STRUCTURES "Internal update serialization structures" OFF)
IF (INTERNAL_UPDATE_SERIALIZATION_STRUCTURES)
ADD_DEFINITIONS( -DBT_INTERNAL_UPDATE_SERIALIZATION_STRUCTURES)
ENDIF (INTERNAL_UPDATE_SERIALIZATION_STRUCTURES)

IF (USE_DOUBLE_PRECISION)
ADD_DEFINITIONS( -DBT_USE_DOUBLE_PRECISION)
ENDIF (USE_DOUBLE_PRECISION)

IF(USE_GRAPHICAL_BENCHMARK)
ADD_DEFINITIONS( -DUSE_GRAPHICAL_BENCHMARK)
ENDIF (USE_GRAPHICAL_BENCHMARK)


IF(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

# ADD_DEFINITIONS(-DBT_USE_FREEGLUT)


SUBDIRS(src)

#INSTALL of other files requires CMake 2.6
IF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5)
  OPTION(INSTALL_EXTRA_LIBS "Set when you want extra libraries installed" OFF)
  OPTION(INSTALL_LIBS "Set when you want to install libraries" OFF)
ENDIF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5)


