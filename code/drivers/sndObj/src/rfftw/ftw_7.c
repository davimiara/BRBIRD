/*
 * Copyright (c) 1997-1999 Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* This file was automatically generated --- DO NOT EDIT */
/* Generated on Sun Nov  7 20:44:33 EST 1999 */

#include <fftw-int.h>
#include <fftw.h>

/* Generated by: ./genfft -magic-alignment-check -magic-twiddle-load-all -magic-variables 4 -magic-loopi -twiddle 7 */

/*
 * This function contains 72 FP additions, 60 FP multiplications,
 * (or, 60 additions, 48 multiplications, 12 fused multiply/add),
 * 24 stack variables, and 28 memory accesses
 */
static const fftw_real K222520933 = FFTW_KONST(+0.222520933956314404288902564496794759466355569);
static const fftw_real K900968867 = FFTW_KONST(+0.900968867902419126236102319507445051165919162);
static const fftw_real K623489801 = FFTW_KONST(+0.623489801858733530525004884004239810632274731);
static const fftw_real K433883739 = FFTW_KONST(+0.433883739117558120475768332848358754609990728);
static const fftw_real K974927912 = FFTW_KONST(+0.974927912181823607018131682993931217232785801);
static const fftw_real K781831482 = FFTW_KONST(+0.781831482468029808708444526674057750232334519);

/*
 * Generator Id's : 
 * $Id: ftw_7.c,v 1.1.1.1 2006/05/12 15:14:50 veplaini Exp $
 * $Id: ftw_7.c,v 1.1.1.1 2006/05/12 15:14:50 veplaini Exp $
 * $Id: ftw_7.c,v 1.1.1.1 2006/05/12 15:14:50 veplaini Exp $
 */

void fftw_twiddle_7(fftw_complex *A, const fftw_complex *W, int iostride, int m, int dist)
{
     int i;
     fftw_complex *inout;
     inout = A;
     for (i = m; i > 0; i = i - 1, inout = inout + dist, W = W + 6) {
	  fftw_real tmp1;
	  fftw_real tmp53;
	  fftw_real tmp12;
	  fftw_real tmp54;
	  fftw_real tmp38;
	  fftw_real tmp50;
	  fftw_real tmp23;
	  fftw_real tmp55;
	  fftw_real tmp44;
	  fftw_real tmp51;
	  fftw_real tmp34;
	  fftw_real tmp56;
	  fftw_real tmp41;
	  fftw_real tmp52;
	  ASSERT_ALIGNED_DOUBLE;
	  tmp1 = c_re(inout[0]);
	  tmp53 = c_im(inout[0]);
	  {
	       fftw_real tmp6;
	       fftw_real tmp36;
	       fftw_real tmp11;
	       fftw_real tmp37;
	       ASSERT_ALIGNED_DOUBLE;
	       {
		    fftw_real tmp3;
		    fftw_real tmp5;
		    fftw_real tmp2;
		    fftw_real tmp4;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp3 = c_re(inout[iostride]);
		    tmp5 = c_im(inout[iostride]);
		    tmp2 = c_re(W[0]);
		    tmp4 = c_im(W[0]);
		    tmp6 = (tmp2 * tmp3) - (tmp4 * tmp5);
		    tmp36 = (tmp4 * tmp3) + (tmp2 * tmp5);
	       }
	       {
		    fftw_real tmp8;
		    fftw_real tmp10;
		    fftw_real tmp7;
		    fftw_real tmp9;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp8 = c_re(inout[6 * iostride]);
		    tmp10 = c_im(inout[6 * iostride]);
		    tmp7 = c_re(W[5]);
		    tmp9 = c_im(W[5]);
		    tmp11 = (tmp7 * tmp8) - (tmp9 * tmp10);
		    tmp37 = (tmp9 * tmp8) + (tmp7 * tmp10);
	       }
	       tmp12 = tmp6 + tmp11;
	       tmp54 = tmp11 - tmp6;
	       tmp38 = tmp36 - tmp37;
	       tmp50 = tmp36 + tmp37;
	  }
	  {
	       fftw_real tmp17;
	       fftw_real tmp42;
	       fftw_real tmp22;
	       fftw_real tmp43;
	       ASSERT_ALIGNED_DOUBLE;
	       {
		    fftw_real tmp14;
		    fftw_real tmp16;
		    fftw_real tmp13;
		    fftw_real tmp15;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp14 = c_re(inout[2 * iostride]);
		    tmp16 = c_im(inout[2 * iostride]);
		    tmp13 = c_re(W[1]);
		    tmp15 = c_im(W[1]);
		    tmp17 = (tmp13 * tmp14) - (tmp15 * tmp16);
		    tmp42 = (tmp15 * tmp14) + (tmp13 * tmp16);
	       }
	       {
		    fftw_real tmp19;
		    fftw_real tmp21;
		    fftw_real tmp18;
		    fftw_real tmp20;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp19 = c_re(inout[5 * iostride]);
		    tmp21 = c_im(inout[5 * iostride]);
		    tmp18 = c_re(W[4]);
		    tmp20 = c_im(W[4]);
		    tmp22 = (tmp18 * tmp19) - (tmp20 * tmp21);
		    tmp43 = (tmp20 * tmp19) + (tmp18 * tmp21);
	       }
	       tmp23 = tmp17 + tmp22;
	       tmp55 = tmp22 - tmp17;
	       tmp44 = tmp42 - tmp43;
	       tmp51 = tmp42 + tmp43;
	  }
	  {
	       fftw_real tmp28;
	       fftw_real tmp39;
	       fftw_real tmp33;
	       fftw_real tmp40;
	       ASSERT_ALIGNED_DOUBLE;
	       {
		    fftw_real tmp25;
		    fftw_real tmp27;
		    fftw_real tmp24;
		    fftw_real tmp26;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp25 = c_re(inout[3 * iostride]);
		    tmp27 = c_im(inout[3 * iostride]);
		    tmp24 = c_re(W[2]);
		    tmp26 = c_im(W[2]);
		    tmp28 = (tmp24 * tmp25) - (tmp26 * tmp27);
		    tmp39 = (tmp26 * tmp25) + (tmp24 * tmp27);
	       }
	       {
		    fftw_real tmp30;
		    fftw_real tmp32;
		    fftw_real tmp29;
		    fftw_real tmp31;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp30 = c_re(inout[4 * iostride]);
		    tmp32 = c_im(inout[4 * iostride]);
		    tmp29 = c_re(W[3]);
		    tmp31 = c_im(W[3]);
		    tmp33 = (tmp29 * tmp30) - (tmp31 * tmp32);
		    tmp40 = (tmp31 * tmp30) + (tmp29 * tmp32);
	       }
	       tmp34 = tmp28 + tmp33;
	       tmp56 = tmp33 - tmp28;
	       tmp41 = tmp39 - tmp40;
	       tmp52 = tmp39 + tmp40;
	  }
	  {
	       fftw_real tmp47;
	       fftw_real tmp46;
	       fftw_real tmp59;
	       fftw_real tmp60;
	       ASSERT_ALIGNED_DOUBLE;
	       c_re(inout[0]) = tmp1 + tmp12 + tmp23 + tmp34;
	       tmp47 = (K781831482 * tmp38) + (K974927912 * tmp44) + (K433883739 * tmp41);
	       tmp46 = tmp1 + (K623489801 * tmp12) - (K900968867 * tmp34) - (K222520933 * tmp23);
	       c_re(inout[6 * iostride]) = tmp46 - tmp47;
	       c_re(inout[iostride]) = tmp46 + tmp47;
	       {
		    fftw_real tmp49;
		    fftw_real tmp48;
		    fftw_real tmp45;
		    fftw_real tmp35;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp49 = (K433883739 * tmp38) + (K974927912 * tmp41) - (K781831482 * tmp44);
		    tmp48 = tmp1 + (K623489801 * tmp23) - (K222520933 * tmp34) - (K900968867 * tmp12);
		    c_re(inout[4 * iostride]) = tmp48 - tmp49;
		    c_re(inout[3 * iostride]) = tmp48 + tmp49;
		    tmp45 = (K974927912 * tmp38) - (K781831482 * tmp41) - (K433883739 * tmp44);
		    tmp35 = tmp1 + (K623489801 * tmp34) - (K900968867 * tmp23) - (K222520933 * tmp12);
		    c_re(inout[5 * iostride]) = tmp35 - tmp45;
		    c_re(inout[2 * iostride]) = tmp35 + tmp45;
	       }
	       c_im(inout[0]) = tmp50 + tmp51 + tmp52 + tmp53;
	       tmp59 = (K974927912 * tmp54) - (K781831482 * tmp56) - (K433883739 * tmp55);
	       tmp60 = (K623489801 * tmp52) + tmp53 - (K900968867 * tmp51) - (K222520933 * tmp50);
	       c_im(inout[2 * iostride]) = tmp59 + tmp60;
	       c_im(inout[5 * iostride]) = tmp60 - tmp59;
	       {
		    fftw_real tmp61;
		    fftw_real tmp62;
		    fftw_real tmp57;
		    fftw_real tmp58;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp61 = (K433883739 * tmp54) + (K974927912 * tmp56) - (K781831482 * tmp55);
		    tmp62 = (K623489801 * tmp51) + tmp53 - (K222520933 * tmp52) - (K900968867 * tmp50);
		    c_im(inout[3 * iostride]) = tmp61 + tmp62;
		    c_im(inout[4 * iostride]) = tmp62 - tmp61;
		    tmp57 = (K781831482 * tmp54) + (K974927912 * tmp55) + (K433883739 * tmp56);
		    tmp58 = (K623489801 * tmp50) + tmp53 - (K900968867 * tmp52) - (K222520933 * tmp51);
		    c_im(inout[iostride]) = tmp57 + tmp58;
		    c_im(inout[6 * iostride]) = tmp58 - tmp57;
	       }
	  }
     }
}

static const int twiddle_order[] =
{1, 2, 3, 4, 5, 6};
fftw_codelet_desc fftw_twiddle_7_desc =
{
     "fftw_twiddle_7",
     (void (*)()) fftw_twiddle_7,
     7,
     FFTW_FORWARD,
     FFTW_TWIDDLE,
     154,
     6,
     twiddle_order,
};
