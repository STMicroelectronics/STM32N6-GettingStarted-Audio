#!/bin/bash

pathCubeIde="<PathtoCube IDE>/STM32CubeIDE"
project="file://<Path_to_Project>/GS_Audio_N6/Projects/GS/STM32CubeIDE"

conf=$1 # BM or BM_LP or FREERTOS or FREERTOS_LP

buildconf="GS_Audio_N6/"$conf
cubeIDE=$pathCubeIde/stm32cubeidec.exe
app="org.eclipse.cdt.managedbuilder.core.headlessbuild"
tmpDir="/C/ST/Temp/cubeIDE_WS"
rm -Rf $tmpDir

if [ "$#" -lt 1 ]; then
  echo "You need to provide 1 argument:"
  echo "the build config [BM/BM_LP/FREERTOS/FREERTOS_LP]"
fi

$cubeIDE --launcher.suppressErrors -nosplash -application $app -import $project -data $tmpDir -cleanBuild $buildconf
