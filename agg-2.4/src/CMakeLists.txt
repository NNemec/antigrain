# =============================================================================
# Antigrain Geometry Core Libraries.
# =============================================================================
FILE (GLOB antigrain_HEADERS ${antigrain_SOURCE_DIR}/include/*.h)
FILE (GLOB antigrain_SOURCES *.cpp)
ADD_LIBRARY (antigrain ${antigrain_SOURCES} ${antigrain_HEADERS})
SET_TARGET_PROPERTIES (antigrain PROPERTIES OUTPUT_NAME agg${PFDEBUG})
INSTALL (FILES ${antigrain_HEADERS} DESTINATION agg/include)
INSTALL (TARGETS antigrain DESTINATION lib)

# =============================================================================
# Controls code for interactive modifying samples.
# =============================================================================
IF (AGG_BUILD_EXAMPLES)
  FILE (GLOB controls_HEADERS 
        ${antigrain_SOURCE_DIR}/include/ctrl/*.h)
  FILE (GLOB controls_SOURCES ctrl/*.cpp)
  ADD_LIBRARY (controls ${controls_SOURCES} ${controls_HEADERS})
  SET_TARGET_PROPERTIES (controls  PROPERTIES OUTPUT_NAME aggctrl${PFDEBUG})
  INSTALL (FILES ${controls_HEADERS} DESTINATION agg/include/ctrl)
  INSTALL (TARGETS controls DESTINATION lib)
ENDIF ()

# =============================================================================
# Platform-dependent GUI stuff to ease sample use.
# =============================================================================
IF (AGG_BUILD_EXAMPLES)  
  # 1. Get the master header file.
  SET(platform_HEADERS 
      ${antigrain_SOURCE_DIR}/include/platform/AGG_platform_support.h)
  # 2. Populate the right GUI sources for the corresponding OS.
  IF (WIN32)
    INCLUDE_DIRECTORIES( ${antigrain_SOURCE_DIR}/font_win32_tt )
    ADD_LIBRARY(platform
                ../src/platform/win32/AGG_win32_bmp.cpp
                ../src/platform/win32/AGG_platform_support.cpp
                ../font_win32_tt/AGG_font_win32_tt.cpp
                ${platform_HEADERS})
    INSTALL(FILES ../font_win32_tt/AGG_font_win32_tt.h 
            DESTINATION agg/font_win32_tt)
  ELSEIF (UNIX)
    ADD_LIBRARY(platform
                ../src/platform/X11/AGG_platform_support.cpp
                ${platform_HEADERS})
  ELSEIF (APPLE)
    ADD_LIBRARY(platform
                ../src/platform/mac/AGG_mac_pmap.cpp
                ../src/platform/mac/AGG_platform_support.cpp
                ${platform_HEADERS})
  ENDIF ()
  SET_TARGET_PROPERTIES(platform PROPERTIES OUTPUT_NAME aggplatform${PFDEBUG})
  INSTALL(FILES ${platform_HEADERS} DESTINATION agg/include/platform)
  INSTALL(TARGETS platform DESTINATION lib)
  # 3. Optional: build SDL-dependent stuff.
  IF (SDL_FOUND AND AGG_USE_SDL_PLATFORM EQUAL ON)
    ADD_LIBRARY(sdlplatform
                ../src/platform/sdl/AGG_platform_support.cpp
                ${platform_HEADERS})
    INSTALL(TARGETS sdlplatform DESTINATION lib)
    SET_TARGET_PROPERTIES(sdlplatform PROPERTIES 
                          OUTPUT_NAME aggsdlplatform${PFDEBUG})
  ENDIF ()
ENDIF ()

# =============================================================================
# Freetype library
# =============================================================================
IF (AGG_USE_FREETYPE)
  INCLUDE_DIRECTORIES(${antigrain_SOURCE_DIR}/font_freetype)
  ADD_LIBRARY(freetypefont
              ../font_freetype/AGG_font_freetype.h
              ../font_freetype/AGG_font_freetype.cpp)
  SET_TARGET_PROPERTIES(freetypefont PROPERTIES 
                        OUTPUT_NAME aggfontfreetype${PFDEBUG})
  INSTALL(TARGETS freetypefont DESTINATION lib)
	INSTALL(FILES ../font_freetype/AGG_font_freetype.h
          DESTINATION agg/font_freetype)	
ENDIF ()

# =============================================================================
# Boolean operations library GPC
# =============================================================================
IF (AGG_USE_GPC)
  INCLUDE_DIRECTORIES( ${antigrain_SOURCE_DIR}/gpc )
  ADD_LIBRARY(gpcbool ../gpc/gpc.c ../gpc/gpc.h)
  INSTALL(TARGETS gpcbool DESTINATION lib)
	INSTALL(FILES ../gpc/gpc.h DESTINATION agg/gpc)	
  SET_TARGET_PROPERTIES(gpcbool PROPERTIES OUTPUT_NAME gpc${PFDEBUG})
ENDIF ()

# =============================================================================
# AGG 2D module
# =============================================================================
IF (AGG_USE_AGG2D)
  INCLUDE_DIRECTORIES(${antigrain_SOURCE_DIR}/agg2d)
  ADD_LIBRARY(agg2d ../agg2d/agg2d.cpp ../agg2d/agg2d.h)
  INSTALL(TARGETS agg2d DESTINATION lib)
	INSTALL(FILES ../agg2d/agg2d.h DESTINATION agg/agg2d)	
  SET_TARGET_PROPERTIES(agg2d PROPERTIES OUTPUT_NAME agg2d${PFDEBUG})
ENDIF ()
