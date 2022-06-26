#define N_point 61440
#define PI_2 6.2831852
#define Normalize 16.63553 //ln(16777216.0)
#include <ap_fixed.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
typedef ap_uint<1> ap_uint1;
typedef ap_uint<4> ap_uint4;
typedef ap_uint<8> ap_uint8;
typedef ap_uint<24> ap_uint24;
typedef ap_uint<32> ap_uint32;
typedef ap_uint<64> ap_uint64;
typedef ap_axiu<64,1,1,1> axiu_64;
typedef hls::stream<axiu_64> strm_u64;


ap_uint32 Segment64(ap_uint64 input, ap_uint1 i);

void AWGN_channel(strm_u64 &inStrm, ap_uint8 snr, ap_uint4 sigma, ap_uint4 mu, strm_u64 &OutStrm);


union TypeTrans{
	unsigned int  i;
	float f;
};

