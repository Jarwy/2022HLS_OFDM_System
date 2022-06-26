#define N_point 61440
#define Sqrt_2 1.41421356
#include <ap_fixed.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
typedef ap_uint<1> ap_uint1;
typedef ap_uint<8> ap_uint8;
typedef ap_uint<16> ap_uint16;
typedef ap_uint<32> ap_uint32;
typedef ap_uint<64> ap_uint64;
typedef ap_axiu<64,1,1,1> axiu_64;
typedef hls::stream<axiu_64> strm_u64;

ap_uint32 Segment64(ap_uint64 input, ap_uint1 i);

void Rayleigh_channel(strm_u64 &inStrm, ap_uint16 fc, ap_uint8 M, strm_u64 &fade);

union TypeTrans
{
	unsigned int  i;
	float f;
};
