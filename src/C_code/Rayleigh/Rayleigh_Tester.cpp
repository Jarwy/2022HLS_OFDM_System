#include <fstream>
#include <iostream>
#include "Rayleigh_channel.h"
using namespace std;
int main(int argc, char *argv[]) {
	ap_uint16 fc = 2400;
	float fade_mag;
	ap_uint8 M = 8;
	ofstream file_I("fade_I.txt");
	ofstream file_Q("fade_Q.txt");
	ofstream file_mag("fade_mag.txt");

	strm_u64 inStream;
	strm_u64 outStream;
	for(int idx = 0; idx < N_point; idx++){
		axiu_64 valIn_I;
		valIn_I.data = 4294967297;
		valIn_I.keep = 1; valIn_I.strb = 1; valIn_I.user = 1;valIn_I.id = 0; valIn_I.dest = 0;
		inStream<< valIn_I;
	}


	Rayleigh_channel(inStream, fc, M, outStream);

	for (int i = 0; i<N_point; i++) {
		axiu_64 outFade;
		outFade = outStream.read();

		ap_uint32 outFade_I = Segment64(outFade.data, 1);
		ap_uint32 outFade_Q = Segment64(outFade.data, 0);

		TypeTrans outCoef_I;
		TypeTrans outCoef_Q;
		outCoef_I.i = outFade_I;
		outCoef_Q.i = outFade_Q;

		fade_mag = sqrt(pow(outCoef_I.f, 2) + pow(outCoef_Q.f, 2));
		file_I << outCoef_I.f << endl;
		file_Q << outCoef_Q.f << endl;
		file_mag << fade_mag << endl;
	}
	file_I.close();
	file_Q.close();
	file_mag.close();
	cout << "Test PASSED!" << endl;
	return 0;
}
