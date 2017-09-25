LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS		:= optional
LOCAL_SRC_FILES			:= uinput.c
LOCAL_MODULE			:= uinput
LOCAL_CPPFLAGS			:= -DANDROID
LOCAL_SHARED_LIBRARIES	:= libc libutils libcutils
LOCAL_C_FLAGS			:= -DDEBUG
LOCAL_C_INCLUDES += $(LOCAL_PATH) $(LOCAL_PATH)/$(KERNEL_DIR)/include
include $(BUILD_EXECUTABLE)
