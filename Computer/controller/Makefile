# Compiler and linker options
LIBS += -lSDL -pthread -lm
INCLUDES += -I. -I../common/ -I/usr/include/SDL
OPTS += -Wall -g
CC ?= gcc

# Source file layout
BINARY ?= controller
LOCAL_OBJ = controller.o joystick.o 
LOCAL_OBJ_FENRIR = controller_events_fenrir.o

COMMON = ../common
COMMON_OBJ = robot_comm.o robot_log.o \
		 robot_queue.o \
		 timer.o \
		 profile.o

# Begin derived variables
LOCAL_SRC = $(LOCAL_OBJ:%.o=%.c)
COMMON_SRC = $(COMMON_OBJ:%.o=$(COMMON)/%.c)
CFLAGS += $(INCLUDES) $(OPTS) -I/usr/local/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
SRC = $(LOCAL_SRC) $(COMMON_SRC)
OBJ = $(LOCAL_OBJ) $(COMMON_OBJ)
OBJ_FENRIR = $(OBJ) $(LOCAL_OBJ_FENRIR)
.PHONY: all
all: $(BINARY)-fenrir

.PHONY: fenrir
colonel: $(BINARY)-fenrir

$(BINARY)-fenrir: $(OBJ_FENRIR)
	$(CC) $(CFLAGS) $(OPTS) $(OBJ_FENRIR) -o $(BINARY)-fenrir $(LIBS)

%.o: $(COMMON)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	-rm -f $(OBJ) $(OBJ_ROSLUND) $(OBJ_COLONEL) $(OBJ_FENRIR) $(BINARY)-roslund $(BINARY)-colonel $(BINARY)-fenrir $(BINARY)
