#include "QAM16_d.h"
#include <iostream>

using namespace std;

unsigned long long container[SampleNUM];
uint_4 bitData;

uint_4 QAM16_demod(din_t in_bit){
	float real = 0.0, imag = 0.0;
	my_converter.ui = (in_bit);
	imag = my_converter.f;
	my_converter.ui = in_bit >> 32;
	real = my_converter.f;
	int quad;
	int mapPoint[4][1];
	float miniDist;
	float dist;
	int idx = 0;

	if(real * imag > 0){
		if(real > 0){
			quad = 1;
		}
		else{
			quad = 3;
		}
	}
	else if(real * imag < 0){
		if(real > 0){
			quad = 4;
		}
		else{
			quad = 2;
		}
	}
	else if(real * imag == 0){
		if(imag == 0 && real > 0){
			quad = 1;
		}
		else if(real < 0){
			quad = 3;
		}
		if(real == 0 && imag > 0){
			quad = 2;
		}
		else if(imag < 0 ){
			quad = 4;
		}
		if(real == 0 && imag == 0){
			quad = 0;
		}
	}

	switch(quad){
		case 0:
			my_converter.i = 0;
			bitData = my_converter.ui;
			break;
		case 1:
			mapPoint[0][0] = 8;
			mapPoint[1][0] = 9;
			mapPoint[2][0] = 12;
			mapPoint[3][0] = 13;

			for(int k = 0; k < 4;k++){
				if(k == 0){
					dist = (real - 3)*(real - 3) + (imag - 3)*(imag - 3);
					miniDist = dist;
					idx = 0;
				}
				else if(k == 1){
					dist = (real - 3)*(real - 3) + (imag - 1)*(imag - 1);
					if(dist < miniDist){
						miniDist = dist;
						idx = 1;
					}
				}
				else if(k == 2){
					dist = (real - 1)*(real - 1) + (imag - 3)*(imag - 3);
					if(dist < miniDist){
						miniDist = dist;
						idx = 2;
					}
				}
				else if(k == 3){
					dist = (real - 1)*(real - 1) + (imag - 1)*(imag - 1);
					if(dist < miniDist){
						miniDist = dist;
						idx = 3;
					}
				}
			}
			my_converter.i = mapPoint[idx][0];
			bitData = my_converter.ui;
			break;
		case 2:
			mapPoint[0][0] = 0;
			mapPoint[1][0] = 1;
			mapPoint[2][0] = 4;
			mapPoint[3][0] = 5;

			for(int k = 0; k < 4;k++){
				if(k == 0){
					dist = (real - (-3))*(real - (-3)) + (imag - 3)*(imag - 3);
					miniDist = dist;
					idx = 0;
				}
				else if(k == 1){
					dist = (real - (-3))*(real - (-3)) + (imag - 1)*(imag - 1);
					if(dist < miniDist){
						miniDist = dist;
						idx = 1;
					}
				}
				else if(k == 2){
					dist = (real - (-1))*(real - (-1)) + (imag - 3)*(imag - 3);
					if(dist < miniDist){
						miniDist = dist;
						idx = 2;
					}
				}
				else if(k == 3){
					dist = (real - (-1))*(real - (-1)) + (imag - 1)*(imag - 1);
					if(dist < miniDist){
						miniDist = dist;
						idx = 3;
					}
				}
			}
			my_converter.i = mapPoint[idx][0];
			bitData = my_converter.ui;
			break;
		case 3:
			mapPoint[0][0] = 2;
			mapPoint[1][0] = 3;
			mapPoint[2][0] = 6;
			mapPoint[3][0] = 7;

			for(int k = 0; k < 4;k++){
				if(k == 0){
					dist = (real - (-3))*(real - (-3)) + (imag - (-3))*(imag - (-3));
					miniDist = dist;
					idx = 0;
				}
				else if(k == 1){
					dist = (real - (-3))*(real - (-3)) + (imag - (-1))*(imag - (-1));
					if(dist < miniDist){
						miniDist = dist;
						idx = 1;
					}
				}
				else if(k == 2){
					dist = (real - (-1))*(real - (-1)) + (imag - (-3))*(imag - (-3));
					if(dist < miniDist){
						miniDist = dist;
						idx = 2;
					}
				}
				else if(k == 3){
					dist = (real - (-1))*(real - (-1)) + (imag - (-1))*(imag - (-1));
					if(dist < miniDist){
						miniDist = dist;
						idx = 3;
					}
				}
			}
			my_converter.i = mapPoint[idx][0];
			bitData = my_converter.ui;
			break;
		case 4:
			mapPoint[0][0] = 10;
			mapPoint[1][0] = 11;
			mapPoint[2][0] = 14;
			mapPoint[3][0] = 15;

			for(int k = 0; k < 4;k++){
				if(k == 0){
					dist = (real - 3)*(real - 3) + (imag - (-3))*(imag - (-3));
					miniDist = dist;
					idx = 0;
				}
				else if(k == 1){
					dist = (real - 3)*(real - 3) + (imag - (-1))*(imag - (-1));
					if(dist < miniDist){
						miniDist = dist;
						idx = 1;
					}
				}
				else if(k == 2){
					dist = (real - 1)*(real - 1) + (imag - (-3))*(imag - (-3));
					if(dist < miniDist){
						miniDist = dist;
						idx = 2;
					}
				}
				else if(k == 3){
					dist = (real - 1)*(real - 1) + (imag - (-1))*(imag - (-1));
					if(dist < miniDist){
						miniDist = dist;
						idx = 3;
					}
				}
			}
			my_converter.i = mapPoint[idx][0];
			bitData = my_converter.ui;
			break;

	}
	return bitData;
}

void QAM16_demodStr(stream_t &instream , stream_t &outstream){
	#pragma HLS INTERFACE mode=axis port=outstream
	#pragma HLS INTERFACE mode=axis port=instream
	#pragma HLS INTERFACE s_axilite port=return

	value_t in_64;
	value_t firstIn;
	value_t out;
	for(int j = 0; j < SampleNUM; j++){
		for(int i = 0;i < QamNUM; i++){
			in_64 = instream.read();
			dout_t temp = 0;
			temp = temp | QAM16_demod(in_64.data);
			temp = temp << 4*i;
			container[j] = container[j] | temp;

			if((j == SampleNUM - 1) && (i == QamNUM - 1)){
				out.last = in_64.last;
				out.keep = in_64.keep;
				out.strb = in_64.strb;
				out.user = in_64.user;
				out.dest = in_64.dest;
				out.id = in_64.id;
			}
			else if(i == 0 && j == 0){
				firstIn.last = in_64.last;
				firstIn.keep = in_64.keep;
				firstIn.strb = in_64.strb;
				firstIn.user = in_64.user;
				firstIn.dest = in_64.dest;
				firstIn.id = in_64.id;
			}
			else{
				out.last = firstIn.last;
				out.keep = firstIn.keep;
				out.strb = firstIn.strb;
				out.user = firstIn.user;
				out.dest = firstIn.dest;
				out.id = firstIn.id;
			}
		}

		out.data = container[j];
		outstream.write(out);
	}

     return;
}
