#include <iostream>
#include <fstream>
#include "WordBank.h"
#include "Decrypter.h"

void testWordBank(std::string key);

int main(int argc, char** argv) {

	std::string crypt = "";
	for (int i = 1; i < argc; i++) {
		if (i != 1)
			crypt += " ";
		crypt += argv[i];
	}

	std::cout << crypt << std::endl;

	Decrypter dc(crypt);
	dc.decrypt();

	return 0;
}