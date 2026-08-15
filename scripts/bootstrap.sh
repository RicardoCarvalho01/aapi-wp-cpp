#!/bin/bash

set -e

mkdir -p third_party

cd third_party

if [ ! -d buildroot ]; then

    git clone \
        --depth 1 \
        --branch 2026.02 \
        https://github.com/buildroot/buildroot.git

fi