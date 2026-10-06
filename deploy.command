#!/bin/bash
# Publishes a new firmware for the weather station (OTA update, see README).
#
# Put this file into the web server folder that OTA_BASE_URL in Settings27.h
# points to, and set SKETCH below to your sketch folder.
#
# 1. Arduino IDE: Sketch -> Export Compiled Binary
# 2. Start this script: double-click in Finder (macOS) or "bash deploy.command" (Linux)
#
# It takes the newest *.ino.bin from the sketch's build folder (or one copied
# by hand into this folder) and publishes it as firmware.bin + firmware.md5.
# The station picks it up on its next wake-up. The .md5 file is written last:
# the station only starts an update once the binary is completely copied.

SKETCH="$HOME/Documents/Arduino/Solar_WiFi_Weather_Station_v2_7"    # <-- adjust to your sketch folder

set -e
TARGET="$(cd "$(dirname "$0")" && pwd)"

if [ "$TARGET" = "$SKETCH" ]; then
  echo "Copy deploy.command into the folder of your web server and start it there."
  exit 1
fi

BIN=""
for f in "$SKETCH"/build/*/*.ino.bin "$TARGET"/*.ino.bin; do
  [ -f "$f" ] || continue
  if [ -z "$BIN" ] || [ "$f" -nt "$BIN" ]; then BIN="$f"; fi
done
if [ -z "$BIN" ]; then
  echo "No exported binary found in $SKETCH/build or in this folder."
  echo "Arduino IDE: Sketch -> Export Compiled Binary"
  exit 1
fi

rm -f "$TARGET/firmware.md5"
cp "$BIN" "$TARGET/firmware.bin"
if command -v md5sum >/dev/null 2>&1; then
  md5sum "$TARGET/firmware.bin" | cut -d ' ' -f 1 > "$TARGET/firmware.md5"
else
  md5 -q "$TARGET/firmware.bin" > "$TARGET/firmware.md5"
fi

echo "Published: $BIN"
echo "MD5:       $(cat "$TARGET/firmware.md5")"
