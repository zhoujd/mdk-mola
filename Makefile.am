### zzmediaxapp configure

AUTOMAKE_OPTIONS = foreign

DUMP_ROOT        = $(top_builddir)
APP_ROOT         = $(top_srcdir)
LIBVA_ROOT       = /usr/include/va
LIBDRM_ROOT      = /usr/include/libdrm


ZZ_CFLAGS = -Wall \
	-DZZ_LIBVA_USE_X11=1 \
	-I$(LIBVA_ROOT) \
	-I$(LIBDRM_ROOT) \
	-I$(APP_ROOT)/zzmediax/include \
	-I$(APP_ROOT)/zzmediax \
	-I$(APP_ROOT)/zzmediax/framework \
	-I$(APP_ROOT)/zzmediax/utils \
	-I$(APP_ROOT) \
	-I$(APP_ROOT)/task \
	-I$(APP_ROOT)/matrix \
	-I$(APP_ROOT)/lib

ZZ_SRC_FILES = \
	zzmediaxapp.c \
	zzmediaxapp_register.c \
	zzmediax/main.c \
	zzmediax/framework/app_base.c \
	zzmediax/framework/dispatch.c \
	zzmediax/framework/event.c \
	zzmediax/framework/help.c \
	zzmediax/framework/matrix_base.c \
	zzmediax/framework/task_base.c \
	zzmediax/framework/task_db.c \
	zzmediax/framework/task_mng.c \
	zzmediax/utils/split_str.c \
	zzmediax/utils/utils.c \
	task/task_1001.c \
	task/task_2001.c \
	matrix/matrix_1002.c \
	matrix/matrix_2002.c \
	matrix/matrix_9001.c \
	matrix/matrix_9002.c \
	lib/frame_cmn.c \
	lib/frame_cmn_read.c \
	lib/frame_cmn_write.c \
	lib/frame_reader.c \
	lib/frame_writer.c \
	lib/va_ctx_mng.c \
	lib/va_utils.c \
	lib/va_render_utils.c \
	lib/va_render_parser.c \
	lib/va_surf_mng.c

bin_PROGRAMS        = zzmediaxapp
zzmediaxapp_LDFLAGS =
zzmediaxapp_LDADD   = -lva -lva-x11 -ldrm_intel -lX11 -lva-drm -ldrm
zzmediaxapp_CFLAGS  = $(ZZ_CFLAGS)
zzmediaxapp_SOURCES = $(ZZ_SRC_FILES)

noinst_LIBRARIES  = libzzmediax.a
libzzmediax_a_SOURCES = $(ZZ_SRC_FILES)
libzzmediax_a_CFLAGS  = $(zzmediaxapp_CFLAGS) -fPIC
libzzmediax_a_LIBADD  = $(zzmediaxapp_LIBADD)
