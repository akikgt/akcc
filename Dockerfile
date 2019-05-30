FROM ubuntu:19.04

RUN apt update 
RUN apt install -y gcc make git binutils libc6-dev

# RUN mkdir /usr/src
WORKDIR /usr/src