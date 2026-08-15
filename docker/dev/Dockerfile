FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
        build-essential \
        clang \
        clang-format \
        clang-tidy \
        cmake \
        ninja-build \
        git \
        gdb \
        python3 \
        ca-certificates && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /workspace