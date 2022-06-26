#include "rmCP_Stream.h"

int main(){
	axiu_64 in64, out64;
	ap_uint64 in_tmp = 0;
	strm_u64 inStream, outStream;
	TypeTrans in_R_type, in_I_type;
	for(int i=0; i<input_size; i++){
		//cout<<i<<endl;
		in_R_type.f = i;
		in_I_type.f = 0.0;
		//printf("%d\n",In_R[s]);
		in_tmp = 0;
		in_tmp = (in_tmp|in_R_type.i)<<32;
		in_tmp = in_tmp|in_I_type.i;
		in64.data = in_tmp;
		in64.keep = 1; in64.strb = 1; in64.user = 1;in64.id = 0; in64.dest = 0;
		inStream.write(in64);

	}
	rmCP_stream(inStream, outStream);

	TypeTrans x, y;
	for(int j = 0; j<SYM_NUM;j++){
		for(int i = 0; i<FFT_POINT; i++){
			axiu_64 valOut;
			valOut = outStream.read();
			x.i = (valOut.data >> 32);
			y.i = valOut.data;
			cout<<i<<"| "<< x.f << " " << y.f << endl;
		}
	cout<<"\n---------------------------------------------------------"<<endl;
	}
	return 0;
}
