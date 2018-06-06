include config.mk

TARGET = sepcam_ipc_demo

TARGET_CFLAGS +=  -Wall  -g -O2 -DLINUX -Wl,-gc-sections -pthread  -lrt -ldl
STATIC_LIBRARIES_RELY = lib_sepcam_sdk libsepcambase
STATIC_LIBRARIES_RELY += libjson
//STATIC_LIBRARIES_RELY += libmpi

SDK_STATIC_LIBRARIES += $(foreach n, $(STATIC_LIBRARIES_RELY), ./libs/$(n).a)
#$(error $(SDK_STATIC_LIBRARIES))

OBJ := demo/sepcam_ipc_demo.o
COBJ := demo/convert.o
#OBJ += demo/save_bmp.o

all: $(TARGET)


$(TARGET):$(OBJ) $(COBJ)
	 $(CROSS_COMPILE)g++ $(CFLAGS) $(OBJ) $(COBJ) $(SDK_STATIC_LIBRARIES) $(TARGET_CFLAGS) -o $(TARGET)
	@cp -rf $(TARGET) $(TARGET_OUT)/usr/bin
$(OBJ):%.o:%.c
	$(CROSS_COMPILE)g++ -I./include -c -o $@ $<

$(COBJ):%.o:%.cpp 
	$(CROSS_COMPILE)g++ -I./include -c -o $@ $<
clean:
	rm -rf $(TARGET) $(OBJ)
	rm -rf $(TARGET_OUT)/usr/bin/$(TARGET)
