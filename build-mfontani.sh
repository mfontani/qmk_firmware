#!/bin/bash

set -e

docker build . -t qmk
docker run --rm -v "$(pwd):/qmk_firmware:rw" qmk make ergodox_ez:mfontani
new_hex="$( date +%s )_ergodox_ez_mfontani.hex"
mv ergodox_ez_mfontani.hex "$new_hex"
teensy_loader_cli --mcu=TEENSY2 -v -w "$new_hex"
