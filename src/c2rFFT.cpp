#include <iostream> // std::cerr std::cout
#include <iomanip> // std::setprecision()
#include <cstring> // std::strcmp(char*,char*)
#include "../include/fastFT.h"
using namespace std;

int main(int argc, char* argv[]) {
	if(argc != 5 && argc != 6){
		cerr << "Usage: " << argv[0] << " --N_channel <number of channels> --lengthFT <length of Fourier Transform> [optional: --humanReadable]"  << endl;
		return 1;
	}
	unsigned N_channel = 0;
	unsigned lengthFT = 0;
	bool humanReadable = false;
	for(int arg = 0; arg < argc; ++arg) {
		if(strcmp(argv[arg], "--N_channel") == 0 || strcmp(argv[arg], "-Nc") == 0)
			sscanf(argv[arg + 1], "%d", &N_channel);
		else if(strcmp(argv[arg], "--lengthFT") == 0 || strcmp(argv[arg], "-lFT") == 0)
			sscanf(argv[arg + 1], "%d", &lengthFT);
		else if(strcmp(argv[arg], "--humanReadable") == 0 || strcmp(argv[arg], "-hR") == 0)
			humanReadable = true;
	}
	if(N_channel <= 0 || lengthFT <= 0) {
		cerr << "Error in " << argv[0] << ":\n Invalid command-line arguments" << endl;
		return 1;
	}

	fastFT<std::complex<double>, double> c2rFFT(N_channel, lengthFT);
	unsigned indexData = 0;

	if(humanReadable) { // Print human-readable data
		cout << setprecision(numeric_limits<double>::digits10 + 1);
		while(cin >> c2rFFT.rawInput[indexData]) {
			indexData++;
			if(indexData == N_channel*(lengthFT/2 + 1)) {
				c2rFFT.FFT();
				for(int t = 0; t < lengthFT; ++t) {
					for(int c = 0; c < N_channel; ++c) {
						cout << c2rFFT.rawOutput[c + N_channel*t];
						if(c < N_channel - 1)
							cout << "\t";
						else
							cout << endl;
					}
				}
				indexData = 0;
			}
		}
	} else { // Print binary representation
		unsigned sizeInput = N_channel*(lengthFT/2 + 1);
		unsigned sizeOutput = N_channel*lengthFT;
		while(fread(c2rFFT.rawInput, sizeInput, sizeof(std::complex<double>), stdin)) {
			c2rFFT.FFT();
			fwrite(c2rFFT.rawOutput, sizeOutput, sizeof(double), stdout);
		}
	}

	return 0;
}
