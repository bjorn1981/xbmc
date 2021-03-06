include ../../depends/Makefile.include

OBJS = libxbmad.so
OBJS += libxbnfs.so
OBJS += libxbcurl.so
OBJS += libxbrtmp.so
OBJS += libxbplist.so
OBJS += libxbafpclient.so
OBJS += libxbshairplay.so
OBJS += libxbbluray.so
OBJS += libxbass.so
OBJS += libxbogg.so
OBJS += libxbFLAC.so
OBJS += libxbvorbis.so
OBJS += libxbvorbisfile.so
OBJS += libxbmpeg2.so
OBJS += libxbmpeg2convert.so
OBJS += libxbmodplug.so
OBJS += libxbmp3lame.so

PLATFORM_OBJS =
EXCLUDED_ADDONS = screensaver.rsxs.euphoria visualization.dxspectrum visualization.milkdrop visualization.projectm

# android branding
XBMCAPP = xbmcapp
XBMCLIB = libxbmc.so

UNAME := $(shell uname -sm)
# Host OS
ifneq (,$(findstring Linux,$(UNAME)))
HOST_OS := linux
endif
ifneq (,$(findstring Darwin,$(UNAME)))
HOST_OS := darwin
endif
ifneq (,$(findstring Macintosh,$(UNAME)))
HOST_OS := darwin
endif

XBMCROOT = $(shell cd $(CURDIR)/../../..; pwd)
COPYDIRS = system addons language media
# Conditionally set SDK file locations
# aapt
ifneq ($(wildcard $(SDKROOT)/tools/$(HOST_OS)/aapt)),)
AAPT = $(SDKROOT)/tools/$(HOST_OS)/aapt
else
AAPT = $(SDKROOT)/platform-tools/aapt
endif
# dx
ifneq ($(wildcard $(SDKROOT)/tools/dx)),)
DX = $(SDKROOT)/tools/dx
else
DX = $(SDKROOT)/platform-tools/dx
endif
#zipalign
ifneq ($(wildcard $(SDKROOT)/tools/$(HOST_OS)/zipalign)),)
ZIPALIGN = $(SDKROOT)/tools/$(HOST_OS)/zipalign
else
ZIPALIGN = $(SDKROOT)/tools/zipalign
endif
# Check android.jar location
ifeq ($(wildcard $(SDKROOT)/platforms/$(SDK_PLATFORM)/android.jar)),)
ANDROIDJAR = $(SDKROOT)/platforms/$(SDK_PLATFORM)/android.jar
else
# Using prebuilts/sdk in AOSP tree
SDK_PLATFORM_NO = $(filter-out android,$(subst -, ,$(SDK_PLATFORM)))
ANDROIDJAR = $(SDKROOT)/$(SDK_PLATFORM_NO)/android.jar
endif

GCC_VERSION=$(shell $(CC) -dumpversion)

X86OVERRIDES=XBMC_OVERRIDE_HOST=i686-android-linux XBMC_OVERRIDE_TOOLCHAIN=$(XBMC_X86_TOOLCHAIN)
ARMOVERRIDES=XBMC_OVERRIDE_HOST=arm-linux-androideabi XBMC_OVERRIDE_TOOLCHAIN=$(XBMC_ARM_TOOLCHAIN)

#this fixes a android ndk fuckup where the pathes to 
#prebuilt stuff follow different name shemes for
#arm and x86
ifeq ($(findstring i686,$(CPU)),i686)
ARCH=x86
CPU=x86
endif
ifeq ($(findstring arm,$(CPU)),arm)
ARCH=arm
endif

# Check java version to correct source and target for javac
ifneq ($(shell -java -version 2>&1 | grep - "1.7"),)
JAVAC = javac -source 1.7 -target 1.7
else
JAVAC = javac -source 1.5 -target 1.5
endif

#older ndk x86
GDBPATH=$(NDKROOT)/toolchains/$(ARCH)-$(GCC_VERSION)/prebuilt/gdbserver

ifneq ($(shell test -f $(GDBPATH) && echo 1),1)
#older ndk arm
GDBPATH=$(NDKROOT)/toolchains/$(HOST)-$(GCC_VERSION)/prebuilt/gdbserver
endif

ifneq ($(shell test -f $(GDBPATH) && echo 1),1)
#vanilla ndk >= 8
GDBPATH=$(NDKROOT)/prebuilt/android-$(ARCH)/gdbserver/gdbserver
endif

LOCAL_BRANCH=$(shell git rev-parse --abbrev-ref HEAD)
LOCAL_REV=$(shell git rev-parse --short=7 HEAD)
LOCAL_VERSION=$(shell cd $(CURDIR); grep "android:versionName" xbmc/AndroidManifest.xml | cut -d '=' -f2 | tr -d ">" | sed "s/\"//g" | tr -d " ")

all: package
SRCLIBS = $(addprefix $(PREFIX)/lib/,$(OBJS)) $(addprefix $(PREFIX)/lib/$(SDK_PLATFORM_NO)/,$(PLATFORM_OBJS))
DSTLIBS = $(CPU)/lib/$(XBMC_LIBNAME) $(addprefix $(CPU)/lib/,$(OBJS)) $(addprefix $(CPU)/lib/,$(PLATFORM_OBJS))
libs= $(DSTLIBS)

multi: x86 arm
	@cp images/$(XBMCAPP)-debug-skeleton.apk images/$(XBMCAPP)-debug-multi-unaligned.apk
	@cd xbmc; zip -r -q ../images/$(XBMCAPP)-debug-multi-unaligned.apk lib/ assets
	@jarsigner -sigalg MD5withRSA -digestalg SHA1 -keystore ~/.android/debug.keystore -storepass android images/$(XBMCAPP)-debug-multi-unaligned.apk androiddebugkey
	@$(ZIPALIGN) -f 4 images/$(XBMCAPP)-debug-multi-unaligned.apk $(XBMCROOT)/$(XBMCAPP)-multi-debug.apk
	@rm images/$(XBMCAPP)-debug-multi-unaligned.apk
	@echo "$(XBMCROOT)/$(XBMCAPP)-multi-debug.apk created"

package: extras
	@cp images/$(XBMCAPP)-debug-skeleton.apk images/xbmcapp-debug-$(CPU)-unaligned.apk
	@$(JAVAC) -classpath $(ANDROIDJAR):xbmc/obj -d xbmc/obj -sourcepath xbmc/src xbmc/src/*.java
	@$(JAVAC) -classpath $(ANDROIDJAR):xbmc/obj -d xbmc/obj -sourcepath xbmc/src xbmc/src/org/xbmc/xbmc/*.java
	@$(DX) --dex --output=xbmc/classes.dex xbmc/obj
	@cd xbmc; zip -r -q ../images/$(XBMCAPP)-debug-$(CPU)-unaligned.apk lib/$(CPU) assets classes.dex
	@jarsigner -sigalg MD5withRSA -digestalg SHA1 -keystore ~/.android/debug.keystore -storepass android images/$(XBMCAPP)-debug-$(CPU)-unaligned.apk androiddebugkey
	@$(ZIPALIGN) -f 4 images/$(XBMCAPP)-debug-$(CPU)-unaligned.apk $(XBMCROOT)/$(XBMCAPP)-$(LOCAL_BRANCH)-$(LOCAL_VERSION)_$(LOCAL_REV)_debug.apk
	@rm images/$(XBMCAPP)-debug-$(CPU)-unaligned.apk
	@echo "$(XBMCROOT)/$(XBMCAPP)-$(LOCAL_BRANCH)-$(LOCAL_VERSION)_$(LOCAL_REV)_debug.apk created"

extras: libs
	rm -rf xbmc/assets
	mkdir -p xbmc/assets xbmc/res xbmc/res/raw xbmc/assets/python2.6/lib/
	cp -rfp $(PREFIX)/share/xbmc/* ./xbmc/assets
	find `pwd`/xbmc/assets/ -depth -name ".git" -exec rm -rf {} \;
	find `pwd`/xbmc/assets/system/ -name "*.so" -exec rm {} \;
	find `pwd`/xbmc/assets/addons/skin.*/media/* -depth -not -iname "Textures.xbt" -exec rm -rf {} \;
	find `pwd`/xbmc/assets/system/keymaps/ -depth -name "joystick*.xml" ! -name "joystick.xml" -exec rm {} \;
	@echo "native_arch=$(ARCH)" > xbmc/res/raw/xbmc.properties
	cd xbmc/assets/addons; rm -rf $(EXCLUDED_ADDONS)
	cp -rfp $(PREFIX)/lib/python2.6 xbmc/assets/python2.6/lib/
	cp -fp $(XBMCROOT)/media/Splash.png xbmc/res/drawable/splash.png
	cp -fp media/drawable-mdpi/ic_launcher.png xbmc/res/drawable-mdpi/ic_launcher.png
	cp -fp media/drawable-hdpi/ic_launcher.png xbmc/res/drawable-hdpi/ic_launcher.png
	cp -fp media/drawable-xhdpi/ic_launcher.png xbmc/res/drawable-xhdpi/ic_launcher.png
	cp -fp media/drawable-xxhdpi/ic_launcher.png xbmc/res/drawable-xxhdpi/ic_launcher.png
	cd xbmc/assets/python2.6/lib/python2.6/; rm -rf test config lib-dynload
	mkdir -p tmp/res; $(AAPT) c -S xbmc/res -C tmp/res; cp -r -n xbmc/res tmp/ || true
	$(AAPT) p -f -I $(ANDROIDJAR) -S tmp/res/ -M xbmc/AndroidManifest.xml -F images/$(XBMCAPP)-debug-skeleton.apk -J xbmc/src
	@rm -rf tmp/

libs: $(PREFIX)/lib/xbmc/$(XBMCLIB)
	rm -rf xbmc/lib/$(CPU) xbmc/libs/$(CPU) xbmc/obj/local/$(CPU)
	mkdir -p xbmc/lib/$(CPU) images xbmc/assets/python2.6/lib/ xbmc/libs/$(CPU) xbmc/obj/local/$(CPU)
	cp -fp $(SRCLIBS) xbmc/obj/local/$(CPU)/
	cp -fp $(PREFIX)/lib/xbmc/$(XBMCLIB) xbmc/obj/local/$(CPU)/
	find $(PREFIX)/lib/xbmc/addons -name "*.so" -exec cp -fp {} xbmc/obj/local/$(CPU)/ \;
	find $(PREFIX)/lib/xbmc/system -name "*.so" -exec cp -fp {} xbmc/obj/local/$(CPU)/ \;
	cd xbmc/obj/local/$(CPU)/; find . -name "*.so" -not -name "lib*.so" | sed "s/\.\///" | xargs -I@ mv @ lib@
	cp -fp xbmc/obj/local/$(CPU)/*.so xbmc/lib/$(CPU)/
	$(STRIP) --strip-unneeded xbmc/lib/$(CPU)/*.so
	install -p $(GDBPATH) ./xbmc/libs/$(CPU)/gdbserver
	echo "set solib-search-path ./obj/local/$(CPU)" > ./xbmc/libs/$(CPU)/gdb.setup
	echo "directory $(TOOLCHAIN)/sysroot/usr/include $(NDKROOT)/sources/android/native_app_glue" \
	     "$(NDKROOT)/sources/cxx-stl/gnu-libstdc++/$(GCC_VERSION)/include $(XBMCROOT)  $(PREFIX)/include jni" >> ./xbmc/libs/$(CPU)/gdb.setup
	cp -fp xbmc/libs/$(CPU)/* xbmc/lib/$(CPU)

$(PREFIX)/lib/xbmc$(XBMCLIB): $(SRCLIBS)
	$(MAKE) -C ../../depends/target/xbmc

$(SRCLIBS):

arm: armeabi-v7a

armeabi-v7a: $(XBMC_ARM_TOOLCHAIN) force
	XBMC_OVERRIDE_PLATFORM=$@ $(ARMOVERRIDES) $(MAKE) package

x86: $(XBMC_X86_TOOLCHAIN) force
	XBMC_OVERRIDE_PLATFORM=$@ $(X86OVERRIDES) $(MAKE) package

clean:
	rm -rf images
	rm -rf xbmc/lib
	rm -rf xbmc/libs
	rm -rf xbmc/assets
	rm -rf xbmc/obj
	rm -rf xbmc/res/raw
	rm -rf tmp
	rm -f xbmc/res/drawable/splash.png
	rm -f xbmc/src/R.java
	rm -f xbmc/classes.dex

.PHONY: arm armeabi-v7a x86 force extras libs
