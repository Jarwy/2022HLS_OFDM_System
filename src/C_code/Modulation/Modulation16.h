#define QamNUM 16
#define SampleNUM 3584
#include <ap_fixed.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
typedef ap_uint<1> ap_uint1;
typedef ap_uint<32> ap_uint32;
typedef ap_uint<64> ap_uint64;
typedef ap_axiu<64,1,1,1> axiu_64;
typedef hls::stream<axiu_64> strm_u64;

ap_uint32 Segment64(ap_uint64 input, ap_uint1 i);
void QAM16_Streaming(strm_u64 &inStrm, strm_u64 &OutStrm);
void MOD_QAM16(int inc_bit);
union TypeTrans{
	unsigned int  ui;
	float f;
};
