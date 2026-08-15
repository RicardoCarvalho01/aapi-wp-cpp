FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
        build-essential \
        bc \
        bison \
        flex \
        cpio \
        rsync \
        unzip \
        wget \
        git \
        file \
        cmake \
        ninja-build \
        python3 \
        libssl-dev \
        xz-utils \
        ca-certificates && \
    apt-get clean

WORKDIR /workspace