FROM ubuntu:19.04

RUN apt update 
RUN apt install -y gcc make git binutils libc6-dev
RUN apt install -y gdb

# RUN mkdir /usr/src
WORKDIR /usr/src