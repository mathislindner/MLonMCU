#include "pmsis.h"

#include "pulp_nn_utils.h"

#include "pulp_nn_functions.h"

#define log2(x) __builtin_pulp_fl1(x)
#define min(a,b) ((a)<(b)?(a):(b))
#define SumDotp(a, b, c) __builtin_pulp_sdotusp4(a, b, c)

void pulp_nn_linear_u4_i4_i2(
		 	uint8_t *pInBuffer,
			int8_t *pWeights,
			uint16_t dim_vec,
			uint16_t num_o_neurons,
			int8_t *bias,
			uint16_t bias_shift,
			int8_t out_shift,
          	uint16_t out_mult,
			int32_t *k,
			int32_t *lambda,
			int8_t *pOutBuffer,
			int flag_relu,
          	int flag_batch_norm,
          	unsigned int * memory_chan
			)
{
	uint16_t dim_vec_in = dim_vec >> 1;
	uint16_t dim_vec_wt = dim_vec >> 2;

  int8_t mask = 0xf0;
  int8_t n_mask = ~ mask;
  int8_t off = 0x04;

	int core_id = pi_core_id();
	int Log2Core = log2(NUM_CORES);
	int chunk = (num_o_neurons >> Log2Core) + ((num_o_neurons & (NUM_CORES-1))!=0);
	volatile int start = min(chunk * core_id, num_o_neurons);
	volatile int stop = min(start + chunk, num_o_neurons);

	v4u vecA[4];
	v4s vecB[4];
	v4s vecB2[4];

	volatile int32_t *pOut = (int32_t *) pOutBuffer + start;

	int stop_even = stop - (stop & 0x01);

	for(int i=start; i<stop_even; i+=2)
	{
		int sum = 0;
		int sum2 = 0;

		uint8_t *pA = pInBuffer;
		int8_t *pB = pWeights + (i * dim_vec_wt);
		int8_t *pB2 = pB + dim_vec_wt;

		for (int j=0; j<(dim_vec >> 4); j++)
		{
	      pulp_nn_u4_to_u8(pA,vecA);
	      pA+=4;
	      pulp_nn_u4_to_u8(pA,vecA + 2);
		  pulp_nn_i2_to_i8(pB,vecB);
	      pulp_nn_i2_to_i8(pB2,vecB2);	      	      
		  sum = SumDotp(vecA[0], vecB[0], sum);
	      sum = SumDotp(vecA[1], vecB[1], sum);
	      sum = SumDotp(vecA[2], vecB[2], sum);
	      sum = SumDotp(vecA[3], vecB[3], sum);
	      sum2 = SumDotp(vecA[0], vecB2[0], sum2);
	      sum2 = SumDotp(vecA[1], vecB2[1], sum2);
	      sum2 = SumDotp(vecA[2], vecB2[2], sum2);
	      sum2 = SumDotp(vecA[3], vecB2[3], sum2);
	      pA+=4;
	      pB+=4;
	      pB2+=4;			
		}
    uint16_t col_cnt = dim_vec & 0xf;
    while (col_cnt)
    {
      uint8_t inA = (uint8_t) bitext((unsigned int) *pA, 4, 0);
      uint8_t inA2 = (uint8_t) bitext((unsigned int) *pA, 4, 4);
      pA++;
      uint8_t inA3 = (uint8_t) bitext((unsigned int) *pA, 4, 0);
      uint8_t inA4 = (uint8_t) bitext((unsigned int) *pA, 4, 4);
      pA++;
      int8_t inB = (int8_t) bitext((int) *pB, 2, 0);
      int8_t inB2 = (int8_t) bitext((int) *pB, 2, 2);
      int8_t inB3 = (int8_t) bitext((int) *pB, 2, 4);
      int8_t inB4 = (int8_t) bitext((int) *pB, 2, 6);
      pB++;
      int8_t inB5 = (int8_t) bitext((int) *pB2, 2, 0);
      int8_t inB6 = (int8_t) bitext((int) *pB2, 2, 2);
      int8_t inB7 = (int8_t) bitext((int) *pB2, 2, 4);
      int8_t inB8 = (int8_t) bitext((int) *pB2, 2, 6);
      pB2++;
 	  sum += inA * inB;
 	  sum += inA2 * inB2;
 	  sum += inA3 * inB3;
 	  sum += inA4 * inB4;
 	  sum2 += inA * inB5;
 	  sum2 += inA2 * inB6;
 	  sum2 += inA3 * inB7;
 	  sum2 += inA4 * inB8;
      col_cnt--;
    }
    if (flag_batch_norm && flag_relu)
    {
      sum = pulp_nn_bn_quant_u4(sum, *k, *lambda, out_shift);
      k++;
      lambda++;
      sum2 = pulp_nn_bn_quant_u4(sum2, *k, *lambda, out_shift);
      k++;
      lambda++;
      *pOut = bitins(sum, n_mask, sum2, mask, off);
      pOut++;
    }
    else
    {
      if (flag_relu == 1)
      {
        sum = pulp_nn_quant_u4(sum, out_mult, out_shift);
        sum2 = pulp_nn_quant_u4(sum2, out_mult, out_shift);
        *pOut = bitins(sum, n_mask, sum2, mask, off);
        pOut++;
      }
      else
      {
        sum = (uint8_t) clip8(sum >> out_shift);
        sum2 = (uint8_t) clip8(sum2 >> out_shift);
        *pOut = bitins(sum, n_mask, sum2, mask, off);
        pOut++;
      }
    }
  }
	if (stop & 0x01)
	{
		int sum = 0;

		uint8_t *pA = pInBuffer;
		int8_t *pB = pWeights + dim_vec_wt;

		for (int j=0; j<(dim_vec >> 4); j++)
		{
	      pulp_nn_u4_to_u8(pA,vecA);
	      pA+=4;
	      pulp_nn_u4_to_u8(pA,vecA + 2);
		  pulp_nn_i2_to_i8(pB,vecB);	      	      
		  sum = SumDotp(vecA[0], vecB[0], sum);
	      sum = SumDotp(vecA[1], vecB[1], sum);
	      sum = SumDotp(vecA[2], vecB[2], sum);
	      sum = SumDotp(vecA[3], vecB[3], sum);
	      pA+=4;
	      pB+=4;		
		}
    uint16_t col_cnt = dim_vec & 0xf;
    while (col_cnt)
    {
      uint8_t inA = (uint8_t) bitext((unsigned int) *pA, 4, 0);
      uint8_t inA2 = (uint8_t) bitext((unsigned int) *pA, 4, 4);
      pA++;
      uint8_t inA3 = (uint8_t) bitext((unsigned int) *pA, 4, 0);
      uint8_t inA4 = (uint8_t) bitext((unsigned int) *pA, 4, 4);
      pA++;
      int8_t inB = (int8_t) bitext((int) *pB, 2, 0);
      int8_t inB2 = (int8_t) bitext((int) *pB, 2, 2);
      int8_t inB3 = (int8_t) bitext((int) *pB, 2, 4);
      int8_t inB4 = (int8_t) bitext((int) *pB, 2, 6);
      pB++;
 	  sum += inA * inB;
 	  sum += inA2 * inB2;
 	  sum += inA3 * inB3;
 	  sum += inA4 * inB4;
      col_cnt--;
    }
		*pOut = sum;
		pOut++;	
	}
	pi_cl_team_barrier(0);
}