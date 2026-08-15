#!/bin/bash

set -e

find src \
    -name "*.cpp" \
| xargs clang-tidy \
    -p build/debug