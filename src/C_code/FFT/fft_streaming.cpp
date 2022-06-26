																																														#include "fft_streaming.h"
#include "math.h"
#include <iostream>
#include <stdio.h>
using namespace std;

ap_uint32 Segment64(ap_uint64 input, ap_uint1 i){
	//1 for real, 0 for image
	ap_uint32 out;
	if(i){
		out = (input>>32);
	}else{
		out = (input);
	}
	return out;
}

unsigned int reverse_bits(unsigned int input) {
	int i, rev = 0;
	for (i = 0; i < M; i++) {
		rev = (rev << 1) | (input & 1);
		input = input >> 1;
	}
	return rev;
}

void bit_reverse(DTYPE X_R[SIZE], DTYPE X_I[SIZE],
		 DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {
  unsigned int reversed;
  unsigned int i;
  DTYPE temp;
  for (int i = 0; i < SIZE; i++) {
	  reversed = reverse_bits(i); // Find the bit reversed index
	  //printf("reserve:%d\n",reversed);
	  //printf("i:%d\n",i);
		if (i <= reversed) {
			// Swap the real values
			temp = X_R[i];
			OUT_R[i] = X_R[reversed];
			OUT_R[reversed] = temp;

			// Swap the imaginary values
			temp = X_I[i];
			OUT_I[i] = X_I[reversed];
			OUT_I[reversed] = temp;
		}
	}
}

void fft_stage(int stage, DTYPE X_R[SIZE], DTYPE X_I[SIZE],
		     DTYPE Out_R[SIZE], DTYPE Out_I[SIZE]) {
  int DFTpts = 1 << stage;    // DFT = 2^stage = points in sub DFT
  int numBF = DFTpts / 2;     // Butterfly WIDTHS in sub-DFT
  int step = SIZE >> stage;
  // Perform butterflies for j-th stage
 butterfly_loop:
  for (int j = 0; j < numBF; j++) {
    // Compute butterflies that use same W**k
  dft_loop:
      for(int t = 0; t < step; t++) {
          int i = j + t*DFTpts;
          //    for (int i = j; i < SIZE; i += DFTpts) {
#pragma HLS pipeline
          int k = j*step;
		  DTYPE c = W_real[k];
		  DTYPE s = W_imag[k];
		  int i_lower = i + numBF; // index of lower point in butterfly
		  DTYPE temp_R = X_R[i_lower] * c - X_I[i_lower] * s;
		  DTYPE temp_I = X_I[i_lower] * c + X_R[i_lower] * s;
		  Out_R[i_lower] = X_R[i] - temp_R;
		  Out_I[i_lower] = X_I[i] - temp_I;
		  Out_R[i] = X_R[i] + temp_R;
		  Out_I[i] = X_I[i] + temp_I;
    }
  }
}
//DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]
void fft_streaming(strm_u64 &inStrm, strm_u64 &outStrm)
{
#pragma HLS INTERFACE axis port=inStrm
#pragma HLS INTERFACE axis port=outStrm
#pragma HLS INTERFACE s_axilite port=return
  //#pragma HLS dataflow
  DTYPE Stage_R[2][SIZE], Stage_I[2][SIZE];
  //DTYPE X_R[SIZE], X_I[SIZE];
  //DTYPE OUT_R[SIZE], OUT_I[SIZE];
  axiu_64 in64[SIZE], out64[SIZE];
  TypeTrans in_R_type, in_I_type, out_R_type, out_I_type;
  for(int s=0; s<SIZE; s++){
	  in64[s] = inStrm.read();

	  //tear real & Image apart
	  ap_uint32 in_R = Segment64(in64[s].data, 1);
	  ap_uint32 in_I = Segment64(in64[s].data, 0);
	  in_R_type.i = in_R;
	  in_I_type.i = in_I;
	  Stage_R[0][s] = in_R_type.f;
	  Stage_I[0][s] = in_I_type.f;
  }
  #pragma HLS array_partition variable=Stage_R dim=1 complete
  #pragma HLS array_partition variable=Stage_I dim=1 complete

  bit_reverse(Stage_R[0], Stage_I[0], Stage_R[1], Stage_I[1]);
  memcpy(Stage_R[0] ,Stage_R[1],sizeof(Stage_R[0]));
  memcpy(Stage_I[0] ,Stage_I[1],sizeof(Stage_I[0]));
 stage_loop:
  for (int stage = 1; stage < M; stage++) { // Do M-1 stages of butterflies
    #pragma HLS unroll
    fft_stage(stage, Stage_R[0], Stage_I[0], Stage_R[1], Stage_I[1]);
    memcpy(Stage_R[0] ,Stage_R[1],sizeof(Stage_R[0]));
    memcpy(Stage_I[0] ,Stage_I[1],sizeof(Stage_I[0]));
  }
  fft_stage(M, Stage_R[0], Stage_I[0], Stage_R[1], Stage_I[1]);

  ap_uint64 out_tmp = 0;
  for(int s=0; s<SIZE; s++){
	  out_R_type.f = Stage_R[1][s];
	  out_I_type.f =Stage_I[1][s];
	  out_tmp = 0;
	  out_tmp = (out_tmp|out_R_type.i)<<32;
	  out_tmp = out_tmp|out_I_type.i;

	  out64[s].data = out_tmp;
	  out64[s].keep = in64[s].keep; out64[s].strb = in64[s].strb; out64[s].user = in64[s].user;
	  out64[s].last = in64[s].last; out64[s].id = in64[s].id; out64[s].dest = in64[s].dest;
	  outStrm.write(out64[s]);

    }
}
