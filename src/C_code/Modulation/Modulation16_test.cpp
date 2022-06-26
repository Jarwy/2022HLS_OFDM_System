#include <fstream>
#include <iostream>
#include <ap_fixed.h>
#include "Modulation16.h"
using namespace std;


int main(){
	ap_uint64 inSig;
	strm_u64 inStream, outStream;

	for(int i=0;i<SampleNUM;i++){
		axiu_64 valIn_I;
		valIn_I.data = 413+i;
		valIn_I.keep = 1; valIn_I.strb = 1; valIn_I.user = 1;valIn_I.id = 0; valIn_I.dest = 0;
		inStream << valIn_I;
	}


	QAM16_Streaming(inStream, outStream);

	TypeTrans x, y;
	for(int j = 0;j<SampleNUM;j++){
		cout<<"The "<<j<<"th input:"<<endl;
		for (int i = 0; i < QamNUM; i++) {
			axiu_64 valOut;
			valOut = outStream.read();
			x.ui = (valOut.data >> 32);
			y.ui = valOut.data;
			cout << x.f << " " << y.f << endl;
		}
		cout<<"==============="<<endl;
	}


	return 0;
}
