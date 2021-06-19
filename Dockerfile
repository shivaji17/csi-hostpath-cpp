FROM ubuntu

LABEL maintainer="Shivaji Sawant"
LABEL description="C++ HostPath Driver"

ARG binary=./build/hostpath

COPY ${binary} /hostpath
ENTRYPOINT ["/hostpath"]
