#include "pmsis.h"

#include "pulp_nn_utils.h"

#include "pulp_nn_functions.h"

#define log2(x) __builtin_pulp_fl1(x)
#define min(a,b) ((a)<(b)?(a):(b))
#define SumDotp(a, b, c) __builtin_pulp_sdotusp4(a, b, c)
#define nn_round(out_shift) (0x1 << (out_shift -1))
#define bitins(dst,not_mask_imm,src,mask_imm,off) __builtin_pulp_binsert(dst,not_mask_imm,src,mask_imm,off)
#define bitext(x,size,off) __builtin_pulp_bextract(x,size,off)
#define clip8(x) __builtin_pulp_clipu_r(x, 3)

void pulp_nn_conv_u4_u2_i4(
          const uint8_t *pInBuffer,
					const uint16_t dim_in_x,
					const uint16_t dim_in_y,
					const uint16_t ch_in,
					const int8_t *pWeight,
					const uint16_t ch_out,
					const uint16_t dim_kernel_x,
					const uint16_t dim_kernel_y,
					const uint16_t padding_y_top,
					const uint16_t padding_y_bottom,
					const uint16_t padding_x_left,
					const uint16_t padding_x_right,
					const uint16_t stride_x,
					const uint16_t stride_y,
					const int8_t *bias,
					const uint16_t bias_shift,
					const int8_t out_shift,
          const uint16_t out_mult,
          uint8_t *pOutBuffer,
          const uint16_t dim_out_x,
          const uint16_t dim_out_y,
					int64_t *k,
					int64_t *lambda,
					uint8_t *pIm2ColBuffer,
          int flag_relu,
          int flag_batch_norm,
          unsigned int * memory_chan
) {
	uint16_t ch_in_r = ch_in >> 1;
	uint16_t ch_out_r = ch_out >> 2;

	int core_id = pi_core_id();
	uint8_t * pIm2ColBase = pIm2ColBuffer + (2 * core_id * ch_in * dim_kernel_x * dim_kernel_y);
	int i_out_y, i_out_x, i_ker_y, i_ker_x;
	int Log2Core;

	uint8_t extra_chunk = ((dim_out_y & (NUM_CORES-1)) != 0);
	uint8_t extra_chunk_r;
	uint16_t dim_out_x_r;
	uint8_t section;
	int core_id_r;

	if(extra_chunk && dim_out_x > 1)
	{
		Log2Core = log2(NUM_CORES >> 1);
		core_id_r = (core_id >> 1);
		dim_out_x_r = (dim_out_x >> 1);
		section = (core_id & 0x1);
		extra_chunk_r = ((dim_out_y & ((NUM_CORES >> 1) - 1)) != 0);
	}
	else
	{
		Log2Core = log2(NUM_CORES);
		core_id_r = core_id;
		dim_out_x_r = dim_out_x;
		section = 0;
		extra_chunk_r = extra_chunk;
		extra_chunk = 0;
	}

  uint8_t flag_dim_out_x_odd = dim_out_x & 0x01;

	int chunk = (dim_out_y >> Log2Core) + extra_chunk_r;

	int start_pixel = min((chunk * core_id_r), dim_out_y);
	int stop_pixel = min(start_pixel + chunk, dim_out_y);

	uint8_t *pIm2Col = pIm2ColBase;
	uint8_t *pOut = pOutBuffer + (start_pixel * ch_out_r * dim_out_x) + (section * ch_out_r * dim_out_x_r);

  for (i_out_y = start_pixel; i_out_y < stop_pixel; i_out_y++)
  {
		for(i_out_x=(section * dim_out_x_r); i_out_x<(dim_out_x_r + (section * (dim_out_x_r + flag_dim_out_x_odd))); i_out_x++)
    {
			if(i_out_y < padding_y_top)
      {
				for(i_ker_y=((i_out_y * stride_y) - padding_y_top); i_ker_y<((i_out_y * stride_y) - padding_y_top + dim_kernel_y); i_ker_y++)
        {
          for(i_ker_x=((i_out_x * stride_x) - padding_x_left); i_ker_x<((i_out_x * stride_x) - padding_x_left + dim_kernel_x); i_ker_x++)
          {
						if((i_ker_y < 0) || (i_ker_y >= dim_in_y) || (i_ker_x < 0) || (i_ker_x >= dim_in_x))
            {
              pulp_zero_mem(pIm2Col, ch_in);
            }
            else
            {
							pulp_nn_im2col_u4_to_u8((uint8_t*) (pInBuffer + ((i_ker_y * dim_in_x + i_ker_x) * ch_in_r)), pIm2Col, ch_in);
            }
            pIm2Col+=ch_in;
          }
        }
      }
			else if(i_out_y < dim_out_y - padding_y_bottom)
      {
        if(i_out_x < padding_x_left)
        {
          for(i_ker_y=((i_out_y * stride_y) - padding_y_top); i_ker_y<((i_out_y * stride_y) - padding_y_top + dim_kernel_y); i_ker_y++)
          {
            for(i_ker_x=((i_out_x * stride_x) - padding_x_left); i_ker_x<((i_out_x * stride_x) - padding_x_left + dim_kernel_x); i_ker_x++)
            {
							if((i_ker_x < 0) || (i_ker_x >= dim_in_x))
              {
                pulp_zero_mem(pIm2Col, ch_in);
              }
              else
              {
								pulp_nn_im2col_u4_to_u8((uint8_t*) (pInBuffer + ((i_ker_y * dim_in_x + i_ker_x) * ch_in_r)), pIm2Col, ch_in);
              }
              pIm2Col+=ch_in;
            }
          }
        }
        else if(i_out_x < (dim_out_x - padding_x_right))
        {
					for(i_ker_y=((i_out_y * stride_y) - padding_y_top); i_ker_y<((i_out_y * stride_y) - padding_y_top + dim_kernel_y); i_ker_y++)
          {
            pulp_nn_im2col_u4_to_u8((uint8_t*) pInBuffer + (i_ker_y * dim_in_x + i_out_x * stride_x - padding_x_left)*ch_in_r,pIm2Col,ch_in * dim_kernel_x);
            pIm2Col+=(ch_in * dim_kernel_x);
          }
        }
        else
        {
					for(i_ker_y=((i_out_y * stride_y) - padding_y_top); i_ker_y<((i_out_y * stride_y) - padding_y_top + dim_kernel_y); i_ker_y++)
          {
            for(i_ker_x = i_out_x * stride_x - padding_x_left; i_ker_x < i_out_x * stride_x - padding_x_left + dim_kernel_x; i_ker_x++)
            {
              if((i_ker_x < 0) || (i_ker_x >= dim_in_x))
              {
                pulp_zero_mem(pIm2Col, ch_in);
              }
              else
              {
								pulp_nn_im2col_u4_to_u8((uint8_t *)pInBuffer+ (i_ker_y*dim_in_x+i_ker_x)* ch_in_r, pIm2Col, ch_in);
              }
              pIm2Col+=ch_in;
            }
          }
        }
      }
      else
      {
				for(i_ker_y=((i_out_y * stride_y) - padding_y_top); i_ker_y<((i_out_y * stride_y) - padding_y_top + dim_kernel_y); i_ker_y++)
        {
          for(i_ker_x = i_out_x * stride_x - padding_x_left; i_ker_x < i_out_x * stride_x - padding_x_left + dim_kernel_x; i_ker_x++)
          {
						if(i_ker_y < 0 || (i_ker_y >= dim_in_y) || i_ker_x < 0 || i_ker_x >= dim_in_x)
            {
              pulp_zero_mem(pIm2Col, ch_in);
            }
            else
            {
              pulp_nn_im2col_u4_to_u8((uint8_t *) pInBuffer + (i_ker_y * dim_in_x + i_ker_x) * ch_in_r, pIm2Col, ch_in);
            }
            pIm2Col+=ch_in;
          }
        }
      }
      if(pIm2Col == (pIm2ColBase + ((ch_in * dim_kernel_x * dim_kernel_y) << 1)))
      {
        pOut = pulp_nn_matmul_u2_i4(
          pWeight,
          pIm2ColBase,
          ch_out, 
          (ch_in * dim_kernel_x * dim_kernel_y),
          bias_shift,
          out_shift,
          out_mult,
          k,
          lambda,
          bias,
          pOut,
          flag_relu,
          flag_batch_norm
        );
        pIm2Col = pIm2ColBase;
      }
    }
  //   pOut+=(extra_chunk * ((dim_out_x_r + ((1 - section) * flag_dim_out_x_odd)) * ch_out_r));
  // }

    if(pIm2Col != pIm2ColBase)
    {
      int8_t mask2 = 0x0c;
      int8_t n_mask2 = ~ mask2;
      int8_t mask4 = 0x30;
      int8_t n_mask4 = ~ mask4;
      int8_t mask6 = 0xc0;
      int8_t n_mask6 = ~ mask6;
      int8_t off2 = 2;
      int8_t off4 = 4;
      int8_t off6 = 6;
      const int8_t *pA = pWeight;
      int i;
      int64_t * k1 = k;
      int64_t * lambda1 = lambda;
      v4s inA[2];
      v4u inB;
      uint8_t out[4];
      for(i = 0; i < ch_out; i++)
      {
        int sum = 0;//((int)(bias[i]) << bias_shift);// + nn_round(out_shift);

        uint8_t *pB = pIm2ColBase;
        uint16_t col_cnt_im2col = ch_in * dim_kernel_x * dim_kernel_y >> 3;
        for(int j=0; j < col_cnt_im2col; j++)
        {
          inB = *((v4u*) pB);

          pB+=4;

          pulp_nn_i4_to_i8(pA,inA);

          sum = SumDotp(inB, inA[0], sum);

          inB = *((v4u*) pB);

          sum = SumDotp(inB, inA[1], sum);

          pB+=4;
          pA+=4;
        }
        col_cnt_im2col = (ch_in * dim_kernel_y * dim_kernel_x) & 0x7;
        while (col_cnt_im2col)
        {
          int8_t inA1 = (int8_t) bitext((int) *pA, 4, 0);
          uint8_t inB1 = *pB++;
          sum += inA1 * inB1;
          inA1 = (int8_t) bitext((int) *pA, 4, 4);
          inB1 = *pB++;
          sum += inA1 * inB1;

          pA++;
          col_cnt_im2col-=2;
        }
        if (flag_batch_norm && flag_relu)
        {
          uint8_t i_o = i & 0x03;
          out[i_o] = pulp_nn_bn_quant_u2(sum, *k1, *lambda1, out_shift);
          k1++;
          lambda1++;
          if(i_o == 0x03)
          {
            out[0] = bitins(out[0], n_mask2, out[1], mask2, off2);
            out[0] = bitins(out[0], n_mask4, out[2], mask4, off4);
            *pOut = bitins(out[0], n_mask6, out[3], mask6, off6);
            pOut++;
          }
        }
        else
        {
          if(flag_relu == 1)
          {
            uint8_t i_o = i & 0x03;
            out[i_o] = pulp_nn_quant_u2(sum, out_mult, out_shift);
            if(i_o == 0x03)
            {
              out[0] = bitins(out[0], n_mask2, out[1], mask2, off2);
              out[0] = bitins(out[0], n_mask4, out[2], mask4, off4);
              *pOut = bitins(out[0], n_mask6, out[3], mask6, off6);
              pOut++;
            }
          }
          else
          {
            uint8_t i_o = i & 0x03;
            out[i_o] = (uint8_t) clip8(sum >> out_shift);
            if(i_o == 0x03)
            {
              out[0] = bitins(out[0], n_mask2, out[1], mask2, off2);
              out[0] = bitins(out[0], n_mask4, out[2], mask4, off4);
              *pOut = bitins(out[0], n_mask6, out[3], mask6, off6);
              pOut++;
            }
          }
        }
      }
    }
    pOut+=(extra_chunk * ((dim_out_x_r + ((1 - section) * flag_dim_out_x_odd)) * ch_out_r));
    pIm2Col = pIm2ColBase;
  }
  pi_cl_team_barrier(0);
}
