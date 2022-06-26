#include "rmCP_Stream.h"

uint64_t output[FFT_POINT][SYM_NUM] = { 0 };
void rmCP_stream(strm_u64 &inStrm, strm_u64 &outStrm) {
#pragma HLS INTERFACE axis port=inStrm
#pragma HLS INTERFACE axis port=outStrm
#pragma HLS INTERFACE s_axilite port=return
	axiu_64 input[input_size];
	axiu_64 in;
	int count = 0;
	for (int i = 0; i < CP1_SIZE; i++) {
		in = inStrm.read();
	}
	for (int i = 0; i < FFT_POINT; i++) {
		in = inStrm.read();
		outStrm.write(in);
	}
	for(int j = 1; j<SYM_NUM; j++){
		for (int i = 0; i < CP2_SIZE; i++) {
			in = inStrm.read();
		}
		for (int i = 0; i < FFT_POINT; i++) {
			in = inStrm.read();
			outStrm.write(in);
		}
	}
}
