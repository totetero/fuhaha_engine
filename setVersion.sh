#!/bin/bash

IDENTIFIER=com.example.fuhaha
VERSIONCODE=1
VERSIONNAME=0.0.1

# --------------------------------

# android id設定
sed -i '' -e 's/^fuhahaApplicationId=.*/fuhahaApplicationId='${IDENTIFIER}'/g' src_platform/android/gradle.properties

# ios id設定
sed -i '' -e 's/PRODUCT_BUNDLE_IDENTIFIER = .*;/PRODUCT_BUNDLE_IDENTIFIER = '${IDENTIFIER}';/g' src_platform/ios/fuhaha.xcodeproj/project.pbxproj

# android version設定
sed -i '' -e 's/^fuhahaVersionCode=.*/fuhahaVersionCode='${VERSIONCODE}'/g' src_platform/android/gradle.properties
sed -i '' -e 's/^fuhahaVersionName=.*/fuhahaVersionName='${VERSIONNAME}'/g' src_platform/android/gradle.properties

# ios version設定
cat src_platform/ios/fuhaha/Info.plist | perl -e '
	$true = 1;
	$false = 0;
	$isCode = $false;
	$isName = $false;
	while($line = <STDIN>){
		if($isCode){
			$isCode = $false;
			$line =~ s/<string>.*<\/string>/<string>'${VERSIONCODE}'<\/string>/;
		}elsif($isName){
			$isName = $false;
			$line =~ s/<string>.*<\/string>/<string>'${VERSIONNAME}'<\/string>/;
		}elsif($line =~ /CFBundleShortVersionString/){
			$isName = $true;
		}elsif($line =~ /CFBundleVersion/){
			$isCode = $true;
		}
		print $line
	}
' > temp.txt
mv temp.txt src_platform/ios/fuhaha/Info.plist

# --------------------------------

