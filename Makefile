DIR = $(shell pwd)
IDENTIFIER = com.example.fuhaha

default: web

# --------------------------------

clean: web-clean android-clean ios-clean

debug: web-debug android-debug ios-debug

# --------------------------------

copy:
	rsync -av --delete contents/ src_platform/android/assets/ --exclude='*.ogg'
	rsync -av --delete contents/ src_platform/ios/assets/ --exclude='*.ogg'
	rsync -av --delete contents/ src_platform/web/bin/ --exclude='game.js' --exclude='game.js.mem' --exclude='game.html' --exclude='frame.html'

# --------------------------------

web: web-node

web-node: copy web-debug
	node src_server/node/main.js

web-debug:
	$(MAKE) -C src_platform/web debug

web-release: copy
	$(MAKE) -C src_platform/web release

web-clean:
	$(MAKE) -C src_platform/web clean

# --------------------------------

android: copy android-debug
	adb install -r src_platform/android/build/outputs/apk/android-all-debug.apk
	adb logcat

android-isIns:
	adb shell pm list package | grep $(IDENTIFIER)

android-debug:
	cd src_platform/android; ./gradlew assembleDebug

android-release: copy
	cd src_platform/android; ./gradlew assembleRelease
	ls src_platform/android/build/outputs/apk/android-all-release.apk

android-clean:
	cd src_platform/android; ./gradlew clean

# --------------------------------

ios: copy ios-debug
	open /Applications/Xcode.app/Contents/Developer/Applications/Simulator.app
	xcrun simctl install booted src_platform/ios/build/Debug-iphonesimulator/fuhaha.app
	xcrun simctl launch booted $(IDENTIFIER)
 
ios-debug:
	xcodebuild build -project src_platform/ios/fuhaha.xcodeproj -scheme fuhaha -sdk iphonesimulator -configuration Debug

ios-clean:
	xcodebuild clean -project src_platform/ios/fuhaha.xcodeproj -scheme fuhaha -sdk iphonesimulator -configuration Debug

# --------------------------------

