#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <ap_fixed.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include "AWGN_channel.h"
using namespace std;
unsigned long seed1 = 790930;
unsigned long seed2 = 880413;
/*
double LSFR(unsigned long SR){
	SR = ((((SR >> 31)^ (SR >> 6)^ (SR >> 4)^ (SR >> 1)^ (SR))& 0x0001)<< 31)| (SR >> 1);
	return SR;
}
*/

float LSFR(ap_uint24 SR){
	SR = ((((SR >> 23)^ (SR >> 6)^ (SR >> 4)^ (SR >> 1)^ (SR))& 0x0001)<< 23)| (SR >> 1);
	return SR;
}

float division2(float input, int div_pow) {
	TypeTrans in, out;
	in.f = input;
	int in_int = (in.i);
	int mask = 2139095040;
	int maskInv = 8388607;
	int exp = (in_int & mask);
	int manti = (in_int & maskInv);
	int new_exp = (((exp >> 23) - div_pow) << 23);
	int new_val = manti | new_exp;
	out.i = new_val;
	return out.f;
}

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

void AWGN_channel(strm_u64 &inStrm, ap_uint8 snr, ap_uint4 sigma, ap_uint4 mu, strm_u64 &OutStrm) {
#pragma HLS INTERFACE axis port=inStrm
#pragma HLS INTERFACE axis port=OutStrm
#pragma HLS INTERFACE s_axilite port=snr bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=sigma bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=mu bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
//#pragma HLS INTERFACE ap_ctrl_none port=return

	int sigPower = 1;								//default signal power
	float reqSNR = pow(10 ,(snr / 10.0));				//turn SNR from dB to percentage
	float noisePower = sqrt((sigPower / reqSNR)/2.0);

	AWGN_channel_label0:
	for (int i = 0; i < N_point; i++){

		axiu_64 sig = inStrm.read();

		//tear real & Image apart
		ap_uint32 sig_I = Segment64(sig.data, 1);
		ap_uint32 sig_Q = Segment64(sig.data, 0);

		//data type transform
		TypeTrans Data_I;
		TypeTrans Data_Q;

		Data_I.f = sig_I; 	//float type
		Data_Q.f = sig_Q;	//float type

		//generate arbitrary number in range [0 1] in uniform distribution
		seed1 = LSFR(seed1);
		seed2 = LSFR(seed2);
		float X1 = seed1;
		//float X2 = seed2/16777216.0;
		float X2 = division2(seed2, 24);
		//generate White Gaussian Noise with Box-Muller method
		//float gn_I = sqrt(-2*log(X1))*cos(PI_2*X2)*sigma+mu;
		//float gn_Q = sqrt(-2*log(X1))*sin(PI_2*X2)*sigma+mu;
		float alpha = log(seed1);
		float beta = PI_2*X2;
		float f = sqrt(-2*(alpha-Normalize));
		//float gn_I = f*cos(beta)*sigma+mu;
		//float gn_Q = f*sin(beta)*sigma+mu;
		float gn_I = f;
		float gn_Q = f;
		gn_I = noisePower*gn_I;
		gn_Q = noisePower*gn_Q;

		//add WGN to input signal
		Data_I.f = Data_I.f + gn_I;
		Data_Q.f = Data_Q.f+ gn_Q;

		//merge real part & image part
		ap_uint64 Data_IQ = 0;
		Data_IQ = (Data_IQ | Data_I.i)<<32;
		Data_IQ = Data_IQ | Data_Q.i;

		//assign value to output data
		axiu_64 out;
		out.data = Data_IQ;
		out.keep = sig.keep; out.strb = sig.strb; out.user = sig.user;
		out.last = sig.last; out.id = sig.id; out.dest = sig.dest;

		OutStrm.write(out);
	}

	return;
}


