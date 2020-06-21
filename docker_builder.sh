#!/bin/bash

[ ${#} -eq 0 ] && sh ${0} help && exit
[ ${#} -eq 1 ] && [ ${1} = "first" ] && sh ${0} create start install install_node install_emscripten install_android_sdk_ndk put && exit
[ ${#} -eq 1 ] && [ ${1} = "last" ] && sh ${0} stop clear && exit

DOCKER_CONTAINER_NAME_01=docker-fuhaha-builder
DOCKER_LINUX=ubuntu:16.04
EMSCRIPTEN_VERSION=sdk-fastcomp-1.37.27-64bit
ANDROID_SDK_VERSION=r24.4.1
ANDROID_NDK_VERSION=r10e
PROJECT=${PWD##*/}

# makeコマンド このコマンドだけはオプションをつけることができる
[ ${#} -ge 1 ] && [ ${1} = "make" ] && {
	# 最初にファイル転送
	sh ${0} put
	echo -------- make start --------
	# makeコマンド作成と実行
	MAKE_COMMAND="cd /root/${PROJECT}"
	MAKE_COMMAND+=" && export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64"
	MAKE_COMMAND+=" && export ANDROID_HOME=/opt/android-sdk-${ANDROID_SDK_VERSION}"
	MAKE_COMMAND+=" && export ANDROID_NDK_HOME=/opt/android-ndk-${ANDROID_NDK_VERSION}"
	MAKE_COMMAND+=" && . /opt/emsdk/emsdk_env.sh"
	MAKE_COMMAND+=" && make ${@:$((1 + 1))}"
	docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c "${MAKE_COMMAND}"
	echo -------- make exit --------
	echo -------- get start --------
	# 最後にファイル転送
	SYNC_TARGET_01=src_client/platform_web/bin
	SYNC_TARGET_02=src_client/platform_android/build/outputs/apk
	mkdir -p ./${SYNC_TARGET_01}
	mkdir -p ./${SYNC_TARGET_02}
	rsync --blocking-io -e 'docker exec -i' -rltDv ${DOCKER_CONTAINER_NAME_01}:/root/${PROJECT}/${SYNC_TARGET_01}/ ./${SYNC_TARGET_01}/
	rsync --blocking-io -e 'docker exec -i' -rltDv ${DOCKER_CONTAINER_NAME_01}:/root/${PROJECT}/${SYNC_TARGET_02}/ ./${SYNC_TARGET_02}/
	echo -------- get exit --------
	# スクリプトを抜ける
	exit
}

for ARG in "${@}" ; do
	echo -------- ${ARG} start --------
	# 引数解釈
	case ${ARG} in
		status)
			docker network ls && echo '--------' && docker images && echo '--------' && docker ps -a
			;;
		create)
			docker create --name ${DOCKER_CONTAINER_NAME_01} --publish 8080:8080 --interactive --tty ${DOCKER_LINUX} /bin/bash --login
			[ ${?} -gt 0 ] && exit
			;;
		start)
			docker start ${DOCKER_CONTAINER_NAME_01}
			;;
		bash)
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash
			;;
		stop)
			docker stop ${DOCKER_CONTAINER_NAME_01}
			;;
		clear)
			docker rm ${DOCKER_CONTAINER_NAME_01}
			;;
		install)
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'apt-get update'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y rsync'
			;;
		install_node)
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y wget'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y nodejs'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y npm'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'npm cache clean'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'npm install n -g'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'n v9.3.0'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'ln -s /usr/local/bin/node /usr/bin/node'
			;;
		install_emscripten)
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y wget'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y cmake'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y python'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y python2.7'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y nodejs'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y default-jre'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y git-core'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'cd /opt && git clone https://github.com/emscripten-core/emsdk.git'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'cd /opt/emsdk && ./emsdk update-tags'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'cd /opt/emsdk && ./emsdk install '${EMSCRIPTEN_VERSION}
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'cd /opt/emsdk && ./emsdk activate '${EMSCRIPTEN_VERSION}
			;;
		install_android_sdk_ndk)
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y wget'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y expect'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y openjdk-8-jdk'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y lib32stdc++6'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y lib32z1'
			ANDROID_SDK_FILE="android-sdk_${ANDROID_SDK_VERSION}-linux.tgz"
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'cd /opt && wget https://dl-ssl.google.com/android/'${ANDROID_SDK_FILE}
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'cd /opt && tar -zxvf '${ANDROID_SDK_FILE}
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'mv /opt/android-sdk-linux /opt/android-sdk-'${ANDROID_SDK_VERSION}
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c '/opt/android-sdk-'${ANDROID_SDK_VERSION}'/tools/android list sdk --all --extended'
			ANDROID_EXPECT_COMMAND_01='set timeout -1 ; spawn /opt/android-sdk-'${ANDROID_SDK_VERSION}'/tools/android update sdk --no-ui --all --filter'
			ANDROID_EXPECT_COMMAND_02='; expect "Do you accept the license" { send "y\n" ; exp_continue } Downloading { exp_continue } Installing { exp_continue }'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} expect -c "${ANDROID_EXPECT_COMMAND_01} android-23 ${ANDROID_EXPECT_COMMAND_02}"
			docker exec -it ${DOCKER_CONTAINER_NAME_01} expect -c "${ANDROID_EXPECT_COMMAND_01} build-tools-23.0.2 ${ANDROID_EXPECT_COMMAND_02}"
			docker exec -it ${DOCKER_CONTAINER_NAME_01} expect -c "${ANDROID_EXPECT_COMMAND_01} extra-google-m2repository ${ANDROID_EXPECT_COMMAND_02}"
			docker exec -it ${DOCKER_CONTAINER_NAME_01} expect -c "${ANDROID_EXPECT_COMMAND_01} extra-android-m2repository ${ANDROID_EXPECT_COMMAND_02}"
			ANDROID_NDK_FILE="android-ndk-${ANDROID_NDK_VERSION}-linux-x86_64.bin"
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'cd /opt && wget https://dl-ssl.google.com/android/ndk/'${ANDROID_NDK_FILE}
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'cd /opt && chmod +x '${ANDROID_NDK_FILE}
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'cd /opt && ./'${ANDROID_NDK_FILE}
			;;
		sync_put|put)
			RSYNC_SRC=./
			RSYNC_DST=${DOCKER_CONTAINER_NAME_01}:/root/${PROJECT}/
			RSYNC_COMMAND="rsync --blocking-io -e 'docker exec -i' --exclude='.git' --filter=':- .gitignore' -rltDv"
			eval ${RSYNC_COMMAND} ${RSYNC_SRC} ${RSYNC_DST}
			;;
		sync_get|get)
			RSYNC_SRC=${DOCKER_CONTAINER_NAME_01}:/root/${PROJECT}/
			RSYNC_DST=./
			RSYNC_COMMAND="rsync --blocking-io -e 'docker exec -i' --exclude='.git' --filter=':- .gitignore' -rltDv"
			eval ${RSYNC_COMMAND} ${RSYNC_SRC} ${RSYNC_DST}
			;;
		restore)
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'rm -rf /root/'${PROJECT}/src_client/fuhahaEngine
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'rm -rf /root/'${PROJECT}/src_client/main
			;;
		serve)
			COMMAND="cd /root/${PROJECT}"
			COMMAND+=" && node src_server/node/main.js"
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c "${COMMAND}"
			;;
		browse)
			DOCKER_IP=$(docker inspect -f '{{.NetworkSettings.IPAddress}}' ${DOCKER_CONTAINER_NAME_01})
			DOCKER_HOSTIP=$(docker inspect -f '{{(index (index .NetworkSettings.Ports "8080/tcp") 0).HostIp}}' ${DOCKER_CONTAINER_NAME_01})
			DOCKER_HOSTPORT=$(docker inspect -f '{{(index (index .NetworkSettings.Ports "8080/tcp") 0).HostPort}}' ${DOCKER_CONTAINER_NAME_01})
			open http://${DOCKER_HOSTIP}:${DOCKER_HOSTPORT}
			;;
		help)
			echo create or status or start or install or put or get or restore or make or serve or browse or bash or stop or clear
			;;
		*)
			echo nothing to do
	esac
	echo -------- ${ARG} exit --------
done

