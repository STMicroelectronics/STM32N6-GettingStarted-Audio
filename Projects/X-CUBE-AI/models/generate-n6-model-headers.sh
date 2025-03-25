#!/bin/bash

if [ "$1" == "aed" ]; then 
	arg="--config-path . --config-name user_config_aed.yaml"
elif [ "$1" == "se" ]; then 
	arg="--config-path . --config-name user_config_se.yaml"
else
	arg=""
fi

python GenHeader/GenHeaders.py $arg

cp -r ./GenHeaderOutput/C_Header/*.* ../../Dpu
rm -rf ./GenHeaderOutput
