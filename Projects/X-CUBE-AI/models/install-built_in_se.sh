#!/bin/bash

echo "Install Built-in TCN for Speech Enhancement  "
cp network.c.se network.c
cp  ../../Dpu/ai_model_config.h.se  ../../Dpu/ai_model_config.h
cp  ../../Dpu/user_mel_tables.c.se  ../../Dpu/user_mel_tables.c
cp  ../../Dpu/user_mel_tables.h.se  ../../Dpu/user_mel_tables.h
