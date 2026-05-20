#!/usr/bin/env bash
set -euo pipefail

: "${VillageSQL_BUILD_DIR:?VillageSQL_BUILD_DIR must be set}"

mkdir -p build
cmake -S . -B build -DVillageSQL_BUILD_DIR="$VillageSQL_BUILD_DIR"
cmake --build build -- -j "$(( $(getconf _NPROCESSORS_ONLN) - 2 ))"
cmake --install build
