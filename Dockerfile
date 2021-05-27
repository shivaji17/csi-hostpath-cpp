FROM alpine

LABEL maintainer="Shivaji Sawant"
LABEL description="C++ HostPath Driver"

ARG binary=./build/hostpath

RUN apk update && apk upgrade && apk add util-linix coreutils gcompat libstdc++

COPY ${binary} /hostpath