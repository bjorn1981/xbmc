# Android makefile to build xbmc as part of an Android Build


ifneq ($(BUILD_WITH_COLORS),0)
    CL_RED="\033[31m"
    CL_GRN="\033[32m"
    CL_YLW="\033[33m"
    CL_BLU="\033[34m"
    CL_MAG="\033[35m"
    CL_CYN="\033[96m"
    CL_RST="\033[0m"
endif

LOCAL_PATH := $(shell pwd)

TARGET_XBMC_SRC   := $(LOCAL_PATH)
TARGET_ANDROID_SRC := //elmo/source/jellybean

# User must set TOOLCHAIN, SDK_PATH, and NDK_PATH to their desired directories for compilation
# Examples seen below

DEPS_FULL         := $(LOCAL_PATH)/tools/depends
TOOLCHAIN         := $(TARGET_ANDROID_SRC)/prebuilts/gcc/linux-x86/arm/android-14
SDK_PATH          := $(TARGET_ANDROID_SRC)/prebuilts/sdk
NDK_PATH          := $(TARGET_ANDROID_SRC)/ndk
PREFIX            := $(LOCAL_PATH)/xbmc-deps
TARBALLS          := $(LOCAL_PATH)/xbmc-tarballs
PLATFORM          := android-17

define reset
	@echo ${CL_RST}
endef

define bootstrap
	# bootstrap
	@echo ${CL_RED}
	$(shell cd $(DEPS_FULL); ./bootstrap)
	@echo ${CL_RST}
endef

define configure
	# configure
	@echo ${CL_YLW}
	cd $(DEPS_FULL) && ./configure --with-toolchain=$(TOOLCHAIN) --host=arm-linux-androideabi --with-sdk-path=$(SDK_PATH) --with-ndk=$(NDK_PATH) --prefix=$(PREFIX) --with-tarballs=$(TARBALLS)
	@echo ${CL_RST}
endef

define make-deps
	# Make xbmc dependencies
	@echo ${CL_GRN}
	$(MAKE) -C $(DEPS_FULL)
	@echo ${CL_RST}
endef

define make-xbmc-module
	# Make libxbmc
	@echo ${CL_CYN}
	$(MAKE) -C $(DEPS_FULL)/target/xbmc
	@echo ${CL_RST}
endef

define make-xbmc
	# Make xbmc
	@echo ${CL_BLU}
	$(MAKE) -j8
	@echo ${CL_RST}
endef

define make-xbmc-apk
	# Make the android apk
	@echo ${CL_MAG}
	$(MAKE) -j8 apk
	@echo ${CL_RST}
endef

define install
	# adb install -r new apk
	@echo ${CL_YLW}
	@$(shell adb install -r `find . -name xbmcapp*` > /dev/null 2>&1)
	@echo ${CL_RST}
endef

define rm-previous
	# remove previous apk
	@rm -rf xbmcapp*
endef

all:
	$(bootstrap)
	$(configure)
	$(make-deps)
	$(make-xbmc-module)
	$(make-xbmc)
	$(make-xbmc-apk)

bootstrap:
	$(bootstrap)

configure: 
	$(configure)

dependencies:
	$(bootstrap)
	$(configure)
	$(make-deps)

module:
	$(make-xbmc-module)

xbmc:
	$(make-xbmc)

apk:
	$(make-xbmc)
	$(rm-previous)
	$(make-xbmc-apk)

install:
	$(install)

clobber:
	make clean
	git clean -xfd

reset:
	$(reset)
