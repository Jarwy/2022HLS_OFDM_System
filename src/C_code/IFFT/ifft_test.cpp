/*
This is traditional 2-radix DIT FFT algorithm implementation.
It is based on conventional 3-loop structure.
INPUT:
	In_R, In_I[]: Real and Imag parts of Complex signal

OUTPUT:
	In_R, In_I[]: Real and Imag parts of Complex signal
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include "ifft_stream.h"
using namespace std;

DTYPE In_R[SIZE], In_I[SIZE];
DTYPE Out_R[SIZE], Out_I[SIZE];
DTYPE WW_R[SIZE], WW_I[SIZE];
DTYPE Gold_R[SIZE], Gold_I[SIZE];

int main()
{
	FILE *fp;
	FILE *fp_r, *fp_i;
	printf("GENERATING INPUTS\n");


	DTYPE rx, ix;
	int idx;
	int i = 0;
	string filename("E:/NTU/110_2/HLS_project/FinalProject/FFT_HLS/out.fft.gold_FFT4096.dat");
	ifstream input_file(filename);

	DTYPE error = 0.0;
	DTYPE maxerror = 0.0;
	while(input_file >> idx >> rx >> ix) {
		//cout << idx << ";		" << rx << ";		" << ix << endl;//**************************************************
		In_R[i] = rx;
		In_I[i] = ix;
		i++;
	}
	//Twiddle factor is calculated here and saved in fft.h to be used in offline.
		double	e = 6.2831853071795864769;
		printf("GENERATING %d TWIDDLE FACTORS\n", SIZE);
		fp_r=fopen("tw_r.h", "w");
		fp_i=fopen("tw_i.h", "w");
		fprintf(fp_r, "const DTYPE W_real[]={");
		fprintf(fp_i, "const DTYPE W_imag[]={");
		for(int i=0; i<SIZE2; i++)
		{
			//COMPLEX W;	// e^(-j 2 pi/ N)
		  double w = e*double(i)/double(SIZE);
		  WW_R[i]=cos(w);
		  WW_I[i]=sin(w);
		  //printf("%4d\t%f\t%f\n",i,WW_R[i],WW_I[i]);
			fprintf(fp_r, "%.20f,",WW_R[i]);
			fprintf(fp_i, "%.20f,",WW_I[i]);
			if(i%16==0)
				{
					fprintf(fp_r, "\n");
					fprintf(fp_i, "\n");
				}
		}
		fprintf(fp_r, "};\n");
		fprintf(fp_i, "};\n");
		fclose(fp_r);
		fclose(fp_i);

	strm_u64 inStream, outStream;
	//Perform FFT
	ap_uint64 in_tmp = 0;
	axiu_64 in64, out64;
	TypeTrans in_R_type, in_I_type, out_R_type, out_I_type;
	for(int s=0; s<SIZE; s++){
		in_R_type.f = In_R[s];
		in_I_type.f = In_I[s];
		//cout<<"In_R:"<<In_R[s]<<"	In_I:"<<In_I[s]<<endl;//*********************************************************
		in_tmp = 0;
		in_tmp = (in_tmp|in_R_type.i)<<32;
		in_tmp = in_tmp|in_I_type.i;
		in64.data = in_tmp;
		in64.keep = 255; in64.strb = 1; in64.user = 1;in64.id = s; in64.dest = 0;
		inStream << in64;
	}


		ifft_streaming(inStream, outStream);

	for(int idx = 0; idx < SIZE; idx++){
		axiu_64 valOut;
		valOut = outStream.read();

		ap_uint32 valOut_R = Segment64(valOut.data, 1);
		ap_uint32 valOut_I = Segment64(valOut.data, 0);

		TypeTrans out_R;
		TypeTrans out_I;
		out_R.i = valOut_R;
		out_I.i = valOut_I;
		Out_R[idx]= out_R.f;
		Out_I[idx]= out_I.f;
	}

	//Print output
	fp=fopen("out.ifft.dat", "w");
	printf("Printing IFFT Output\n");
	for(int i=0; i<SIZE; i++){
		printf("%4d\t%f\t%f\n",i,Out_R[i],Out_I[i]);
		fprintf(fp, "%4d\t%f\t%f\n",i,Out_R[i],Out_I[i]);
	}
	fclose(fp);


	printf ("Comparing against output data \n");
	for(int i=0; i<SIZE; i++){
	  DTYPE newerror = fabs(i-Out_R[i]) + fabs(0-Out_I[i]);
	  //DTYPE newerror = fabs(In_R[i]-Out_R[i]) + fabs(In_I[i]-Out_I[i]);//******************************************************************
	  //cout<<"Out_R:"<<(Out_R[i]/4096.0)<<endl;
	  //cout<<"newerror:"<<newerror<<endl;
	  error += newerror;
	  if(newerror > maxerror) {
	    maxerror = newerror;
	    fprintf(stdout, "Max Err@%d: %f\n", i, maxerror);
	  }
	}

	fprintf(stdout, "Average Err: %f\n",error/SIZE);

	if ((error/SIZE) > .05 || maxerror > 2) { // This is somewhat arbitrary.  Should do proper error analysis.
	  fprintf(stdout, "*******************************************\n");
	  fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
	  fprintf(stdout, "*******************************************\n");
	  if((error/SIZE) > .05){
		  cout<<"Average Err>.05"<<endl;
	  }else{
		  cout<<"Max Err>2"<<endl;
	  }
	  return 1;
	} else {
	  fprintf(stdout, "*******************************************\n");
	  fprintf(stdout, "PASS: The output matches the golden output!\n");
	  fprintf(stdout, "*******************************************\n");
	  return 0;
	}
}
