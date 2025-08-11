#!/bin/bash

if [ "$#" -lt 3 ]; then
  echo "You need to provide 3 arguments:"
  echo "first argument is the model file (xxx.onnx)"
  echo "second is the type of model (se/aed )"
  echo "third is build config [BM/BM_LP/FREERTOS/FREERTOS_LP]"
fi

echo "please connect the board and switch BOOT1 to Right position"
echo "when done,  press a key to continue ..."
read -n 1 -s

echo "==== Generating model for N6 ===="
source generate-n6-model.sh $1

echo "==== generate and install headers ===="
source generate-n6-model-headers.sh $2

echo "==== building FW ===="
source build-firmware.sh $3

echo "==== Flashing FW ===="
source sign-and-flash-model.sh $3

echo "please power cycle the board after switching BOOT1 to Left position"