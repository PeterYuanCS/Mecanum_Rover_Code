#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."

# Compile every standalone sketch with the same board target as the rover.
for sketch in sketches/*/*.ino diagnostics/*/*.ino; do
  printf '\nCompiling %s\n' "$sketch"
  arduino-cli compile --fqbn esp32:esp32:esp32s3 --warnings all "$(dirname "$sketch")"
done
