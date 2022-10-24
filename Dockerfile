FROM ubuntu:22.04

ENV MYPATH /root/filebak

RUN mkdir $MYPATH
COPY . $MYPATH
WORKDIR $MYPATH

# 设置时区
ENV TZ="Asia/Shanghai"

# 构建环境
RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list
RUN apt update
RUN apt install -y build-essential cmake libssl-dev

# 编译安装
RUN rm -rf build && mkdir build
RUN cd build && cmake .. && make && make install