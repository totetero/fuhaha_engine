DIR = $(shell pwd)

default: web

# --------------------------------

clean:
	$(MAKE) -C src_platform/web clean
	cd src_platform/android; ./gradlew clean

# --------------------------------

web:
	$(MAKE) -C src_platform/web build

# --------------------------------

android: android-debug
	adb install -r src_platform/android/build/outputs/apk/android-all-debug.apk
	adb logcat

android-isIns:
	adb shell pm list package | grep totetero

android-debug:
	cd src_platform/android; ./gradlew assembleDebug

android-release:
	cd src_platform/android; ./gradlew assembleRelease

# --------------------------------

