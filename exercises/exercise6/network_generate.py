#!/bin/bash
# network_generate.py
# Alessio Burrello <alessio.burrello@unibo.it>
#
# Copyright (C) 2019-2020 University of Bologna
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from dory.ONNX_management import ONNX_management as onnx_m
from dory.Model_deployment import Model_deployment as model_deploy
import os
import argparse
from argparse import RawTextHelpFormatter

def main():
    parser = argparse.ArgumentParser(formatter_class=RawTextHelpFormatter)
    parser.add_argument('--network_dir', default = "./Test_suite_DORY/MobilenetV1/", help = 'directory of the onnx file of the network')
    parser.add_argument('--l1_buffer_size', type=int, default = 35000, help = 'L1 buffer size. IT DOES NOT INCLUDE SPACE FOR STACKS.')
    parser.add_argument('--l2_buffer_size', type=int, default = 400000, help = 'L2 buffer size.')
    parser.add_argument('--master_stack', type=int, default = 4096, help = 'Cluster Core 0 stack')
    parser.add_argument('--slave_stack', type=int, default = 3072, help = 'Cluster Core 1-7 stack')
    parser.add_argument('--Bn_Relu_Bits', type=int, default = 32, help = 'Number of bits for Relu/BN')
    parser.add_argument('--perf_layer', default = 'No', help = 'Yes: MAC/cycles per layer. No: No perf per layer.')
    parser.add_argument('--verbose_level', default = 'Last+Perf_final', help = "None: No_printf.\nPerf_final: only total performance\nCheck_all+Perf_final: all check + final performances \nLast+Perf_final: all check + final performances \nExtract the parameters from the onnx model")
    parser.add_argument('--Mobilenet_correction',  type=int, default = 1, help = '1 for /Test_suite_DORY/MobilenetV1/, 0 otherwise')
    parser.add_argument('--chip', default = 'GAP8v3', help = 'GAP8v2 for fixing DMA issue. GAP8v3 otherise')
    parser.add_argument('--fc_frequency', default = 100000000, help = 'frequency of fabric controller')
    parser.add_argument('--cl_frequency', default = 100000000, help = 'frequency of cluster')
    args = parser.parse_args()

    for files in os.listdir(args.network_dir):
        if 'onnx' in files:
            net = files
        elif '.' not in files:
            for sub_files in files:
                if 'onnx' in files:
                    net = files
    
    PULP_Nodes_Graph = onnx_m('GAP8', args.chip, args.network_dir + net).parameters_from_onnx(100)
    model_deploy('GAP8', args.chip).print_model_network(PULP_Nodes_Graph,
                            100,
                            args.network_dir,
                            100,
                            args.verbose_level,
                            args.perf_layer,
                            args.Mobilenet_correction,
                            args.l1_buffer_size,
                            args.master_stack,
                            args.slave_stack,
                            args.l2_buffer_size,
                            args.fc_frequency,
                            args.cl_frequency,
                            8, 8, 8, args.Bn_Relu_Bits, '8bit'
                            )

if __name__ == '__main__':
    main()
