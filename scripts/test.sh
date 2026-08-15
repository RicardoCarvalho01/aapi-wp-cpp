#!/bin/bash

set -e

preset=${1:-debug}

ctest \
    --preset ${preset} \
    --output-on-failure