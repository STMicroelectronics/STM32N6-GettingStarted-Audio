#!/bin/bash

bin=$1"_"$2".bin"
fsbl="fsbl_fw_lrun_v1.2.0.bin"
weight=$1"_weights.bin"
external_loader="$(dirname "$(which STM32_Programmer_CLI)")/ExternalLoader/MX66UW1G45G_STM32N6570-DK.stldr"

echo "please connect the board and switch BOOT1 to Rigth position"
echo "when done,  press a key to continue ..."
read -n 1 -s
echo "flashing the application "$bin" with weigth "$weight

set -x

STM32_Programmer_CLI -c port=swd mode=HOTPLUG ap=1 --extload $external_loader -w $fsbl 0x70000000
STM32_Programmer_CLI -c port=swd mode=HOTPLUG ap=1 --extload $external_loader -w $bin 0x70100000
STM32_Programmer_CLI -c port=swd mode=HOTPLUG ap=1 --extload $external_loader -w $weight 0x70180000

set +x

echo "please switch BOOT1 to Left position and then power cycle the board"
echo "when done, press a key to continue ..."
read -n 1 -s
echo "Flashing done"
