#!/bin/bash

set -e

find inc src tests \
    \( -name "*.hpp" -o -name "*.cpp" \) \
| xargs clang-format -i