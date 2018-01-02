#!/bin/bash

[ $# -eq 0 ] && sh $0 help && exit
[ $# -eq 1 ] && [ $1 = "first" ] && sh $0 create start install put && exit
[ $# -eq 1 ] && [ $1 = "last" ] && sh $0 stop clear && exit

DOCKER_CONTAINER_NAME_01=docker-fuhaha-builder
DOCKER_LINUX=ubuntu:16.04
PROJECT=fuhaha_engine

# makeコマンド
[ $# -ge 1 ] && [ $1 = "make" ] && {
	# 最初にファイル転送
	rsync --blocking-io -e 'docker exec -i' -rltDv ./ ${DOCKER_CONTAINER_NAME_01}:/root/${PROJECT}/
	# makeコマンド作成
	COMMAND=":"
	COMMAND+=" && . ~/emsdk-portable/emsdk_env.sh"
	COMMAND+=" && cd /root/${PROJECT}"
	COMMAND+=" && ${@}"
	# makeコマンド実行
	docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c "${COMMAND}"
	# スクリプトを抜ける
	exit
}

for ARG in "$@" ; do
	echo -------- $ARG start --------
	# 引数解釈
	case $ARG in
		status)
			docker network ls && echo '--------' && docker images && echo '--------' && docker ps -a
			;;
		create)
			docker create --name ${DOCKER_CONTAINER_NAME_01} --publish 8080:8080 --interactive --tty ${DOCKER_LINUX} /bin/bash --login
			[ $? -gt 0 ] && exit
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
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y wget'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y rsync'
			# install node
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y nodejs'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y npm'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'npm cache clean'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'npm install n -g'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'n v9.3.0'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'ln -s /usr/local/bin/node /usr/bin/node'
			# install emscripten
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y cmake'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y python2.7'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y nodejs'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y default-jre'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'DEBIAN_FRONTEND=noninteractive apt-get install -y git-core'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'cd /root/ && wget https://s3.amazonaws.com/mozilla-games/emscripten/releases/emsdk-portable.tar.gz'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'cd /root/ && tar -zxvf emsdk-portable.tar.gz'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'cd /root/emsdk-portable && ./emsdk update'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'cd /root/emsdk-portable && ./emsdk install sdk-1.37.27-64bit'
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c 'cd /root/emsdk-portable && ./emsdk activate sdk-1.37.27-64bit'
			;;
		sync_put|put)
			rsync --blocking-io -e 'docker exec -i' -rltDv ./ ${DOCKER_CONTAINER_NAME_01}:/root/${PROJECT}/
			;;
		sync_get|get)
			rsync --blocking-io -e 'docker exec -i' -rltDv ${DOCKER_CONTAINER_NAME_01}:/root/${PROJECT}/ ./
			;;
		web_debug)
			COMMAND="cd /root/${PROJECT} && . ~/emsdk-portable/emsdk_env.sh && make copy && make -C src_client/platform_web debug"
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c "${COMMAND}"
			;;
		web_release)
			COMMAND="cd /root/${PROJECT} && . ~/emsdk-portable/emsdk_env.sh && make copy && make -C src_client/platform_web release"
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c "${COMMAND}"
			;;
		web_clean)
			COMMAND="cd /root/${PROJECT} && make -C src_client/platform_web clean"
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c "${COMMAND}"
			;;
		serve)
			COMMAND="cd /root/${PROJECT} && node src_server/node/main.js"
			docker exec -it ${DOCKER_CONTAINER_NAME_01} /bin/bash -c "${COMMAND}"
			;;
		browse)
			DOCKER_IP=$(docker inspect -f '{{.NetworkSettings.IPAddress}}' ${DOCKER_CONTAINER_NAME_01})
			DOCKER_HOSTIP=$(docker inspect -f '{{(index (index .NetworkSettings.Ports "8080/tcp") 0).HostIp}}' ${DOCKER_CONTAINER_NAME_01})
			DOCKER_HOSTPORT=$(docker inspect -f '{{(index (index .NetworkSettings.Ports "8080/tcp") 0).HostPort}}' ${DOCKER_CONTAINER_NAME_01})
			open http://${DOCKER_HOSTIP}:${DOCKER_HOSTPORT}
			;;
		help)
			echo create or status or start or install or put or get or web_debug or make or web_release or web_clean or serve or browse or bash or stop or clear
			;;
		*)
			echo nothing to do
	esac
	echo -------- $ARG exit --------
done

