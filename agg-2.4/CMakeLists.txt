PROJECT (antigrain)

# Additional are modified Find routines

SET (CMAKE_MODULE_PATH "${antigrain_SOURCE_DIR}/bin")

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET (AGG_MAJOR_VERSION 0)
SET (AGG_MINOR_VERSION 1)
SET (AGG_BUILD_VERSION 1)
SET (AGG_FLAGS "")
SET (AGG_INCLUDE_DIRS "")
SET (AGG_LIBRARY_DIRS "")
SET (AGG_LIBRARIES "")

SET (LIBRARY_OUTPUT_PATH ${antigrain_BINARY_DIR}/lib/ CACHE PATH "Single output directory for building all libraries." FORCE)
SET (AGG_LIBRARY_DIRS lib)
#SET (EXECUTABLE_OUTPUT_PATH ${antigrain_BINARY_DIR}/exe/ CACHE PATH "Single output directory for building all executables.")
#MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)

LINK_DIRECTORIES(${antigrain_BINARY_DIR}/lib)
LINK_LIBRARIES(antigrain)

OPTION(AGG_USE_GPC "Use Gpc Boolean library" OFF)
OPTION(AGG_USE_FREETYPE "Use Freetype library" OFF)
OPTION(AGG_USE_EXPAT "Use Expat library" OFF)
OPTION(AGG_USE_SDL_PLATFORM "Use SDL as platform" OFF)
OPTION(AGG_USE_PACK "Package Agg" OFF)
# [DO]: AGG2D does not seem to be fully implemented yet. 
OPTION(AGG_USE_AGG2D "Agg 2D graphical context" OFF)
OPTION(AGG_USE_DEBUG "For debug version" OFF)
OPTION(AGG_BUILD_EXAMPLES "Build program examples" OFF)

IF (AGG_USE_DEBUG)
  #SET (PFDEBUG "d")
  SET (CMAKE_DEBUG_POSTFIX "d")
ENDIF (AGG_USE_DEBUG)

# =============================================================================
# Detect the platform code.
# =============================================================================
IF (WIN32)
  ADD_DEFINITIONS(-D_MSWVC_ -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
  SET (WIN32GUI WIN32)    
  INCLUDE_DIRECTORIES( ${antigrain_SOURCE_DIR}/font_win32_tt)
  SET (AGG_INCLUDE_DIRS ${AGG_INCLUDE_DIRS} font_win32_tt)    
ELSEIF (UNIX)
  ADD_DEFINITIONS( -D__UNIX__ )
  SET (WIN32GUI "")
  FIND_PACKAGE(X11)
  IF (X11_FOUND)  
    INCLUDE_DIRECTORIES(${X11_INCLUDE_DIRS})
    LINK_LIBRARIES(${X11_LIBRARIES})
  ENDIF (X11_FOUND)
ENDIF ()
# =============================================================================
# Find expat
# =============================================================================
IF (AGG_USE_EXPAT)
  FIND_PACKAGE( EXPAT)
  IF (EXPAT_FOUND)  
    INCLUDE_DIRECTORIES(${EXPAT_INCLUDE_DIRS})
    LINK_LIBRARIES(${EXPAT_LIBRARIES})
  ELSE(EXPAT_FOUND)  
    MESSAGE(SEND_ERROR "expat not found")
  ENDIF (EXPAT_FOUND)
ENDIF (AGG_USE_EXPAT)
   
# =============================================================================
# Find freetype
# =============================================================================
IF (AGG_USE_FREETYPE)
  FIND_PACKAGE( Freetype)                    
  IF (NOT FREETYPE_FOUND)
    MESSAGE(FATAL_ERROR "freetype not found")
  ENDIF (FREETYPE_FOUND)
  
  INCLUDE_DIRECTORIES( ${FREETYPE_INCLUDE_DIRS})
  LINK_LIBRARIES(${FREETYPE_LIBRARIES})
  LINK_DIRECTORIES( ${FREETYPE_LINK_DIR})		

  INCLUDE_DIRECTORIES( ${antigrain_SOURCE_DIR}/font_freetype)	
  SET (AGG_INCLUDE_DIRS ${AGG_INCLUDE_DIRS} font_freetype)    
  ADD_DEFINITIONS(-DAGG_USE_FREETYPE)
  SET (AGG_FLAGS ${AGG_FLAGS} -DAGG_USE_FREETYPE)    
  LINK_LIBRARIES(freetypefont)
  SET (AGG_LIBRARIES ${AGG_LIBRARIES} aggfontfreetype${PFDEBUG})

  ADD_DEFINITIONS(-DAGG2D_USE_FREETYPE)
  SET (AGG_FLAGS ${AGG_FLAGS} -DAGG2D_USE_FREETYPE)
ENDIF ()

# =============================================================================
# Find SDL
# =============================================================================
IF (AGG_USE_SDL_PLATFORM)
  FIND_PACKAGE(SDL REQUIRED)
  IF (NOT SDL_FOUND)
    MESSAGE(FATAL_ERROR "SDL is not found: please disable AGG_USE_SDL_PLATFORM")
  ENDIF ()
  INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
  LINK_LIBRARIES(${SDL_LIBRARY})
ENDIF ()

# =============================================================================
# SDL as platform or OS
# =============================================================================
IF (SDL_FOUND AND AGG_USE_SDL_PLATFORM)
  LINK_LIBRARIES(controls sdlplatform)
  SET (AGG_LIBRARIES ${AGG_LIBRARIES} aggctrl${PFDEBUG} aggsdlplatform${PFDEBUG} agg${PFDEBUG})
ELSEIF (AGG_BUILD_EXAMPLES)
  LINK_LIBRARIES(controls platform)
  SET (AGG_LIBRARIES ${AGG_LIBRARIES} aggctrl${PFDEBUG} aggplatform${PFDEBUG} agg${PFDEBUG})
ENDIF ()

# =============================================================================
# GPC lib if used within Agg
# =============================================================================
IF (AGG_USE_GPC)
  INCLUDE_DIRECTORIES( ${antigrain_SOURCE_DIR}/gpc)
  SET (AGG_INCLUDE_DIRS ${AGG_INCLUDE_DIRS} gpc)    
  ADD_DEFINITIONS( -DAGG_USE_GPC)
  SET (AGG_FLAGS ${AGG_FLAGS} -DAGG_USE_GPC)    
  LINK_LIBRARIES(gpcbool)
  SET (AGG_LIBRARIES ${AGG_LIBRARIES} gpc${PFDEBUG}) 
ENDIF ()

   
# =============================================================================
# Agg2D lib if used within Agg
# =============================================================================
IF (AGG_USE_AGG2D)
  INCLUDE_DIRECTORIES( ${antigrain_SOURCE_DIR}/agg2d)
  SET (AGG_INCLUDE_DIRS ${AGG_INCLUDE_DIRS} agg2d)    
  ADD_DEFINITIONS( -DAGG_USE_AGG2D)
  OPTION(AGG_USE_AGG2D_FREETYPE "Agg 2D graphical context uses freetype" OFF)    
  SET (AGG_FLAGS ${AGG_FLAGS} -DAGG_USE_AGG2D)    
  LINK_LIBRARIES(agg2d)
  SET (AGG_LIBRARIES ${AGG_LIBRARIES} agg2d${PFDEBUG}) 
ENDIF ()

# =============================================================================
# Generate AGG libraries
# =============================================================================
INCLUDE_DIRECTORIES (${antigrain_SOURCE_DIR}/include)
SET (AGG_INCLUDE_DIRS ${AGG_INCLUDE_DIRS} include) 
SET (AGG_FLAGS ${AGG_FLAGS} CACHE STRING "Agg package flags" FORCE)

SET (AGG_INCLUDE_DIRS ${AGG_INCLUDE_DIRS} CACHE STRING 
     "Agg package libs include paths" FORCE)
SET (AGG_LIBRARY_DIRS ${AGG_LIBRARY_DIRS} CACHE STRING 
     "Agg package libs directory paths" FORCE)
SET (AGG_LIBRARIES ${AGG_LIBRARIES} CACHE STRING "Agg package libraries" FORCE)

ADD_SUBDIRECTORY(src)
IF (AGG_BUILD_EXAMPLES)
  ADD_SUBDIRECTORY(examples)
ENDIF ()
ADD_SUBDIRECTORY(tutorial)
#ADD_SUBDIRECTORY(myapp)
#INCLUDE( myapp/myproject.cmake)

# =============================================================================
# Main include directory of AGG
# =============================================================================
CONFIGURE_FILE(${antigrain_SOURCE_DIR}/bin/AggConfig.cmake.in
               ${antigrain_BINARY_DIR}/bin/AggConfig.cmake
               @ONLY IMMEDIATE)
CONFIGURE_FILE(${antigrain_SOURCE_DIR}/bin/AggConfigOutBuild.cmake.in
               ${antigrain_BINARY_DIR}/bin/AggConfigOutBuild.cmake
               @ONLY IMMEDIATE)               
CONFIGURE_FILE(${antigrain_SOURCE_DIR}/bin/FindAgg.cmake
               ${antigrain_BINARY_DIR}/myapp/FindAgg.cmake
               @ONLY IMMEDIATE)
CONFIGURE_FILE(${antigrain_SOURCE_DIR}/bin/UseAgg.cmake.in
               ${antigrain_BINARY_DIR}/bin/UseAgg.cmake
               @ONLY IMMEDIATE)
INSTALL( FILES ${antigrain_BINARY_DIR}/bin/AggConfigOutBuild.cmake DESTINATION "bin" RENAME AggConfig.cmake)
INSTALL( FILES ${antigrain_BINARY_DIR}/bin/AggConfig.cmake DESTINATION "bin")
INSTALL( FILES ${antigrain_BINARY_DIR}/bin/UseAgg.cmake DESTINATION "bin")


# =============================================================================
# Build a CPack installer if CPack is available and this is a build 
# =============================================================================
IF (AGG_USE_PACK)    
  IF (EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
    SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Agg - Vector Graphics")
    SET (CPACK_PACKAGE_VENDOR "Agg")
    SET (CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/copying")
    SET (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/copying")
    SET (CPACK_PACKAGE_VERSION_MAJOR "${AGG_MAJOR_VERSION}")
    SET (CPACK_PACKAGE_VERSION_MINOR "${AGG_MINOR_VERSION}")
    SET (CPACK_PACKAGE_VERSION_PATCH "${AGG_BUILD_VERSION}")
    SET (CPACK_PACKAGE_INSTALL_DIRECTORY "AGG_${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
    SET (CPACK_SOURCE_PACKAGE_FILE_NAME "agg-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
    SET (CPACK_PACKAGE_EXECUTABLES "agg" "AGG")
    SET (CPACK_SOURCE_STRIP_FILES "")
    SET (CPACK_STRIP_FILES "bin/ccmake;bin/cmake;bin/cpack;bin/ctest")
    IF (WIN32)
      SET (CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
      SET (CPACK_NSIS_HELP_LINK "http://agg.sourceforge.net")
      SET (CPACK_NSIS_URL_INFO_ABOUT "http://agg.sourceforge.net")
      SET (CPACK_NSIS_CONTACT "http://agg.sourceforge.net")
    ENDIF (WIN32)
    INCLUDE(CPack)
  ENDIF ()
ENDIF ()
