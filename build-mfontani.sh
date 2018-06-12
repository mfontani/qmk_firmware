#!/bin/bash

set -e

docker build . -t qmk
docker run -e keyboard=ergodox_ez -e subproject=mfontani -e keymap=mfontani --rm -v "$(pwd):/qmk:rw" qmk
mv ergodox_ez_mfontani.hex "$( date +%s )_ergodox_ez_mfontani.hex"
