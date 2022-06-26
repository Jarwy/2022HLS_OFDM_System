#include "CP_Stream.h"
using namespace std;
float In_R[FFT_POINT], In_I[FFT_POINT];
int main()
{
	for(int i=0; i<FFT_POINT; i++){
		In_R[i] = i;
		In_I[i] = 0.0;

	}

	axiu_64 in64, out64;
	ap_uint64 in_tmp = 0;
	strm_u64 inStream, outStream;
	TypeTrans in_R_type, in_I_type;

	for(int k=0;k<SYM_NUM;k++){
		for(int s=0; s<FFT_POINT; s++){
			in_R_type.f = In_R[s];
			in_I_type.f = In_I[s];
			//printf("%d\n",In_R[s]);
			in_tmp = 0;
			in_tmp = (in_tmp|in_R_type.i)<<32;
			in_tmp = in_tmp|in_I_type.i;
			//cout<<k*4096+s<<"  "<<in_tmp<<endl;
			in64.data = in_tmp;
			in64.keep = 1; in64.strb = 1;
			in64.user = 1; in64.dest = 0;
			in64.id = 0;

			inStream.write(in64);

		}
	}

	cp_stream(inStream, outStream);

	TypeTrans x, y;
	for(int i = 0; i<array_size;i++){
		axiu_64 valOut;
		valOut = outStream.read();
		//cout<<i<<"  "<<valOut.data<<endl;
		x.i = (valOut.data >> 32);
		y.i = valOut.data;
		cout<<i<<"| "<< x.f << " " << y.f << endl;
	}
	return 0;
}
