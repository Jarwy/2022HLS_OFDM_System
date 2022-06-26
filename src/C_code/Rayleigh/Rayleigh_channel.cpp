#include <math.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <ap_fixed.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include "Rayleigh_channel.h"

using namespace std;

int alpha = 0; //p.84;
ap_uint16 C = 300;//(Mm/s)
const float v = 16.67; //(m/s) about 60km/hr


ap_uint32 Segment64(ap_uint64 input, ap_uint1 i){
	//1 for real, 0 for image
	ap_uint32 out;
	if(i){
		out = (input>>32);
	}else{
		out = (input);
	}
	return out;
}

void Rayleigh_channel(strm_u64 &inStrm, ap_uint16 fc, ap_uint8 M, strm_u64 &outStrm_fade) {
#pragma HLS INTERFACE axis port=inStrm
#pragma HLS INTERFACE axis port=outStrm_fade
#pragma HLS INTERFACE s_axilite port=fc bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=M bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
//#pragma HLS INTERFACE ap_ctrl_none port=return bundle=CRTL_BUS

	float w_n, beta_n;
	ap_uint8 N = 4 * M + 2; //Number of sinusoids(p.102)
	float lamda = C / (float)fc; //wave length
	float fm = v / lamda; //Maximum Doppler Frequency
	float w_m = 2 * M_PI*fm;
	float cos_a = cos(alpha);
	float sin_a = sin(alpha);

	for(int idx = 0; idx<N_point; idx++){

		axiu_64 sig = inStrm.read();

		//tear real & Image apart
		ap_uint32 sig_I = Segment64(sig.data, 1);
		ap_uint32 sig_Q = Segment64(sig.data, 0);

		//data type transform
		TypeTrans inData_I;
		TypeTrans inData_Q;
		inData_I.f = sig_I;	//float type
		inData_Q.f = sig_Q;	//float type


		//sum of sinusoidal
		float coef_I = Sqrt_2*cos_a*cos(w_m*idx);
		float coef_Q = Sqrt_2*sin_a*cos(w_m*idx);

		for (int n = 1; n <= M; n++) {
			beta_n = (float)n*M_PI / (float)M;
			w_n = w_m*cos(2.0*M_PI*(float)n / (float)N);// p.83
			coef_I += 2 * cos(beta_n)*cos(w_n*idx);
			coef_Q += 2 * sin(beta_n)*cos(w_n*idx);
		}
		//multiply fading coefficient to input signal
		TypeTrans outData_I;
		TypeTrans outData_Q;
		outData_I.f = (inData_I.f*coef_I) - (inData_Q.f*coef_Q);
		outData_Q.f = (inData_I.f*coef_Q) + (inData_Q.f*coef_I);

		//merge real part & image part
		ap_uint64 Data_IQ = 0;
		Data_IQ = (Data_IQ | outData_I.i)<<32;
		Data_IQ = Data_IQ | outData_Q.i;

		axiu_64 out;
		out.data = Data_IQ;
		out.keep = sig.keep; out.strb = sig.strb; out.user = sig.user;
		out.last = sig.last; out.id = sig.id; out.dest = sig.dest;

		outStrm_fade.write(out);

	}

	return;
}
