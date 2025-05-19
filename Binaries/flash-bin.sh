#!\bin\bash

pathCubeIde="<Path_to_cube ide>"
pathProg="<Path_to_cube_programmer_plug_in>/tools/bin"
el=$pathCubeIde$pathProg"/ExternalLoader/MX66UW1G45G_STM32N6570-DK.stldr"
prog=$pathCubeIde$pathProg"/STM32_Programmer_CLI.exe"

bin=$1"_"$2".bin"
fsbl="fsbl_fw_lrun_v1.2.0.bin"
weight=$1"_weights.bin"

echo "please connect the board and switch BOOT1 to Rigth position" 
echo "when done,  press a key to continue ..."
read -n 1 -s
echo "flashing the application "$bin" with weigth "$weight

$prog -c port=swd mode=HOTPLUG ap=1 --extload $el -w $fsbl 0x70000000
$prog -c port=swd mode=HOTPLUG ap=1 --extload $el -w $bin 0x70100000
$prog -c port=swd mode=HOTPLUG ap=1 --extload $el -w $weight 0x70180000

echo "please power cycle the board after switching BOOT1 to Left position" 