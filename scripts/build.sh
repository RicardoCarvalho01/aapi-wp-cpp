#!/bin/bash

set -e

preset=${1:-debug}

cmake --build --preset ${preset}