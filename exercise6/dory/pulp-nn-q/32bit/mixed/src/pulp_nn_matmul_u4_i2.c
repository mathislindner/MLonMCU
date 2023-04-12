#include "pmsis.h"

#include "pulp_nn_utils.h"

#define SumDotp(a, b, c) __builtin_pulp_sdotusp4(a, b, c)
#define nn_round(out_shift) (0x1 << (out_shift -1))
#define bitins(dst,not_mask_imm,src,mask_imm,off) __builtin_pulp_binsert(dst,not_mask_imm,src,mask_imm,off)
#define bitext(x,size,off) __builtin_pulp_bextract(x,size,off)
#define clip8(x) __builtin_pulp_clipu_r(x, 15)

uint8_t *pulp_nn_matmul_u4_i2(
          const int8_t * pWeight,
          uint8_t * pInBuffer,
          uint16_t ch_out,
          uint16_t num_col_im2col,
					uint16_t bias_shift,
          int8_t out_shift,
          uint16_t out_mult,
          int32_t *k,
          int32_t *lambda,
					const int8_t * bias,
          uint8_t * pOut,
          int flag_relu,
          int flag_batch_norm
) {
  int8_t mask = 0xf0;
  int8_t n_mask = ~ mask;
  int8_t off = 0x04;
  v4s vecA[4];
  v4s vecA2[4];
  v4s vecA3[4];
  v4s vecA4[4];
  v4u vecB3;
  v4u vecB4;
  v4u vecB5;
  v4u vecB6;
  v4u vecB7;
  v4u vecB8;
  v4u vecB;
  v4u vecB2;

  uint16_t ch_out_r = ch_out >> 1;
  uint16_t num_col_im2col_w = num_col_im2col >> 2;

  uint8_t *pOut2 = pOut + ch_out_r;
  int8_t *pA = pWeight;

  uint16_t chan_left = ch_out & 0x3;

  for(int i=0; i < (ch_out >> 2); i++)
  {
    uint8_t *pB =  pInBuffer;
    uint8_t *pB2 = (pB + num_col_im2col);
    int8_t *pA2 = (pA + num_col_im2col_w);
    int8_t *pA3 = (pA2 + num_col_im2col_w);
    int8_t *pA4 = (pA3 + num_col_im2col_w);

    // int bias1 = (bias[i] << bias_shift);
    // int bias2 = (bias[i + 1] << bias_shift);
    // int bias3 = (bias[i + 2] << bias_shift);
    // int bias4 = (bias[i + 3] << bias_shift);

    int sum =  0;//bias1;
    int sum2 = 0;//bias2;
    int sum3 = 0;//bias3;
    int sum4 = 0;//bias4;

    int sum5 = 0;//bias1;
    int sum6 = 0;//bias2;
    int sum7 = 0;//bias3;
    int sum8 = 0;//bias4;

    for(int j=0; j<(num_col_im2col_w >> 2); j++)
    {
      vecB = *((v4u*)pB);
      vecB2 = *((v4u*)pB2);
      vecB3 = *((v4u*)(pB + 4));
      vecB4 = *((v4u*)(pB2 + 4));
      vecB5 = *((v4u*)(pB + 8));
      vecB6 = *((v4u*)(pB2 + 8));
      vecB7 = *((v4u*)(pB + 12));
      vecB8 = *((v4u*)(pB2 + 12));

      pB+=16;
      pB2+=16;

      pulp_nn_i2_to_i8(pA,vecA);

      sum = SumDotp(vecB, vecA[0], sum);
      sum5 = SumDotp(vecB2, vecA[0], sum5);
      sum = SumDotp(vecB3, vecA[1], sum);
      sum5 = SumDotp(vecB4, vecA[1], sum5);
      sum = SumDotp(vecB5, vecA[2], sum);
      sum5 = SumDotp(vecB6, vecA[2], sum5);
      sum = SumDotp(vecB7, vecA[3], sum);
      sum5 = SumDotp(vecB8, vecA[3], sum5);

      pulp_nn_i2_to_i8(pA2,vecA2);

      sum2 = SumDotp(vecB, vecA2[0], sum2);
      sum6 = SumDotp(vecB2, vecA2[0], sum6);
      sum2 = SumDotp(vecB3, vecA2[1], sum2);
      sum6 = SumDotp(vecB4, vecA2[1], sum6);
      sum2 = SumDotp(vecB5, vecA2[2], sum2);
      sum6 = SumDotp(vecB6, vecA2[2], sum6);
      sum2 = SumDotp(vecB7, vecA2[3], sum2);
      sum6 = SumDotp(vecB8, vecA2[3], sum6);

      pulp_nn_i2_to_i8(pA3,vecA3);

      sum3 = SumDotp(vecB, vecA3[0], sum3);
      sum7 = SumDotp(vecB2, vecA3[0], sum7);
      sum3 = SumDotp(vecB3, vecA3[1], sum3);
      sum7 = SumDotp(vecB4, vecA3[1], sum7);
      sum3 = SumDotp(vecB5, vecA3[2], sum3);
      sum7 = SumDotp(vecB6, vecA3[2], sum7);
      sum3 = SumDotp(vecB7, vecA3[3], sum3);
      sum7 = SumDotp(vecB8, vecA3[3], sum7);

      pulp_nn_i2_to_i8(pA4,vecA4);

      sum4 = SumDotp(vecB, vecA4[0], sum4);
      sum8 = SumDotp(vecB2, vecA4[0], sum8);
      sum4 = SumDotp(vecB3, vecA4[1], sum4);
      sum8 = SumDotp(vecB4, vecA4[1], sum8);
      sum4 = SumDotp(vecB5, vecA4[2], sum4);
      sum8 = SumDotp(vecB6, vecA4[2], sum8);
      sum4 = SumDotp(vecB7, vecA4[3], sum4);
      sum8 = SumDotp(vecB8, vecA4[3], sum8);

      pA+=4;
      pA2+=4;
      pA3+=4;
      pA4+=4;
    }
    uint16_t col_cnt_im2col = num_col_im2col & 0xf;
    while (col_cnt_im2col)
    {
      int8_t inA = (int8_t) bitext((int) *pA, 2, 0);
      int8_t inA2 = (int8_t) bitext((int) *pA2, 2, 0);
      int8_t inA3 = (int8_t) bitext((int) *pA3, 2, 0);
      int8_t inA4 = (int8_t) bitext((int) *pA4, 2, 0);     
      uint8_t inB = *pB++;
      uint8_t inB2 = *pB2++;
      sum += inA * inB;
      sum2 += inA2 * inB;
      sum3 += inA3 * inB;
      sum4 += inA4 * inB;
      sum5 += inA * inB2;
      sum6 += inA2 * inB2;
      sum7 += inA3 * inB2;
      sum8 += inA4 * inB2;
      inA = (int8_t) bitext((int) *pA, 2, 2);
      inA2 = (int8_t) bitext((int) *pA2, 2, 2);
      inA3 = (int8_t) bitext((int) *pA3, 2, 2);
      inA4 = (int8_t) bitext((int) *pA4, 2, 2); 
      inB = *pB++;
      inB2 = *pB2++;
      sum += inA * inB;
      sum2 += inA2 * inB;
      sum3 += inA3 * inB;
      sum4 += inA4 * inB;
      sum5 += inA * inB2;
      sum6 += inA2 * inB2;
      sum7 += inA3 * inB2;
      sum8 += inA4 * inB2;
      inA = (int8_t) bitext((int) *pA, 2, 4);
      inA2 = (int8_t) bitext((int) *pA2, 2, 4);
      inA3 = (int8_t) bitext((int) *pA3, 2, 4);
      inA4 = (int8_t) bitext((int) *pA4, 2, 4); 
      inB = *pB++;
      inB2 = *pB2++;
      sum += inA * inB;
      sum2 += inA2 * inB;
      sum3 += inA3 * inB;
      sum4 += inA4 * inB;
      sum5 += inA * inB2;
      sum6 += inA2 * inB2;
      sum7 += inA3 * inB2;
      sum8 += inA4 * inB2;
      inA = (int8_t) bitext((int) *pA, 2, 6);
      inA2 = (int8_t) bitext((int) *pA2, 2, 6);
      inA3 = (int8_t) bitext((int) *pA3, 2, 6);
      inA4 = (int8_t) bitext((int) *pA4, 2, 6); 
      inB = *pB++;
      inB2 = *pB2++;
      sum += inA * inB;
      sum2 += inA2 * inB;
      sum3 += inA3 * inB;
      sum4 += inA4 * inB;
      sum5 += inA * inB2;
      sum6 += inA2 * inB2;
      sum7 += inA3 * inB2;
      sum8 += inA4 * inB2;

      pA++;
      pA2++;
      pA3++;
      pA4++;
      col_cnt_im2col-=4;
    }
    if (flag_batch_norm && flag_relu)
    {
      sum = pulp_nn_bn_quant_u4(sum, *k, *lambda, out_shift);
      sum5 = pulp_nn_bn_quant_u4(sum5, *k, *lambda, out_shift);
      k++;
      lambda++;
      sum2 = pulp_nn_bn_quant_u4(sum2, *k, *lambda, out_shift);
      sum6 = pulp_nn_bn_quant_u4(sum6, *k, *lambda, out_shift);
      *pOut = bitins(sum, n_mask, sum2, mask, off);
      *pOut2 = bitins(sum5, n_mask, sum6, mask, off);
      k++;
      lambda++;
      pOut++;
      pOut2++;
      sum3 = pulp_nn_bn_quant_u4(sum3, *k, *lambda, out_shift);
      sum7 = pulp_nn_bn_quant_u4(sum7, *k, *lambda, out_shift);
      k++;
      lambda++;
      sum4 = pulp_nn_bn_quant_u4(sum4, *k, *lambda, out_shift);
      sum8 = pulp_nn_bn_quant_u4(sum8, *k, *lambda, out_shift);
      k++;
      lambda++;
      *pOut = bitins(sum3, n_mask, sum4, mask, off);
      *pOut2 = bitins(sum7, n_mask, sum8, mask, off);
      pOut++;
      pOut2++;
    }
    else
    {
      if (flag_relu == 1)
      {
        sum = pulp_nn_quant_u4(sum, out_mult, out_shift);
        sum2 = pulp_nn_quant_u4(sum2, out_mult, out_shift);
        *pOut = bitins(sum, n_mask, sum2, mask, off);
        pOut++;
        sum3 = pulp_nn_quant_u4(sum3, out_mult, out_shift);
        sum4 = pulp_nn_quant_u4(sum4, out_mult, out_shift);
        *pOut = bitins(sum3, n_mask, sum4, mask, off);
        pOut++;

        sum5 = pulp_nn_quant_u4(sum5, out_mult, out_shift);
        sum6 = pulp_nn_quant_u4(sum6, out_mult, out_shift);
        *pOut2 = bitins(sum5, n_mask, sum6, mask, off);
        pOut2++;
        sum7 = pulp_nn_quant_u4(sum7, out_mult, out_shift);
        sum8 = pulp_nn_quant_u4(sum8, out_mult, out_shift);
        *pOut2 = bitins(sum7, n_mask, sum8, mask, off);
        pOut2++;
      }
      else
      {
        sum = (uint8_t) clip8(sum >> out_shift);
        sum2 = (uint8_t) clip8(sum2 >> out_shift);
        *pOut = bitins(sum, n_mask, sum2, mask, off);
        pOut++;
        sum3 = (uint8_t) clip8(sum3 >> out_shift);
        sum4 = (uint8_t) clip8(sum4 >> out_shift);
        *pOut = bitins(sum3, n_mask, sum4, mask, off);
        pOut++;

        sum5 = (uint8_t) clip8(sum5 >> out_shift);
        sum6 = (uint8_t) clip8(sum6 >> out_shift);
        *pOut2 = bitins(sum5, n_mask, sum6, mask, off);
        pOut2++;
        sum7 = (uint8_t) clip8(sum7 >> out_shift);
        sum8 = (uint8_t) clip8(sum8 >> out_shift);
        *pOut2 = bitins(sum7, n_mask, sum8, mask, off);
        pOut2++;
      }
    }
    pA+=(3 * num_col_im2col_w);
  }
   uint16_t i = 0;
   while(chan_left)
  {
    uint8_t *pB = pInBuffer ;
    uint8_t *pB2 = (pB + num_col_im2col);
    int sum = 0;//((int) (*bias++) << bias_shift);// + nn_round(out_shift);
    int sum2 = sum;
    uint8_t out[2];
    uint8_t out2[2];
    for(int j=0; j < (num_col_im2col_w >> 2); j++)
    {
      vecB = *((v4u*)pB);
      vecB2 = *((v4u*)pB2);
      vecB3 = *((v4u*)(pB + 4));
      vecB4 = *((v4u*)(pB2 + 4));
      vecB5 = *((v4u*)(pB + 8));
      vecB6 = *((v4u*)(pB2 + 8));
      vecB7 = *((v4u*)(pB + 12));
      vecB8 = *((v4u*)(pB2 + 12));

      pulp_nn_i2_to_i8(pA,vecA);

      sum = SumDotp(vecB, vecA[0], sum);
      sum2 = SumDotp(vecB2, vecA[0], sum2);
      sum = SumDotp(vecB3, vecA[1], sum);
      sum2 = SumDotp(vecB4, vecA[1], sum2);
      sum = SumDotp(vecB5, vecA[2], sum);
      sum2 = SumDotp(vecB6, vecA[2], sum2);
      sum = SumDotp(vecB7, vecA[3], sum);
      sum2 = SumDotp(vecB8, vecA[3], sum2);

      pA+=4;
      pB+=16;
      pB2+=16;
    }
    uint16_t col_cnt_im2col = num_col_im2col & 0xf;
    while(col_cnt_im2col)
    {
      int8_t inA = (int8_t) bitext((int) *pA, 2, 0);   
      uint8_t inB = *pB++;
      uint8_t inB2 = *pB2++;
      sum += inA * inB;
      sum2 += inA * inB2;
      inA = (int8_t) bitext((int) *pA, 2, 2);
      inB = *pB++;
      inB2 = *pB2++;
      sum += inA * inB;
      sum2 += inA * inB2;
      inA = (int8_t) bitext((int) *pA, 2, 4);
      inB = *pB++;
      inB2 = *pB2++;
      sum += inA * inB;
      sum2 += inA * inB2;
      inA = (int8_t) bitext((int) *pA, 2, 6);
      inB = *pB++;
      inB2 = *pB2++;
      sum += inA * inB;
      sum2 += inA * inB2;

      pA++;
      col_cnt_im2col-=4;
    }
    if (flag_batch_norm && flag_relu)
    {
      uint8_t i_o = i & 0x01;
      out[i_o] = pulp_nn_bn_quant_u4(sum, *k, *lambda, out_shift);
      out2[i_o] = pulp_nn_bn_quant_u4(sum2, *k, *lambda, out_shift);
      k++;
      lambda++;
      if(i_o == 0x01)
      {
        *pOut = bitins(out[0], n_mask, out[1], mask, off);
        *pOut2 = bitins(out2[0], n_mask, out2[1], mask, off);
        pOut++;
        pOut2++;
      }
    }
    else
    {
      if (flag_relu == 1)
      {
        uint8_t i_o = i & 0x01;
        out[i_o] = pulp_nn_quant_u4(sum, out_mult, out_shift);
        out2[i_o] = pulp_nn_quant_u4(sum2, out_mult, out_shift);
        if(i_o == 0x01)
        {
          *pOut = bitins(out[0], n_mask, out[1], mask, off);
          *pOut2 = bitins(out2[0], n_mask, out2[1], mask, off);
          pOut++;
          pOut2++;
        }
      }
      else
      {
        uint8_t i_o = i & 0x01;
        out[i_o] = (uint8_t) clip8(sum >> out_shift);
        out2[i_o] = (uint8_t) clip8(sum2 >> out_shift);
        if(i_o == 0x01)
        {
          *pOut = bitins(out[0], n_mask, out[1], mask, off);
          *pOut2 = bitins(out2[0], n_mask, out2[1], mask, off);
          pOut++;
          pOut2++;
        }
      }
    }
    i++;
    chan_left--;
  }
  pOut+=ch_out_r;
  return pOut;
}
