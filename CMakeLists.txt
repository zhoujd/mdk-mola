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
  -DZZ_LIBVA_USE_X11=0
  -DZZ_DEBUG=0
  )

SET(ZZMEDIAX_ROOT ${ZZMEDIAXAPP_SOURCE_DIR}/zzmediax)

INCLUDE_DIRECTORIES (
  /usr/include/va
  /usr/include/libdrm
  ${ZZMEDIAXAPP_SOURCE_DIR}
  ${ZZMEDIAXAPP_SOURCE_DIR}/zzmediax
  ${ZZMEDIAXAPP_SOURCE_DIR}/zzmediax/include
  ${ZZMEDIAXAPP_SOURCE_DIR}/zzmediax/framework
  ${ZZMEDIAXAPP_SOURCE_DIR}/zzmediax/utils
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
  
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/va_ctx_mng.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/va_utils.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/va_surf_mng.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/va_render_utils.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/va_render_parser.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/frame_cmn.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/frame_cmn_read.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/frame_cmn_write.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/frame_reader.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/lib/frame_writer.c

  ${ZZMEDIAXAPP_SOURCE_DIR}/task/task_1001.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/task/task_2001.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/task/task_2002.c
  
  ${ZZMEDIAXAPP_SOURCE_DIR}/matrix/matrix_1002.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/matrix/matrix_2002.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/matrix/matrix_9001.c
  ${ZZMEDIAXAPP_SOURCE_DIR}/matrix/matrix_9002.c
  )

LINK_DIRECTORIES(
  /opt/hdr/iHD/vpg_driver/lib
  )

ADD_EXECUTABLE(zzmediaxapp ${SRC_LIST})
TARGET_LINK_LIBRARIES(zzmediaxapp va va-x11 drm_intel X11 va-drm drm)
