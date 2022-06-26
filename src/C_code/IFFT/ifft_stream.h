//#ifndef FFT_H_
//#define FFT_H_
#include <ap_fixed.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef float DTYPE;
typedef int INTTYPE;
typedef ap_uint<1> ap_uint1;
typedef ap_uint<32> ap_uint32;
typedef ap_uint<64> ap_uint64;
typedef ap_axiu<64,1,1,1> axiu_64;
typedef hls::stream<axiu_64> strm_u64;



#define M 12			/* Number of Stages = Log2N */
#define SIZE 4096 		/* SIZE OF FFT */
#define SIZE2 SIZE>>1	/* SIZE/2 */

void ifft_streaming(strm_u64 &inStrm, strm_u64 &outStrm);
ap_uint32 Segment64(ap_uint64 input, ap_uint1 i);

union TypeTrans{
	unsigned int  i;
	DTYPE f;
};

//W_real and W_image are twiddle factors for 1024 size FFT.
//WW_R[i]=cos(e*i/SIZE);
//WW_I[i]=sin(e*i/SIZE);
//where i=[0,512) and DTYPE	e = -6.283185307178;
#include "tw_r_IFFT4096.h"
#include "tw_i_IFFT4096.h"

//#endif
