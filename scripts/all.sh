#!/bin/bash

set -e

scripts/configure.sh release

scripts/build.sh release

scripts/test.sh release

scripts/check-format.sh

scripts/tidy.sh