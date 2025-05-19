# /*---------------------------------------------------------------------------------------------
#  * Copyright (c) 2022-2025 STMicroelectronics.
#  * All rights reserved.
#  * This software is licensed under terms that can be found in the LICENSE file in
#  * the root directory of this software component.
#  * If no LICENSE file comes with this software, it is provided AS-IS.
#  *--------------------------------------------------------------------------------------------*/

import hydra
import os
import argparse

from omegaconf import DictConfig, OmegaConf
from gen_h_file import gen_h_user_file
from lookup_tables_generator import generate_mel_LUT_files
from lookup_tables_generator import generate_stft_LUT_files

@hydra.main(version_base=None, config_path="", config_name="user_config")
def main(cfg: DictConfig) -> None:
    print("Generating C mode configuration file for Getting Started...")
    gen_h_user_file(cfg)
    print("Done")

    print("Generating C look-up tables files for Getting Started...")
    if cfg.model.model_type != "STFTTCNN" :
        generate_mel_LUT_files(cfg)
    else:
        generate_stft_LUT_files(cfg)
    print("Done")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--config-path', type=str, default='', help='Path to folder containing configuration file')
    parser.add_argument('--config-name', type=str, default='user_config', help='name of the configuration file')

    # Add arguments to the parser
    parser.add_argument('params', nargs='*',
                        help='List of parameters to over-ride in config.yaml')
    args = parser.parse_args()

    # Call the main function
    main()

