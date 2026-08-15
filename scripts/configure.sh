#!/bin/bash

set -e

preset=${1:-debug}

cmake --preset ${preset}