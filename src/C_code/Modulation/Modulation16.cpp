#include <iostream>
#include <fstream>
#include <complex>
#include <stdio.h>
#include "Modulation16.h"
using namespace std;

//unsigned long long input =413;
//unsigned long long container[QamNUM][SampleNUM];
unsigned long long returnVal;
void MOD_QAM16(int inc_bit) {
	TypeTrans m, n;
	unsigned long long tmp;
	switch (inc_bit) {
	case 0:
		m.f = -3;
		n.f = 3;
		tmp = m.ui;
		tmp = (tmp << 32) | n.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	case 1:
		m.f = -3;
		n.f = 1;
		tmp = m.ui;
		tmp = (tmp << 32) | n.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	case 2:
		m.f = -3;
		n.f = -3;
		tmp = m.ui;
		tmp = (tmp << 32) | n.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	case 3:
		m.f = -3;
		n.f = -1;
		tmp = m.ui;
		tmp = (tmp << 32) | n.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	case 4:
		m.f = -1;
		n.f = 3;
		tmp = m.ui;
		tmp = (tmp << 32) | n.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	case 5:
		m.f = -1;
		n.f = 1;
		tmp = m.ui;
		tmp = (tmp << 32) | n.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	case 6:
		m.f = -1;
		n.f = -3;
		tmp = m.ui;
		tmp = (tmp << 32) | n.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	case 7:
		m.f = -1;
		n.f = -1;
		tmp = m.ui;
		tmp = (tmp << 32) | n.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	case 8:
		m.f = 3;
		n.f = 3;
		tmp = m.ui;
		tmp = (tmp << 32) | n.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	case 9:
		m.f = 3;
		n.f = 1;
		tmp = m.ui;
		tmp = (tmp << 32) | n.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	case 10:
		m.f = 3;
		n.f = -3;
		tmp = m.ui;
		tmp = (tmp << 32) | n.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	case 11:
		m.f = 3;
		n.f = -1;
		tmp = m.ui;
		tmp = (tmp << 32) | n.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	case 12:
		m.f = 1;
		n.f = 3;
		tmp = m.ui;
		tmp = (tmp << 32) | n.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	case 13:
		m.f = 1;
		n.f = 1;
		tmp = m.ui;
		tmp = (tmp << 32) | n.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	case 14:
		m.f = 1;
		n.f = -3;
		tmp = m.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	case 15:
		m.f = 1;
		n.f = -1;
		tmp = m.ui;
		tmp = (tmp << 32) | n.ui;
		//container[i][j] = tmp;
		returnVal = tmp;
		break;
	}
}


void QAM16_Streaming(strm_u64 &inStrm, strm_u64 &OutStrm){
#pragma HLS INTERFACE axis port=inStrm
#pragma HLS INTERFACE axis port=OutStrm
#pragma HLS INTERFACE s_axilite port=return
	axiu_64 inFirst, inLast;
	axiu_64 in64;
	for(int j = 0; j<SampleNUM; j++){
		in64= inStrm.read();
		if(j == 0)	inFirst = in64;
		else if(j == SampleNUM-1)	inLast = in64;

		for (int i = 0; i < QamNUM; i++) {
			int inc_bit = (in64.data >> 4*i) & 0x0F;
			MOD_QAM16(inc_bit);
			axiu_64 out;

			out.data = returnVal;

			if((j==SampleNUM-1) && (i==QamNUM-1)){
				out.last = inLast.last;
				out.keep = inLast.keep;
				out.strb = inLast.strb;
				out.user = inLast.user;
				out.dest = inLast.dest;
				out.id = inLast.id;
			}else{
					out.last = inFirst.last;
				out.keep = inFirst.keep;
				out.strb = inFirst.strb;
				out.user = inFirst.user;
				out.dest = inFirst.dest;
				out.id = inFirst.id;
			}
			/*
			if((j==SampleNUM-1) && (i==QamNUM-1)){
				out.last = in64[SampleNUM-1].last;
				out.keep = in64[SampleNUM-1].keep;
				out.strb = in64[SampleNUM-1].strb;
				out.user = in64[SampleNUM-1].user;
				out.dest = in64[SampleNUM-1].dest;
				out.id = in64[SampleNUM-1].id;
			}else{
				out.last = in64[0].last;
				out.keep = in64[0].keep;
				out.strb = in64[0].strb;
				out.user = in64[0].user;
				out.dest = in64[0].dest;
				out.id = in64[0].id;
			}
			*/
			OutStrm.write(out);
		}
	}


	return;
}
