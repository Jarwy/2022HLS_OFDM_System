#include "QAM16_d.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
	din_t data[SampleNUM][QamNUM];
	int goldenData[SampleNUM];
	int retval = 0;
	int ct = 0;
	int temp = 0;
	stream_t inStre,outStre;

	for(int i = 0;i<SampleNUM;i++){
		if(i > 4){
			goldenData[i] = 428;
		}
		else{
			temp += i;
			goldenData[i] = 413+temp;
		}
		cout<<"golden data: "<<goldenData[i]<<endl;
	}

	for(int j = 0; j < SampleNUM; j++){
		for(int i = 0;i < QamNUM; i++){
			float real,imag;
			din_t temp = 0;
			if(j == 0){
				if(i == 0){
					real = 1.8;
					imag = 0;
				}
				else if(i == 1){
					real = 2.4;
					imag = 1.7;
				}
				else if(i == 2){
					real = -2.3;
					imag = 0.7;
				}
				else{
					real = -7.1;
					imag = 5.2;
				}
				my_converter.f = real;
				temp = my_converter.ui;
				my_converter.f = imag;
				temp = (temp << 32) | my_converter.ui;
				data[j][i] = temp;
			}
			else if(j == 1){
				if(i == 0){
					real = 1.6;
					imag = -2.3;
				}
				else if(i == 1){
					real = 2.4;
					imag = 1.3;
				}
				else if(i == 2){
					real = -2.1;
					imag = 1.2;
				}
				else{
					real = -3.5;
					imag = 3.1;
				}
				my_converter.f = real;
				temp = my_converter.ui;
				my_converter.f = imag;
				temp = (temp << 32) | my_converter.ui;
				data[j][i] = temp;
			}
			else if(j == 2){
				if(i == 0){
					real = -6;
					imag = 4;
				}
				else if(i == 1){
					real = 4.6;
					imag = -7.2;
				}
				else if(i == 2){
					real = -5.2;
					imag = 1.23;
				}
				else{
					real = -8.1;
					imag = 9.2;
				}
				my_converter.f = real;
				temp = my_converter.ui;
				my_converter.f = imag;
				temp = (temp << 32) | my_converter.ui;
				data[j][i] = temp;
			}
			else if(j == 3){
				if(i == 0){
					real = -5.1;
					imag = -0.101;
				}
				else if(i == 1){
					real = 9.2;
					imag = -4.4;
				}
				else if(i == 2){
					real = -2.7;
					imag = 1.8;
				}
				else{
					real = -15.66;
					imag = 8.787;
				}
				my_converter.f = real;
				temp = my_converter.ui;
				my_converter.f = imag;
				temp = (temp << 32) | my_converter.ui;
				data[j][i] = temp;
			}
			else if(j == 4){
				if(i == 0){
					real = -0.21;
					imag = 0;
				}
				else if(i == 1){
					real = 2.52;
					imag = -6.1;
				}
				else if(i == 2){
					real = -4.8;
					imag = 0.6;
				}
				else{
					real = -34.334;
					imag = 23;
				}
				my_converter.f = real;
				temp = my_converter.ui;
				my_converter.f = imag;
				temp = (temp << 32) | my_converter.ui;
				data[j][i] = temp;
			}
			else{
				if(i == 0){
					real = 1.0;
					imag = 3.0;
				}
				else if(i == 1){
					real = 3.0;
					imag = -3.0;
				}
				else if(i == 2){
					real = -3;
					imag = 1;
				}
				else{
					real = -3;
					imag = 3;
				}
				my_converter.f = real;
				temp = my_converter.ui;
				my_converter.f = imag;
				temp = (temp << 32) | my_converter.ui;
				data[j][i] = temp;
			}
		}
	}

	for(int j = 0; j < SampleNUM; j++){
		for(int i = 0; i < QamNUM; i++){
			value_t valIn;
					valIn.data = data[j][i];
					valIn.keep = 1; valIn.strb = 1; valIn.user = 1;valIn.id = 0; valIn.dest = 0;
					inStre << valIn;
		}
	}

	QAM16_demodStr(inStre, outStre);

	for(int j = 0; j < SampleNUM; j++){
		value_t valOut;
		valOut = outStre.read();
		cout<< "output:"<<valOut.data<<endl;
		cout<<"bits:"<<bitset<64>(valOut.data)<<endl;
		retval += valOut.data - goldenData[j];
	}


	if (retval == 0) {
		printf("pass!!!");
	}

	else {
		printf("err!");
	}

	return retval;
}
