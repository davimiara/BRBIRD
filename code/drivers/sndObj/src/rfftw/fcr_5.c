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
/* Generated on Sun Nov  7 20:44:19 EST 1999 */

#include <fftw-int.h>
#include <fftw.h>

/* Generated by: ./genfft -magic-alignment-check -magic-twiddle-load-all -magic-variables 4 -magic-loopi -hc2real 5 */

/*
 * This function contains 12 FP additions, 7 FP multiplications,
 * (or, 8 additions, 3 multiplications, 4 fused multiply/add),
 * 12 stack variables, and 10 memory accesses
 */
static const fftw_real K2_000000000 = FFTW_KONST(+2.000000000000000000000000000000000000000000000);
static const fftw_real K1_118033988 = FFTW_KONST(+1.118033988749894848204586834365638117720309180);
static const fftw_real K500000000 = FFTW_KONST(+0.500000000000000000000000000000000000000000000);
static const fftw_real K1_902113032 = FFTW_KONST(+1.902113032590307144232878666758764286811397268);
static const fftw_real K1_175570504 = FFTW_KONST(+1.175570504584946258337411909278145537195304875);

/*
 * Generator Id's : 
 * $Id: fcr_5.c,v 1.1.1.1 2006/05/12 15:14:50 veplaini Exp $
 * $Id: fcr_5.c,v 1.1.1.1 2006/05/12 15:14:50 veplaini Exp $
 * $Id: fcr_5.c,v 1.1.1.1 2006/05/12 15:14:50 veplaini Exp $
 */

void fftw_hc2real_5(const fftw_real *real_input, const fftw_real *imag_input, fftw_real *output, int real_istride, int imag_istride, int ostride)
{
     fftw_real tmp10;
     fftw_real tmp12;
     fftw_real tmp1;
     fftw_real tmp4;
     fftw_real tmp5;
     fftw_real tmp6;
     fftw_real tmp11;
     fftw_real tmp7;
     ASSERT_ALIGNED_DOUBLE;
     {
	  fftw_real tmp8;
	  fftw_real tmp9;
	  fftw_real tmp2;
	  fftw_real tmp3;
	  ASSERT_ALIGNED_DOUBLE;
	  tmp8 = imag_input[imag_istride];
	  tmp9 = imag_input[2 * imag_istride];
	  tmp10 = (K1_175570504 * tmp8) - (K1_902113032 * tmp9);
	  tmp12 = (K1_902113032 * tmp8) + (K1_175570504 * tmp9);
	  tmp1 = real_input[0];
	  tmp2 = real_input[real_istride];
	  tmp3 = real_input[2 * real_istride];
	  tmp4 = tmp2 + tmp3;
	  tmp5 = tmp1 - (K500000000 * tmp4);
	  tmp6 = K1_118033988 * (tmp2 - tmp3);
     }
     output[0] = tmp1 + (K2_000000000 * tmp4);
     tmp11 = tmp6 + tmp5;
     output[ostride] = tmp11 - tmp12;
     output[4 * ostride] = tmp11 + tmp12;
     tmp7 = tmp5 - tmp6;
     output[2 * ostride] = tmp7 - tmp10;
     output[3 * ostride] = tmp7 + tmp10;
}

fftw_codelet_desc fftw_hc2real_5_desc =
{
     "fftw_hc2real_5",
     (void (*)()) fftw_hc2real_5,
     5,
     FFTW_BACKWARD,
     FFTW_HC2REAL,
     125,
     0,
     (const int *) 0,
};
