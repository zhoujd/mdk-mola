###This cmake file for zzmediaxapp

cmake_minimum_required (VERSION 2.6)
project (ZZMEDIAXAPP)
MESSAGE(STATUS "This is BINARY dir " ${ZZMEDIAXAPP_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir " ${ZZMEDIAXAPP_SOURCE_DIR})

#cmake -DCMAKE_BUILD_TYPE=Debug path
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb ")

ADD_DEFINITIONS(
  -Wall
  -O0
  -g
  -DZZ_LIBVA_USE_X11=1
  -DZZ_DEBUG=1
  )

find_package(PkgConfig REQUIRED)

SET(LIBVA_LIBS
  libva
  libva-x11
  libva-drm)
pkg_check_modules(LIBVA REQUIRED ${LIBVA_LIBS})

SET(LIBDRM_LIBS
  libdrm
  libdrm_intel)
pkg_check_modules(LIBDRM REQUIRED ${LIBDRM_LIBS})

SET(X11_LIBS
  x11)
pkg_check_modules(LIBDRM REQUIRED ${X11_LIBS})

SET(FFMPEG_LIBS
  libavdevice
  libavformat
  libavfilter
  libavcodec
  libswresample
  libswscale
  libavutil)
pkg_check_modules(FFMPEG REQUIRED ${FFMPEG_LIBS})

SET(ZZMEDIAX_ROOT ${ZZMEDIAXAPP_SOURCE_DIR}/zzmediax)

INCLUDE_DIRECTORIES (
  ${LIBVA_INCLUDE_DIRS}
  ${LIBDRM_INCLUDE_DIRS}
  ${FFMPEG_INCLUDE_DIRS}
  ${ZZMEDIAX_ROOT}
  ${ZZMEDIAX_ROOT}/include
  ${ZZMEDIAX_ROOT}/framework
  ${ZZMEDIAX_ROOT}/utils
  ${ZZMEDIAXAPP_SOURCE_DIR}
  ${ZZMEDIAXAPP_SOURCE_DIR}/task
  ${ZZMEDIAXAPP_SOURCE_DIR}/matrix
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib
  )

SET(SRC_LIST
  ${ZZMEDIAXAPP_SOURCE_DIR}/zzmediaxapp.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/zzmediaxapp_register.c

  ${ZZMEDIAX_ROOT}/main.c
  ${ZZMEDIAX_ROOT}/framework/app_base.c
  ${ZZMEDIAX_ROOT}/framework/dispatch.c
  ${ZZMEDIAX_ROOT}/framework/event.c
  ${ZZMEDIAX_ROOT}/framework/help.c
  ${ZZMEDIAX_ROOT}/framework/matrix_base.c
  ${ZZMEDIAX_ROOT}/framework/matrix_grp.c
  ${ZZMEDIAX_ROOT}/framework/task_base.c
  ${ZZMEDIAX_ROOT}/framework/task_db.c
  ${ZZMEDIAX_ROOT}/framework/task_mng.c
  ${ZZMEDIAX_ROOT}/utils/split_str.c
  ${ZZMEDIAX_ROOT}/utils/utils.c

  ${ZZMEDIAXAPP_SOURCE_DIR}/utils/codec.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/utils/format.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/utils/frame.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/utils/packet.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/utils/stream.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/utils/buffer.c

  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/ffmpeg/ff_cmn.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/ffmpeg/ff_codec.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/ffmpeg/ff_format.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/ffmpeg/ff_frame.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/ffmpeg/ff_packet.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/ffmpeg/ff_stream.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/ffmpeg/ff_buffer.c
  
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/vaapi/va_ctx_mng.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/vaapi/va_utils.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/vaapi/va_surf_mng.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/vaapi/va_render_utils.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/vaapi/va_render_parser.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/vaapi/frame_cmn.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/vaapi/frame_cmn_read.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/vaapi/frame_cmn_write.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/vaapi/frame_reader.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/vaapi/frame_writer.c
  
  ${ZZMEDIAXAPP_SOURCE_DIR}/task/task_1001.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/task/task_2001.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/task/task_2002.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/task/task_2003.c

  ${ZZMEDIAXAPP_SOURCE_DIR}/matrix/matrix_1002.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/matrix/matrix_1003.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/matrix/matrix_2002.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/matrix/matrix_2003.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/matrix/matrix_9001.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/matrix/matrix_9002.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/matrix/matrix_9003.c
  )

LINK_DIRECTORIES(
  ${LIBVA_LIBRARY_DIRS}
  ${LIBDRM_LIBRARY_DIRS}
  ${FFMPEG_LIBRARY_DIRS}
  )

ADD_EXECUTABLE(zzmediaxapp ${SRC_LIST})
TARGET_LINK_LIBRARIES(zzmediaxapp
  ${LIBVA_LIBRARIES}
  ${LIBDRM_LIBRARIES}
  ${X11_LIBRARIES}
  ${FFMPEG_LIBRARIES})
