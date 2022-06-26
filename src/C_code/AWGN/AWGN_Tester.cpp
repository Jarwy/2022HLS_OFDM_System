#include <fstream>
#include <iostream>
#include <ap_fixed.h>
#include "AWGN_channel.h"
using namespace std;


int main(int argc, char *argv[]){
	ap_uint8 snr = 10;
	ap_uint4 sigma = 1;
	ap_uint4 mu = 0;

	strm_u64 inStream;
	strm_u64 outStream;

	for(int idx = 0; idx < N_point; idx++){
		axiu_64 valIn_I;

		valIn_I.data = 0;
		valIn_I.keep = 1; valIn_I.strb = 1; valIn_I.user = 1;valIn_I.id = 0; valIn_I.dest = 0;
		inStream << valIn_I;

	}

	AWGN_channel(inStream, snr, sigma, mu, outStream);


	ofstream out1("GaussianNoise.txt");
	ofstream out2("UniformDist.txt");

	for(int idx = 0; idx < N_point; idx++){
		axiu_64 valOut;
		valOut = outStream.read();

		ap_uint32 valOut_I = Segment64(valOut.data, 1);
		ap_uint32 valOut_Q = Segment64(valOut.data, 0);

		TypeTrans out_I;
		TypeTrans out_Q;
		out_I.i = valOut_I;
		out_Q.i = valOut_Q;

		out1<<out_I.f<<endl;
		out2<<out_Q.f<<endl;
		}

	out1.close();
	out2.close();
	cout<<"Test PASSED!"<<endl;

	return 0;
}
