#!/bin/bash

set -e

docker build . -t qmk
docker run -e keyboard=ergodox_ez -e subproject=mfontani -e keymap=mfontani --rm -v "$(pwd):/qmk:rw" qmk
new_hex="$( date +%s )_ergodox_ez_mfontani.hex"
mv ergodox_ez_mfontani.hex "$new_hex"
teensy_loader_cli --mcu=TEENSY2 -v -w "$new_hex"
