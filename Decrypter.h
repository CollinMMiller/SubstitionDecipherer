//
// Created by rneptune on 9/18/19.
//

#ifndef MONOALPHABETICALDECYPHERER_DECRYPTER_H
#define MONOALPHABETICALDECYPHERER_DECRYPTER_H

#include <algorithm>
#include <string>
#include "WordBank.h"

class Decrypter {
public:
	char *ALPHABET = const_cast<char *>("abcdefghijklmnopqrstuvwxyz");

	const int MAX_SKIPS = 3;
	const int LIST_SIZE = 10000;

	explicit Decrypter(std::string);
	void decrypt();
	char *getFreq() { return frequencyOrder; }
	void frequencyAnalysis();
	void getKey(std::string *keyPtr, std::string word, char *alphabet);
private:
	std::vector<std::string> split(std::string);
	std::string message;
	std::vector<std::string> cryptWords;
	std::vector<std::string> outputted;
	WordBank wordBank;

	char *bestAlphabet;
	char *frequencyOrder;
	double bestPercent = 0;
	std::vector<std::string> sortByLength(std::vector<std::string>);
	void decryptionLoop(char *alphabet, int layer, int skips);
	std::string decodeFromAlphabet(char *alphabet);

	double getWordPercentage(std::vector<std::string> words);
	int getIndex(char c);

	void addToAlphabet(char *alphabet, std::string encrypt, std::string decrypt);
	void copyValues(char *from, char *to, int size);

	bool isValidAlphabet(char *alphabet);
	std::string removeNonLetters(std::string str);
};


#endif //MONOALPHABETICALDECYPHERER_DECRYPTER_H
