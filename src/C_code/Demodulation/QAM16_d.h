#ifndef __QAM16_d_H__
#define __QAM16_d_H__

#define QamNUM 16
#define SampleNUM 3584

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <bitset>
#include <ap_fixed.h>

typedef struct _sample {
    int32_t imag;
    int32_t real;
} sample;

typedef unsigned int T;
typedef ap_uint<4> uint_4;
typedef  ap_uint<64> din_t, dout_t;
typedef ap_axiu<64,1,1,1> value_t;
typedef hls::stream<value_t> stream_t;
typedef signed int int32_t;

union
{
  float f;
  int i;
  T ui;
} my_converter;

using uch = unsigned long long;
union Bit{
    struct Bit_dev{
        uch data: 32;
        uch data2: 32;
    } bit;
    uch raw;
};

void QAM16_demodStr(stream_t &instream , stream_t &outstream);
void QAM16_demod(din_t in_bit, int i);

#endif
