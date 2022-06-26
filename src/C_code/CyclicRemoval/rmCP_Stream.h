#include <cstdint>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
using namespace std;

typedef ap_uint<1> ap_uint1;
typedef ap_uint<32> ap_uint32;
typedef ap_uint<64> ap_uint64;
typedef ap_axiu<64,0,0,0> axiu_64;
typedef hls::stream<axiu_64> strm_u64;

#define CP1_SIZE 	313		//First Cyclic Prefix Size
#define CP2_SIZE 	291		//Other Cyclic Prefix Size
#define SYM_NUM	 	14 		//Symbol number per slot
#define	FFT_POINT	4096	//Number of FFT point
/*
#define CP1_SIZE 	3		//First Cyclic Prefix Size
#define CP2_SIZE 	2		//Other Cyclic Prefix Size
#define SYM_NUM	 	5 		//Symbol number per slot
#define	FFT_POINT	10		//Number of FFT point
*/
const int input_size = (FFT_POINT + CP1_SIZE) + (FFT_POINT + CP2_SIZE)*(SYM_NUM - 1);


void rmCP_stream(strm_u64 &inStrm, strm_u64 &outStrm);
union TypeTrans{
	unsigned int  i;
	float f;
};
