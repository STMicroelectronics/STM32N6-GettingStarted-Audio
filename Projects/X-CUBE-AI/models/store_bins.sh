#!\bin\bash

pathCubeIde="<PathtoCube IDE>"
pathProg="/plugins/<cube programmer plug-in>/tools/bin"

sign=$pathCubeIde$pathProg"/STM32_SigningTool_CLI.exe"
echo $ 
source build-firmware.sh BM
source build-firmware.sh BM_LP
source build-firmware.sh TX
source build-firmware.sh TX_LP

bin_BM="../../GS/STM32CubeIDE/BM/GS_Audio_N6.bin"
bin_BM_LP="../../GS/STM32CubeIDE/BM_LP/GS_Audio_N6.bin"
bin_TX="../../GS/STM32CubeIDE/TX/GS_Audio_N6.bin"
bin_TX_LP="../../GS/STM32CubeIDE/TX_LP/GS_Audio_N6.bin"
bin_dir="../../../Binaries"

echo "======================="
echo "signing" $bin_BM
$sign -s -bin $bin_BM -nk -t ssbl -hv 2.3 -o $bin_dir/$1_bm.bin
echo "======================="
echo "signing" $bin_BM_LP
$sign -s -bin $bin_BM_LP -nk -t ssbl -hv 2.3 -o $bin_dir/$1_bm_lp.bin
echo "======================="
echo "signing" $bin_TX 
$sign -s -bin $bin_TX -nk -t ssbl -hv 2.3 -o $bin_dir/$1_tx.bin
echo "======================="
echo "signing" $bin_TX-LP
$sign -s -bin $bin_TX_LP -nk -t ssbl -hv 2.3 -o $bin_dir/$1_tx_lp.bin
echo "======================="
echo "copy weights" 
cp network_data.bin $bin_dir/$1"_weights.bin"