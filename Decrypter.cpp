//
// Created by rneptune on 9/18/19.
//

#include <iostream>
#include <vector>
#include "Decrypter.h"

Decrypter::Decrypter(std::string cipher) :
wordBank("/home/rneptune/MonoAlphabeticalDecipherer/10000-english-usa.txt", LIST_SIZE) {

	for (auto & c: cipher) {
		c = (char) tolower(c);
	}
	message = cipher;
	bestAlphabet = new char[26];
	frequencyOrder = new char[26];
	bestPercent = 0;

	cipher = removeNonLetters(cipher);
	cryptWords = sortByLength(split(cipher));
}

void Decrypter::decrypt() {
	char *alphabet = new char[26];
	for (int i = 0; i < 26; i++)
		alphabet[i] = '-';

	decryptionLoop(alphabet, 0, 0);
}


void Decrypter::decryptionLoop(char *alphabet, int layer, int skips) {
	if (layer == cryptWords.size()) {
		std::string mes = decodeFromAlphabet(alphabet);
		double wordValue = getWordPercentage(split(mes));
		if (wordValue >= bestPercent) {
			if(std::find(outputted.begin(), outputted.end(), mes)
					== outputted.end()) {
				outputted.push_back(mes);
				std::cout << mes << std::endl;
				bestPercent = wordValue;
			}
		}
	} else if (skips < MAX_SKIPS) {
		std::string key;
		getKey(&key, cryptWords[layer], alphabet);
		std::vector<std::string> possibilities = wordBank.getList(key);
//		if (key[key.length() - 1] == 's') {
//			std::string key2 = key.substr(0, key.length() - 1);
//			std::vector<std::string> plurals = wordBank.getList(key2);
//			for (std::string a : plurals) {
//				a += 's';
//			}
//			possibilities.insert(possibilities.begin(), plurals.begin(), plurals.end());
//			sort( possibilities.begin(), possibilities.end() );
//			possibilities.erase( unique( possibilities.begin(), possibilities.end() ),
//				possibilities.end() );
//		}

		char *myAlphabet = new char[26];

		for (auto &word : possibilities) {
			copyValues(alphabet, myAlphabet, 26);
			addToAlphabet(myAlphabet, cryptWords[layer], word);

			if (!isValidAlphabet(myAlphabet))
				continue;

			decryptionLoop(myAlphabet, layer+1, skips);
		}
		copyValues(alphabet, myAlphabet, 26);
		decryptionLoop(myAlphabet, layer+1, skips+1);

		delete[] myAlphabet;
	}
}




void Decrypter::frequencyAnalysis() {
	int count[26] = {0};

	//Go through the message, count the number of each character
	char *messagePtr = const_cast<char *>(message.c_str());
	for (int i = 0; i < message.length(); i++) {
		for (int j = 0; j < 26; j++) {
			if (messagePtr[i] == ALPHABET[j]) {
				count[j]++;
				break;
			}
		}
	}

	for (int i = 0; i < 26; i++) {
		std::cout << count[i] << std::endl;
		frequencyOrder[i] = ALPHABET[i];
	}

	//Bubble sort
	//TODO:More efficient sorting method
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 25 - i; j++) {
			if(count[j] < count[j+1]) {
				int temp = count[j];
				count[j] = count[j+1];
				count[j+1] = temp;

				char tempc = frequencyOrder[j];
				frequencyOrder[j] = frequencyOrder[j+1];
				frequencyOrder[j+1] = tempc;
			}
		}
	}
}

std::vector<std::string> Decrypter::split(std::string s) {
	std::vector<std::string> words;

	std::string delimiter = " ";
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		words.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	words.push_back(s);

	return sortByLength(words);
}

std::vector<std::string> Decrypter::sortByLength(std::vector<std::string> words) {
	//TODO:Faster sorting algorithm
	for (int i = 0; i < words.size() - 1; i++) {
		for (int j = 0; j < words.size() - 1 - i; j++) {
			if(words[j].length() < words[j+1].length()) {
				std::string temp = words[j];
				words[j] = words[j+1];
				words[j+1] = temp;
			}
		}
	}

	return words;
}

void Decrypter::getKey(std::string *keyPtr, std::string word, char *alphabet) {
	std::string key = word;
	char *wordArray = const_cast<char *>(word.c_str());
	for (int i = 0; i < word.length(); i++) {
		if (alphabet[((int)wordArray[i]) - 97] != '-') {
			key[i] = alphabet[((int)wordArray[i]) - 97];
			wordArray[i] = '-';
		} else {
			key[i] = '?';
		}
	}

	//Checking for letters that show up multiple times
	int currentCount = 0;
	for(int i = 0; i < word.length() - 1; i++) {
		if (currentCount > 9)
			break;
		if (wordArray[i] == '-')
			continue;
		char letter = wordArray[i];
		bool repeat = false;
		for (int j = i + 1; j < word.length(); j++) {
			if (wordArray[j] == letter) {
				repeat = true;
				wordArray[j] = '-';
				key[j] = (char)(currentCount + 48);
			}
		}
		if (repeat) {
			key[i] = (char)(currentCount + 48);
			currentCount++;
		}
	}
	*keyPtr = key;
}

std::string Decrypter::decodeFromAlphabet(char *alphabet) {
	std::string decoded = message;
	for (auto& c : decoded) {
		if (c >= 'a' && c <= 'z')
			c = alphabet[getIndex(c)];
	}
	return decoded;
}

double Decrypter::getWordPercentage(std::vector<std::string> words) {
	int count = 0;
	for (const auto &word : words) {
		bool skip = false;
		for(int i = 0; i < word.length(); i++) {
			if (word[i] == '-') {
				skip = true;
				break;
			}
		}
		if (!wordBank.getList(word).empty() && !skip)
			count++;
	}

	return ((double)(count)) / ((double)words.size());
}

void Decrypter::addToAlphabet(char *alphabet, std::string encrypt, std::string decrypt) {
	for (int i = 0; i < encrypt.length(); i++) {
		alphabet[getIndex(encrypt[i])] = decrypt[i];
	}
}


void Decrypter::copyValues(char *from, char *to, int size) {
	for (int i = 0; i < size; i++) {
		to[i] = from[i];
	}
}

bool Decrypter::isValidAlphabet(char *alphabet) {
	for (int i = 0; i < 26 - 1; i++) {
		if (alphabet[i] == '-')
			continue;
		for (int j = i+1; j < 26; j++) {
			if (alphabet[i] == alphabet[j])
				return false;
		}
	}

	return true;
}

std::string Decrypter::removeNonLetters(std::string str) {
	//TODO: Make more efficient
	std::string fixed = "";
	for (int i = 0; i < str.length(); i++) {
		if ((int) str[i] > 96 && (int) str[i] < 123 || str[i] == ' ') {
			fixed += str[i];
		}
	}
	return fixed;
}

int Decrypter::getIndex(char c) {
	return c - 97;
}
