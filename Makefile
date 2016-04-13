DIR = $(shell pwd)

default: web

# --------------------------------

clean: web-clean android-clean

# --------------------------------

copy:
	rsync -av --delete contents/ src_platform/android/assets/ --exclude='*.ogg'
	rsync -av --delete contents/ src_platform/ios/assets/ --exclude='*.ogg'
	rsync -av --delete contents/ src_platform/web/bin/ --exclude='game.js' --exclude='game.js.mem' --exclude='game.html' --exclude='frame.html'

# --------------------------------

web: web-node

web-node: web-debug copy
	node src_server/node/main.js

web-debug:
	$(MAKE) -C src_platform/web debug

web-release:
	$(MAKE) -C src_platform/web release

web-clean:
	$(MAKE) -C src_platform/web clean

# --------------------------------

android: copy android-debug
	adb install -r src_platform/android/build/outputs/apk/android-all-debug.apk
	adb logcat

android-isIns:
	adb shell pm list package | grep totetero

android-debug:
	cd src_platform/android; ./gradlew assembleDebug

android-release:
	cd src_platform/android; ./gradlew assembleRelease

android-clean:
	cd src_platform/android; ./gradlew clean

# --------------------------------

