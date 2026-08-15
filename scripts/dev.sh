#!/bin/bash

set -e

docker build \
    -t aapi-wp-dev \
    docker/dev

docker run \
    --rm \
    -it \
    -v "$(pwd)":/workspace \
    -w /workspace \
    aapi-wp-dev
