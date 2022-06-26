#include "CP_Stream.h"

void cp_stream(strm_u64 &inStrm, strm_u64 &outStrm){
#pragma HLS INTERFACE axis port=inStrm
#pragma HLS INTERFACE axis port=outStrm
#pragma HLS INTERFACE s_axilite port=return
	int cp_size;
	axiu_64 out64;
	axiu_64 in64, in64_first, in64_last;
	ap_uint64 slot[FFT_POINT+CP1_SIZE];
	for(int j = 0; j<SYM_NUM; j++){
		for(int i = 0; i<FFT_POINT; i++){
			if(j==0) cp_size = CP1_SIZE;
			else cp_size = CP2_SIZE;
			in64 = inStrm.read();
			if(i==0)	in64_first = in64;
			if(i==FFT_POINT-1) in64_last = in64;

			if(i<FFT_POINT-cp_size){
				slot[(i+cp_size)] = in64.data;		//filling
			}
			else{
				slot[(i+cp_size)] = in64.data;		//filling
				slot[(FFT_POINT-i-1)] = in64.data;	//copy&flip
			}
		}
		if(j==0) cp_size = CP1_SIZE;
		else cp_size = CP2_SIZE;
		for(int n=0; n<(FFT_POINT+cp_size); n++){
			out64.data = slot[n];
			if((j==SYM_NUM-1) && (n==(FFT_POINT+cp_size-1))){
				out64.last = in64_last.last;
				out64.keep = in64_last.keep;
			}else{
				out64.last = in64_first.last;
				out64.keep = in64_first.keep;
			}
			outStrm.write(out64);
		}
	}

}
