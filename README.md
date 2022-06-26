# OFDM System
## Table of Contents
+ OFDM System
	* [OVERVIEW](#OVERVIEW)
	* [SOFTWARE TOOLS AND SYSTEM REQUIREMENTS](#SOFTWARE TOOLS AND SYSTEM REQUIREMENTS)
	* [DESIGN FILE HIERARCHY](#DESIGN FILE HIERARCHY)
	* [Usage](#Usage)
	* [Result](#Result)
	* [References](#References)
	* [SUPPORT](#SUPPORT)

## OVERVIEW
Build a basic OFDM system and implement it in Xilinx's FPGA PYNQ-z2. The system will get 256 input samples and each of samples is an 64 bits unsigned integer data. We use modulation IP which implements 16 points QAM, iFFT IP and cyclic prefix IP to generate transmitted signal. Also we use AWGN and Rayleigh IP to add noise to the signal. Then the system will decode the received data with CyclicPrefixRemoval IP, FFT IP and demodulation IP.
###OFDM architecture block diagram
![ofdm architecture](https://www.gaussianwaves.com/gaussianwaves/wp-content/uploads/2012/06/Cyclic_prefix_OFDM_architecture.jpg)

## SOFTWARE TOOLS AND SYSTEM REQUIREMENTS
* Vitis HLS 2021.2
* Vivado 2021.2
* Python 3

## DESIGN FILE HIERARCHY
```
|   README.md
	\---Design IP
		\Modulation
			Modulation16.cpp
			Modulation16.h
		\IFFT
			ifft_stream.cpp
			ifft_stream.h
		\CyclicPrefix
			CP_Stream.cpp
			CP_Stream.h
		\AWGN
			AWGN_channel.cpp
			AWGN_channel.h
		\Rayleigh
			Rayleigh_channel.cpp
			Rayleigh_channel.h
		\CyclicRemoval
			rmCP_Stream.cpp
			rmCP_Stream.h
		\FFT
			fft_streaming.cpp
			fft_streaming.h
		\Demodulation
			QAM16_d.cpp
			QAM16_d.h
	\---testbench
			Modulation16_test.cpp
			ifft_test.cpp
			CP_Stream_test.cpp
			AWGN_Tester.cpp
			Rayleigh_Tester.cpp
			rmCP_test.cpp
			fft_test.cpp
			QAM16_d_tb.cpp
```

## Usage
### 1.Generate and export IP package with Vitis HLS
* open Vitis HLS

* create new project

* import the source code of IP you want to implement

* set top function

* import the test bench of IP

* choose C simulation

* choose C synthesis

* choose Cosimulation

* choose Export RTL

### 2.Use command lines to Generate and export IP package
* open Vitis HLS Command Prompt

```
vitis_hls -f tcl_script.tcl
```
This command line will run "script.tcl" script file. All of the Tcl commands used when creating a project in the GUI are written to the solution/script.tcl file within the project. You can use this script as a starting point for developing your own batch scripts. An example script is provided below:
```
open_project Project_name
set_top TopFunction_name
add_files ../Project_name_src/IP_name.cpp
add_files -tb ../Project_name_src/IP_name_testbench.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution1" -flow_target vitis
set_part {xc7z020clg400-1}
create_clock -period 10 -name default
source "./Project_name/solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -format ip_catalog
```
Then you can get IP package file "export.zip" as default in path: "Project_name/solution1/impl"

### Use Vivado to generate Bit-stream file and implement design in FPGA
* open Vivado

* create new project

* import IP package file

* finish block design

* choose HDL Wrapper

* choose Generate Bitstream

* get .bit file and .hwh file
.bit file location:
\Project_name\Project_name.runs\impl_1\design_1_wrapper.bit
.hwh file location: \Project_name\Project_name.gen\sources_1\bd\design_1\hw_handoff\design_1.hwh

* put .bit file and .hwh file in PYNQ-z2 FPGA

* write the host code with python and excute it on jupyter Notebook

* get the result

## Result
### AWGN Channel Implementation (Cont.)
![AWGN](https://imgur.com/KLB88vk.jpg)
### Rayleigh Channel Implementation (Cont.)
![Rayleigh](https://imgur.com/ckgLakX.jpg)
### 16 QAM Constellation Diagram
![QAM_16](https://imgur.com/kHvOBVf.jpg)

## References
* [OFDM and Multi-Channel Communication Systems](https://www.ni.com/zh-tw/innovations/white-papers/06/ofdm-and-multi-channel-communication-systems.html)
* [Introduction to OFDM – orthogonal Frequency division multiplexing – part 4 – Cyclic Prefix](https://www.gaussianwaves.com/2011/07/introduction-to-ofdm-orthogonal-frequency-division-multiplexing-part-4-cyclic-prefix/)
* [GitHub: PYNQ_Composable_Pipeline](https://github.com/Xilinx/PYNQ_Composable_Pipeline)
* [Quadrature Amplitude Modulation](https://zh.wikipedia.org/wiki/%E6%AD%A3%E4%BA%A4%E5%B9%85%E5%BA%A6%E8%B0%83%E5%88%B6)
* [FFT original code]()

## SUPPORT
You can find some solution and suggestion about this project or your own projects on [Xilinx Support](https://support.xilinx.com/s/?language=en_US) website.

