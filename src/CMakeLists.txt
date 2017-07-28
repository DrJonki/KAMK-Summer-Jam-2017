set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
  ${DIR}/main.cpp
)

add_executable(Jam ${SRC})
add_dependencies(Jam SFML)

add_definitions(-DSFML_STATIC)

if (MSVC)
  ExternalProject_Get_Property(SFML BINARY_DIR)
  target_link_libraries(Jam
    # SFML libs
    ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-audio-s-d.lib
    ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-graphics-s-d.lib
    ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-main-d.lib
    ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-system-s-d.lib
    ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-window-s-d.lib

    # SFML dependencies
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/flac.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/freetype.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/jpeg.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/ogg.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/openal32.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/vorbis.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/vorbisenc.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/vorbisfile.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/flac.lib

    opengl32.lib
    winmm.lib
    gdi32.lib
  )
endif()