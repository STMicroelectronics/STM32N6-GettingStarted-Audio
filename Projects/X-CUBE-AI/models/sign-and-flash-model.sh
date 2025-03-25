#!\bin\bash

pathCubeIde="<PathtoCube IDE>"
pathProg="/plugins/<cube programmer plug-in>/tools/bin"

el=$pathCubeIde$pathProg"/ExternalLoader/MX66UW1G45G_STM32N6570-DK.stldr"
prog=$pathCubeIde$pathProg"/STM32_Programmer_CLI.exe"
sign=$pathCubeIde$pathProg"/STM32_SigningTool_CLI.exe"

bin="../../GS/STM32CubeIDE/"$1"/GS_Audio_N6.bin"
fsbl="../../../Binaries/fsbl_fw_lrun_v1.2.0.bin"
weight="network_data.bin"

$sign -s -bin $bin -nk -t ssbl -hv 2.3 -o aed_bm.bin
$prog -c port=swd mode=HOTPLUG ap=1 --extload $el -w $fsbl 0x70000000
$prog -c port=swd mode=HOTPLUG ap=1 --extload $el -w aed_bm.bin 0x70100000
$prog -c port=swd mode=HOTPLUG ap=1 --extload $el -w $weight 0x70180000
