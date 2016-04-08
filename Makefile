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

android:
	cd src_platform/android; ./gradlew
	#adb install src_platform/android/build/outputs/apk/android-all-debug.apk

# --------------------------------

