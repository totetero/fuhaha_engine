#!/bin/bash

echo ////////////////////////////////////////////////////////////////
echo ////////////////////////////////////////////////////////////////
echo ////////////////////////////////////////////////////////////////
echo passphraseはtestが良いな
echo ////////////////////////////////////////////////////////////////
echo ////////////////////////////////////////////////////////////////
echo ////////////////////////////////////////////////////////////////

DIR=$(cd $(dirname $0); pwd)
FILENAME1=${DIR}/mysslserver.key
FILENAME2=${DIR}/mysslserver.csr
FILENAME3=${DIR}/mysslserver.crt
FILENAME4=${DIR}/mysslserver.pfx

openssl genrsa -out ${FILENAME1} 2048
openssl req -new -key ${FILENAME1} -out ${FILENAME2} -subj "/C=JP/ST=Tokyo/L=Tokyo/O=Example Ltd./OU=Web/CN=example.com"
openssl x509 -req -signkey ${FILENAME1} -in ${FILENAME2} -out ${FILENAME3} -days 3650
openssl pkcs12 -export -inkey ${FILENAME1} -in ${FILENAME3} -out ${FILENAME4}
