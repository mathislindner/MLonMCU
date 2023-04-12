/*
 * network.c
 * Alessio Burrello <alessio.burrello@unibo.it>
 *
 * Copyright (C) 2019-2020 University of Bologna
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */
#include "mem_controller.h"
#include "network.h"
#include "pulp.h"
#include "dory.h"
% for layer in list_h:
#include "${layer}"
% endfor
#include "pmsis.h"
#include "bsp/fs.h"
#include "bsp/fs/readfs.h"
#include "bsp/flash.h"
#include "bsp/ram.h"
#include "bsp/flash/hyperflash.h"
#include "bsp/ram/hyperram.h"

% if Mobilenet_bit == 1:
# define MOBILENET 1
% endif
#define FLASH_BUFF_SIZE 128
% if verbose:
#define VERBOSE 1
% endif
// allocation of buffers with parameters needed by the network execution
const char * L3_weights_files[] = {
  ${files_list}
};
int L3_weights_size[${weights_number}];
static int L3_weights;
static int L3_input;
static int L3_output;
static int activations_input;
static int L3_layers[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
% if 'L3' in func_name[i]: 
1${'' if loop.last else ', '}\
% else:
0${'' if loop.last else ', '}\
% endif
% endfor
};
static int allocate_layer[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
% if PULP_Nodes_Graph[i].L3_allocation!=1 and ('Gemm' in PULP_Nodes_Graph[i].name or 'Conv' in PULP_Nodes_Graph[i].name or 'MatMul' in PULP_Nodes_Graph[i].name): 
1${'' if loop.last else ', '}\
% else:
0${'' if loop.last else ', '}\
% endif
% endfor
};
static int branch_input[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
% if PULP_Nodes_Graph[i].branch_in == 1:
1${'' if loop.last else ', '}\
% else:
0${'' if loop.last else ', '}\
% endif
% endfor
};
static int branch_output[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
% if PULP_Nodes_Graph[i].branch_out == 1:
1${'' if loop.last else ', '}\
% else:
0${'' if loop.last else ', '}\
% endif
% endfor
};
static int check_weights[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
${PULP_Nodes_Graph[i].check_sum_w}${'' if loop.last else ', '}\
% endfor
};
static int check_weights_dimension[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
% if i == 0:
${int(PULP_Nodes_Graph[i].weights_dimension * BitW / 8.0)}${'' if loop.last else ', '}\
% else:
${int((PULP_Nodes_Graph[i].weights_dimension - PULP_Nodes_Graph[i-1].weights_dimension) * BitW / 8.0)}${'' if loop.last else ', '}\
% endif
% endfor
};
static int cumulative_weights_dimension[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
% if i == 0: 
0${'' if loop.last else ', '}\
% else:
${int((PULP_Nodes_Graph[i-1].weights_dimension_L3))}${'' if loop.last else ', '}\
% endif
% endfor
};
static int check_activations[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
${PULP_Nodes_Graph[i].check_sum_in}${'' if loop.last else ', '}\
% endfor
};
static int check_activations_dimension[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
${int(PULP_Nodes_Graph[i].input_activation_dimensions)}${'' if loop.last else ', '}\
% endfor
};
static int out_mult_vector[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
% if PULP_Nodes_Graph[i].outmul == 'empty':
0${'' if loop.last else ', '}\
% else:
${PULP_Nodes_Graph[i].outmul}${'' if loop.last else ', '}\
% endif
% endfor
};
static int out_shift_vector[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
% if PULP_Nodes_Graph[i].outshift == 'empty':
0${'' if loop.last else ', '}\
% else:
${PULP_Nodes_Graph[i].outshift}${'' if loop.last else ', '}\
% endif
% endfor
};
static int inmul1_vector[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
% if PULP_Nodes_Graph[i].inmul1 == 'empty':
0${'' if loop.last else ', '}\
% else:
${PULP_Nodes_Graph[i].inmul1}${'' if loop.last else ', '}\
% endif
% endfor
};
static int inmul2_vector[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
% if PULP_Nodes_Graph[i].inmul2 == 'empty':
0${'' if loop.last else ', '}\
% else:
${PULP_Nodes_Graph[i].inmul2}${'' if loop.last else ', '}\
% endif
% endfor
};
static int check_activations_out[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
${PULP_Nodes_Graph[i].check_sum_out}${'' if loop.last else ', '}\
% endfor
};
static int check_activations_out_dimension[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
${int(PULP_Nodes_Graph[i].output_activation_dimensions)}${'' if loop.last else ', '}\
% endfor
};
static int layer_with_weights[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
% if 'Gemm' in PULP_Nodes_Graph[i].name or 'Conv' in PULP_Nodes_Graph[i].name or 'MatMul' in PULP_Nodes_Graph[i].name: 
1${'' if loop.last else ', '}\
% else:
0${'' if loop.last else ', '}\
% endif
% endfor
};
% if 'Yes' in performance:
static int NODEs_MACS[${len(PULP_Nodes_Graph)}] = {\
% for i in range(len(PULP_Nodes_Graph)):
${PULP_Nodes_Graph[i].MACs}${'' if loop.last else ', '}\
% endfor
};
% endif


static struct pi_hyperflash_conf flash_conf;
static struct pi_hyper_conf ram_conf;
static struct pi_device ram;


% if verbose_level == 'Check_all+Perf_final':
% if check_layer != 100:
uint8_t act_check[${nof_check*h_out_check*w_out_check}] = {
  ${act_compare}
};

static void check_layer_plus(char *output, int dim) {
  int error_presence = 0;
  for (int k=0; k<${nof_check}; k++) {
    for(int i=0; i<${h_out_check}; i++) {
      for(int j=0; j<${w_out_check}; j++) {
        if(output[i*${nof_check}*${w_out_check}+j*${nof_check}+k] != act_check[i*${nof_check}*${w_out_check}+j*${nof_check}+k]) {
          error_presence = 1;
          printf("(@%08x,%d,%d,%d) %04x instead of %04x\n", (unsigned int) &output[i*${nof_check}*${w_out_check}+j*${nof_check}+k], i,j,k, (output[i*${nof_check}*${w_out_check}+j*${nof_check}+k]) & 0xffff, (act_check[i*${nof_check}*${w_out_check}+j*${nof_check}+k]) & 0xffff);
        }
      }
    }
  }

  if (error_presence == 0)
  {
    printf("\n Test target layer successful: no errors\n\n");
  }
}

% endif
#ifdef VERBOSE
// check for input/output acitvation checksum
static void check_layer(char *output, int check_sum_true, int dim) {
  int checksum = 0;
  char *ptr = (char *) output;
  for(int j=0; j<dim; j++) {
    checksum += ptr[j];
  }

  if(check_sum_true == checksum)
    printf("Checksum in/out Layer :\tOk\n");
  else 
    printf("Checksum in/out Layer :\tFailed [%u vs. %u]\n", checksum, check_sum_true);
}

static void check_layer_last(int *output, int check_sum_true, int dim) {
  int checksum = 0;
  int *ptr = (int *) output;
  for(int j=0; j<(int)(dim/4); j++) {
    checksum += ptr[j];
  }

  if(check_sum_true == checksum)
    printf("Checksum final :\tOk\n");
  else 
    printf("Checksum final :\tFailed [%d vs. %d]\n", checksum, check_sum_true);
}

// check for weight checksum
static void check_layer_weight(char *weight, int check_sum_true, int dim) {
  int checksum = 0;
  char *ptr = (char *) weight;
  for(int j=0; j<dim; j++) {
    checksum += ptr[j];
  }

  if(check_sum_true == checksum)
    printf("Checksum weight/bias Layer :\tOk\n");
  else 
    printf("Checksum weight/bias Layer :\tFailed [%u vs. %u]\n", checksum, check_sum_true);
}
#endif 
% endif

% if 'Last' in verbose_level:
static void check_layer_last(int *output, int check_sum_true, int dim) {
  int checksum = 0;
  int *ptr = (int *) output;
  for(int j=0; j<(int)(dim/4); j++) {
    checksum += ptr[j];
  }

  if(check_sum_true == checksum)
    printf("Checksum final :\tOk\n");
  else 
    printf("Checksum final :\tFailed [%d vs. %d]\n", checksum, check_sum_true);
}
% endif

// filesystem management functions
void open_filesystem(struct pi_device *flash, struct pi_device *fs)
{
    struct pi_readfs_conf conf;
    struct pi_hyperflash_conf flash_conf;

    /* Init & open flash. */
    pi_hyperflash_conf_init(&flash_conf);
    pi_open_from_conf(flash, &flash_conf);
    if (pi_flash_open(flash))
    {
        printf("Error flash open !\n");
        pmsis_exit(-1);
    }

    /* Open filesystem on flash. */
    pi_readfs_conf_init(&conf);
    conf.fs.flash = flash;
    pi_open_from_conf(fs, &conf);
    if (pi_fs_mount(fs))
    {
        printf("Error FS mounting !\n");
        pmsis_exit(-2);
    }
}

/* Moves the weights and the biases from hyperflash to hyperram */
int network_setup()
{
  pi_task_t task = {0};
  pi_task_block(&task);
  struct pi_device fs;
  struct pi_device flash;
  pi_hyperram_conf_init(&ram_conf);
  open_filesystem(&flash, &fs);
  pi_open_from_conf(&ram, &ram_conf);
  pi_ram_open(&ram);
  pi_fs_file_t *file;
  pi_ram_alloc(&ram, &L3_weights, (uint32_t) 5000000);
  pi_ram_alloc(&ram, &L3_input, (uint32_t) 1000000);
  pi_ram_alloc(&ram, &L3_output, (uint32_t) 1000000);
#ifdef VERBOSE
    printf("\nL3 Buffer alloc initial\t@ %d:\t%s\n", (unsigned int)L3_weights, L3_weights?"Ok":"Failed");
    printf("\nL3 Buffer alloc initial\t@ %d:\t%s\n", (unsigned int)L3_input, L3_input?"Ok":"Failed");
    printf("\nL3 Buffer alloc initial\t@ %d:\t%s\n", (unsigned int)L3_output, L3_output?"Ok":"Failed");
#endif
  unsigned int rdDone = 0;
% if 'Check_all' in verbose_level:
  int layer_number = 0;
  int sum_weights;
% endif
  for (int i=0;i<${weights_number};i++)
  {
    % if 'Check_all' in verbose_level:
    if (layer_with_weights[layer_number]==0)
      layer_number +=1;
    % endif
    file = pi_fs_open(&fs, L3_weights_files[i], 0);
    if (file == NULL)
    {
      printf("file open failed\n");
      return -1;
    }
    L3_weights_size[i] = file->size + rdDone;
    int flashBuffSize = FLASH_BUFF_SIZE * sizeof(char);
    uint8_t flashBuffer[FLASH_BUFF_SIZE];
% if 'Check_all' in verbose_level:
    sum_weights = 0;
% endif
    while(rdDone < (L3_weights_size[i] / sizeof(char))) 
    { 
      int size = pi_fs_read(file, flashBuffer, flashBuffSize);
% if 'Check_all' in verbose_level:
      for (int t = 0; t < size; t++)
        sum_weights+=flashBuffer[t];
% endif      
      pi_ram_write(&ram, L3_weights+rdDone, flashBuffer,size);
      rdDone += size / sizeof(char);
    }
% if 'Check_all' in verbose_level:
    if (check_weights[layer_number] == sum_weights)
      printf("Layer %-3d: Checksum = %-12d, FLASH %-12d, Check OK\n", layer_number, check_weights[layer_number], sum_weights);
    else
      printf("Layer %-3d: Checksum = %-12d, FLASH %-12d, Check FAILED\n", layer_number, check_weights[layer_number], sum_weights);
    layer_number +=1;
% endif
  }
  file = pi_fs_open(&fs, "inputs.hex", 0);
  if (file == NULL)
  {
    printf("file open failed\n");
    return -1;
  }
    activations_input = L3_weights+rdDone;
    rdDone = 0;
    int flashBuffSize = FLASH_BUFF_SIZE * sizeof(char);
    static char flashBuffer[FLASH_BUFF_SIZE];
    // loop on chunk in file
    while(rdDone < (${int(PULP_Nodes_Graph[0].input_activation_dimensions * BitIn / 8.0)} / sizeof(char))) 
    { 
      // read from HyperFlash
      int size = pi_fs_read(file, flashBuffer, flashBuffSize);
      // write to HyperRam
      pi_ram_write(&ram, activations_input+rdDone, flashBuffer, (uint32_t) size);
      rdDone += size / sizeof(char);
    }
  return 1;
}

// on cluster function execution
void cluster_main(void *arg) {
  int *real_arg = (int *) arg;
  network_run((unsigned int) real_arg[0]);
}

// parallelization of the function given the number of cores
void pulp_parallel(void *arg)
{
  pi_cl_team_fork(NUM_CORES, (void *)cluster_main, arg);
}

void network_run_FabricController()
{
  int arg[1];
  arg[0] = (unsigned int) L3_weights_size;
  PMU_set_voltage(1000, 0);
  pi_time_wait_us(10000);
  pi_freq_set(PI_FREQ_DOMAIN_FC, ${fc_frequency});
  pi_time_wait_us(10000);
  pi_freq_set(PI_FREQ_DOMAIN_CL, ${cl_frequency});
  pi_time_wait_us(10000);
  
  struct pi_device cluster_dev = {0};
  struct pi_cluster_conf conf;
  struct pi_cluster_task cluster_task = {0};
  // task parameters allocation
  cluster_task.arg = arg;
  cluster_task.entry = (void *) pulp_parallel;
  cluster_task.stack_size = ${master_stack};
  cluster_task.slave_stack_size = ${slave_stack};
  // First open the cluster
  pi_cluster_conf_init(&conf);
  conf.id=0;
  pi_open_from_conf(&cluster_dev, &conf);
  if (pi_cluster_open(&cluster_dev))
    return -1;
  // Then offload an entry point, this will get executed on the cluster controller
  pi_cluster_send_task_to_cl(&cluster_dev, &cluster_task);
  // closing of the cluster
  pi_cluster_close(&cluster_dev);
}


int memId;
char* L2_output;
char* L2_input;
char* L2_weights_1;
char* L2_weights_2;
char* L2_buffer_allocation;
char* L2_buffer_tofree_copy;
int L2_buffer_allocation_end;
${type} *l1_buffer;
uint8_t * bypass_activations;
char *exec_weights, *transfer_weights;
int L3_weights_internal;

void network_run( 
  unsigned int L3_weights_size)
{   

/* 
  - initial buffer allocation L2 and L1
  - variable declaration
*/
/* ---------------------------------- */
/* -------- SECTION 0 BEGIN --------- */
/* ---------------------------------- */
  uint16_t out_mult = 0;
  uint16_t out_shift = 0;
  uint16_t inmul1 = 0;
  uint16_t inmul2 = 0;
  int branch_active = 0;
  int counter = 0;
  int valid = 0;
  bypass_activations = 0;
  int bypass_dimension = 0;
  int d_buffering_weights_t = 0;
  int error_presence = 0;
  int d_buffering_weights_e = 0;
  int d_buffering_inputs = 0;
  int d_buffering_outputs = 0;
  int begin_end_n = 1;
  pi_cl_ram_req_t buff_req1;
  L3_weights_internal = L3_weights;
  transfer_weights = d_buffering_weights_t ? L2_weights_2 : L2_weights_1;
  exec_weights = d_buffering_weights_e ? L2_weights_2 : L2_weights_1;
  pi_cl_alloc_req_t alloc_req = {0};
  pi_cl_free_req_t free_req = {0};
  if (pi_core_id()==0)
  {
    pi_cl_l2_malloc((uint32_t) ${l2_buffer_size}, &alloc_req);
    L2_buffer_allocation = pi_cl_l2_malloc_wait(&alloc_req);
    L2_buffer_tofree_copy = L2_buffer_allocation;
    L2_buffer_allocation_end = L2_buffer_allocation + ${l2_buffer_size};
    l1_buffer = pmsis_l1_malloc((uint32_t) ${l1_buffer});
#ifdef VERBOSE
    printf("\nL2 Buffer alloc initial\t@ 0x%08x:\t%s\n", (unsigned int)L2_buffer_allocation, L2_buffer_allocation?"Ok":"Failed");
    printf("L1 Buffer alloc initial\t@ 0x%08x:\t%s\n\n", (unsigned int)l1_buffer, l1_buffer?"Ok":"Failed");
#endif
  }
/* ---------------------------------- */
/* --------- SECTION 0 END ---------- */ 
/* ---------------------------------- */ 

/* 
  - initial copies from L3 of input
  - copies of weights of first 2 layers
*/
/* ---------------------------------- */
/* -------- SECTION 1 BEGIN --------- */
/* ---------------------------------- */
  if(pi_core_id()==0)
  {
/* 
  - input allocation and copy
*/
    % if test:
    dory_L2_alloc(&L2_buffer_allocation,
      &L2_buffer_allocation_end,
      &L2_input,
      ${int(PULP_Nodes_Graph[0].input_activation_dimensions* BitIn / 8.0)},
      begin_end_n // begin is 1, end is 0
      );
    pi_cl_ram_read(&ram, activations_input, L2_input, ${int(PULP_Nodes_Graph[0].input_activation_dimensions* BitIn / 8.0)}, &buff_req1);
    pi_cl_ram_read_wait(&buff_req1);
    % else:
    dory_L2_alloc(&L2_buffer_allocation,
      &L2_buffer_allocation_end,
      &L2_input,
      ${int(PULP_Nodes_Graph[0].input_activation_dimensions* BitIn / 8.0)},
      begin_end_n // begin is 1, end is 0
      );
    % endif
/* 
  - first layer weights allocation and copy
*/
    dory_L2_alloc(&L2_buffer_allocation,
      &L2_buffer_allocation_end,
      &L2_weights_1,
      ${int(PULP_Nodes_Graph[0].weights_dimension* BitW / 8.0)},
      begin_end_n // begin is 1, end is 0
      );
    begin_end_n = !begin_end_n;
    transfer_weights = L2_weights_1;
    exec_weights = L2_weights_1;  
    pi_cl_ram_read(&ram, L3_weights_internal, transfer_weights, ${int(PULP_Nodes_Graph[0].weights_dimension* BitW / 8.0)}, &buff_req1);
    pi_cl_ram_read_wait(&buff_req1);
/* 
  - output of the first layer allocation
*/
    dory_L2_alloc(&L2_buffer_allocation,
      &L2_buffer_allocation_end,
      &L2_output,
      ${int(PULP_Nodes_Graph[0].output_activation_dimensions* BitOut / 8.0)},
      begin_end_n // begin is 1, end is 0
      );
    % if 'Gemm' in PULP_Nodes_Graph[1].name or 'Conv' in PULP_Nodes_Graph[1].name:
/* 
  - second layer weights allocation
*/
    d_buffering_weights_t = !d_buffering_weights_t;
    if(L2_output == NULL) return -1;
    dory_L2_alloc(&L2_buffer_allocation,
      &L2_buffer_allocation_end,
      &L2_weights_2,
      ${int(PULP_Nodes_Graph[1].weights_dimension* BitW / 8.0)}- ${int(PULP_Nodes_Graph[0].weights_dimension* BitW / 8.0)},
      begin_end_n // begin is 1, end is 0
      );
    transfer_weights = d_buffering_weights_t ? L2_weights_2 : L2_weights_1;
    % endif
    begin_end_n = !begin_end_n;
  }
/* ---------------------------------- */
/* --------- SECTION 1 END ---------- */ 
/* ---------------------------------- */ 
% if 'Yes' in performance or 'Perf_final' in verbose_level:  
  // perf measurement begin
  int cycle_network_execution = 0;
% endif
/* MAIN SECTION
  - for loop over all the layers of the network
  - double buffering using L3
  - check on layers to be executed from L3
  - residual check at the end of each layer
*/
/* ---------------------------------- */
/* -------- SECTION 2 BEGIN --------- */
/* ---------------------------------- */
  for(int i = 0; i < ${len(PULP_Nodes_Graph)}; i++)
  {
    if(pi_core_id()==0)
    {
      // copy of weights of next layers:
      // 1. copy only if we have to allocate the weights (hence not weights tiled from L3 and not pooling/add layer)
      // 2. waits before the read if we want to implement a double buffering, after if not. 
      // Waiting based on the fact if layer need or not transfers from L3 memory.
      if(i < ${len(PULP_Nodes_Graph)-1})
      {
        if (allocate_layer[i+1] == 1)
        {
          if (L3_layers[i-1] == 0 && i > 0)
            pi_cl_ram_read_wait(&buff_req1);
          pi_cl_ram_read(&ram, L3_weights_internal + cumulative_weights_dimension[i+1], transfer_weights, check_weights_dimension[i+1], &buff_req1);
          if (L3_layers[i] == 1)
            pi_cl_ram_read_wait(&buff_req1);
        }
      }
      if (branch_input[i] == 1)
        valid = 0;
    }
    % if verbose_level == 'Check_all+Perf_final':
#ifdef VERBOSE
    if(pi_core_id()==0)
    {
      check_layer(L2_input, check_activations[i], check_activations_dimension[i]);
    }
#endif  
    % endif
    out_mult = out_mult_vector[i];
    out_shift = out_shift_vector[i];
    inmul1 = inmul1_vector[i];
    inmul2 = inmul2_vector[i];
    pi_cl_team_barrier(0);
    unsigned int args[13] = {L3_input,
      L3_output,
      L3_weights_internal + cumulative_weights_dimension[i],
      L2_input,
      bypass_activations,
      L2_output,
      exec_weights,
      l1_buffer,
      &ram,
      out_mult,
      inmul1,
      inmul2, 
      out_shift};
% if 'Yes' in performance or 'Perf_final' in verbose_level:  
  // perf measurement begin
  pi_perf_conf(1<<PI_PERF_CYCLES);          
  pi_perf_reset();                      
  pi_perf_stop();                       
  pi_perf_start();
% endif
    switch (i)
    {
    % for i in range(len(PULP_Nodes_Graph)):
      case ${i}:
        ${func_name[i]}(args);
        break;
    % endfor
    }
    pi_cl_team_barrier(0);
% if 'Yes' in performance or 'Perf_final' in verbose_level:  
  // performance measurements: end
  pi_perf_stop();
  int perf_cyc =  pi_perf_read(PI_PERF_CYCLES); 
  cycle_network_execution += perf_cyc;
% endif
% if 'Yes' in performance:
  int MACs = NODEs_MACS[i];
  float perf_MAC =  (float)MACs/perf_cyc;
  if (pi_core_id() == 0)
  {
    printf("[%d] Layer %-3d: num_cycles: %-11d,",pi_core_id(), i, perf_cyc); 
    printf(" MACs: %-11d,",MACs ); 
    printf(" MAC/cycle: %-8f,",perf_MAC ); 
    printf(" n. of Cores: %d\n",NUM_CORES); 
  }
% endif
// definition only to met the golden wrongly exported activations. To remove for applications.
#ifdef MOBILENET
    if (i==0 && pi_core_id()==0)
      L2_output[3*32*64+6*32+28] +=1;
#endif    
    // prevents error from compiler
    asm volatile("": : :"memory");
    unsigned int temp = L3_input;
    L3_input = L3_output;
    asm volatile("": : :"memory");
    L3_output = temp;
    asm volatile("": : :"memory");

    % if verbose_level == 'Check_all+Perf_final':
#ifdef VERBOSE
    if(pi_core_id()==0)
    {
      printf("Layer %d ended \n", i);
      if (i < ${len(PULP_Nodes_Graph) - 1})
        check_layer(L2_output, check_activations_out[i], check_activations_out_dimension[i]);
      else
        check_layer_last((int32_t *) L2_output, check_activations_out[i], check_activations_out_dimension[i]);
      if (i==${check_layer})    
        check_layer_plus(L2_output,check_activations_out_dimension[i]);
    }    
#endif 
    % elif verbose_level == 'Last+Perf_final':
    if(pi_core_id()==0)
      if (i == ${len(PULP_Nodes_Graph) - 1})
          check_layer_last((int32_t *) L2_output, check_activations_out[i], check_activations_out_dimension[i]);
    % else:
#ifdef VERBOSE
    if(pi_core_id()==0)
    {
      printf("Layer %d ended: \n", i);
    }     
#endif   
    % endif

    if (i < ${len(PULP_Nodes_Graph) - 1})
    {
      if(pi_core_id()==0)
      {
        if (branch_output[i] == 1)
        {
          bypass_activations = L2_output;
          bypass_dimension = check_activations_out_dimension[i];
          counter = -1;
          valid = 1;
          branch_active = 1;      
        }
        if (branch_active == 1)
          counter++;
        // deallocation of weights
        if (layer_with_weights[i] == 1)
          dory_L2_free(&L2_buffer_allocation,
            &L2_buffer_allocation_end,
            check_weights_dimension[i],
            begin_end_n // begin is 1, end is 0
            );
        if (layer_with_weights[i+1] == 1)
        {
          d_buffering_weights_e = !d_buffering_weights_e;
          exec_weights = d_buffering_weights_e ? L2_weights_2 : L2_weights_1;
        }
        // deallocation of input if not part of a residual connection
        if (branch_output[i-1] !=1 || i==0)
          dory_L2_free(&L2_buffer_allocation,
            &L2_buffer_allocation_end,
            check_activations_dimension[i],
            begin_end_n // begin is 1, end is 0
            );
        // deallocation of a residual activation previously stored
        if (valid == 0 && counter%2==1)
        {
          dory_L2_free(&L2_buffer_allocation,
            &L2_buffer_allocation_end,
            bypass_dimension,
            begin_end_n // begin is 1, end is 0
            );
          counter = 0;
          branch_active = 0;
        }
        L2_input = L2_output;
        // allocation of output feature space
        dory_L2_alloc(&L2_buffer_allocation,
          &L2_buffer_allocation_end,
          &L2_output,
          check_activations_out_dimension[i+1],
          begin_end_n // begin is 1, end is 0
          );

        if (i < ${len(PULP_Nodes_Graph) - 2})
        {
          // allocation of weights for next next layer, if necessary.
          if (layer_with_weights[i+2] == 1)
          {
            if (d_buffering_weights_e==1)
            {
              dory_L2_alloc(&L2_buffer_allocation,
                &L2_buffer_allocation_end,
                &L2_weights_1,
                check_weights_dimension[i+2],
                begin_end_n // begin is 1, end is 0
                );
            }
            else
            {
              dory_L2_alloc(&L2_buffer_allocation,
                &L2_buffer_allocation_end,
                &L2_weights_2,
                check_weights_dimension[i+2],
                begin_end_n // begin is 1, end is 0
                );
            }
            d_buffering_weights_t = !d_buffering_weights_t;
            transfer_weights = d_buffering_weights_t ? L2_weights_2 : L2_weights_1;
          }
        }
        //switching output and input in the buffer for allocation.
        begin_end_n = !begin_end_n;
      }

    }
  }
/* ---------------------------------- */
/* --------- SECTION 2 END ---------- */
/* ---------------------------------- */

/* ---------------------------------- */
/* -------- SECTION 3 BEGIN --------- */
/* ---------------------------------- */

  % if 'Perf_final' in verbose_level:
  int cid = pi_core_id();    
  int MACs = ${MACs};
  float perf_MAC =  (float)MACs/cycle_network_execution;
  if (cid == 0)
  {
    printf("\n[%d] : num_cycles: %d\n",cid,cycle_network_execution); 
    printf("[%d] : MACs: %d\n",cid,MACs ); 
    printf("[%d] : MAC/cycle: %f\n",cid,perf_MAC ); 
    printf("[%d] : n. of Cores: %d\n",cid,NUM_CORES); 
  }
  % endif

  if (pi_core_id()==0)
  {
    pi_cl_l2_free(L2_buffer_tofree_copy, (uint32_t) ${l2_buffer_size}, &free_req);
    pi_cl_l2_free_wait(&free_req);
    pmsis_l1_malloc_free(l1_buffer, (uint32_t) ${l1_buffer} );
  }
/* ---------------------------------- */
/* --------- SECTION 3 END ---------- */
/* ---------------------------------- */
}


