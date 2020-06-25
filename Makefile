DIR = $(shell pwd)
IDENTIFIER = com.example.fuhaha

default: web

# --------------------------------

clean: web-clean and-clean ios-clean

debug: web-debug and-debug ios-debug

# --------------------------------

copy: copy-web copy-and copy-ios

copy-web:
	rsync -av --delete contents/ src_client/platform_web/bin/ --exclude='game.js' --exclude='game.js.mem' --exclude='game.wasm' --exclude='game.html' --exclude='frame.html'

copy-and:
	rsync -av --delete contents/ src_client/platform_android/assets/ --exclude='*.ogg'

copy-ios:
	rsync -av --delete contents/ src_client/platform_ios/assets/ --exclude='*.ogg'

# --------------------------------

web: copy-web web-debug web-node

web-cert:
	sh src_server/test/createCertificate.sh

web-node:
	node src_server/node/main.js
	#node src_server/test/fileServeHttps

web-debug:
	$(MAKE) -C src_client/platform_web debug

web-release: copy-web
	$(MAKE) -C src_client/platform_web release

web-clean:
	$(MAKE) -C src_client/platform_web clean

# --------------------------------

and: copy-and and-debug and-install

and-install:
	adb install -r src_client/platform_android/build/outputs/apk/platform_android-all-debug.apk
	adb logcat

and-check-install:
	adb shell pm list package | grep $(IDENTIFIER)

and-debug:
	cd src_client/platform_android; ./gradlew assembleDebug
	ls src_client/platform_android/build/outputs/apk/platform_android-all-debug.apk

and-release: copy-and
	cd src_client/platform_android; ./gradlew assembleRelease
	ls src_client/platform_android/build/outputs/apk/platform_android-all-release.apk

and-clean:
	cd src_client/platform_android; ./gradlew clean

# --------------------------------
 
ios-debug:
	xcodebuild build -project src_client/platform_ios/fuhaha.xcodeproj -scheme fuhaha -sdk iphonesimulator -configuration Debug

ios-clean:
	xcodebuild clean -project src_client/platform_ios/fuhaha.xcodeproj -scheme fuhaha -sdk iphonesimulator -configuration Debug

# --------------------------------

