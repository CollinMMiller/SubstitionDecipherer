//
// Created by rneptune on 9/18/19.
//

#include "WordBank.h"
#include <fstream>

WordBank::WordBank(std::string path, int limit){
	std::ifstream file(path);
	std::string word;
	int count = 0;
	while (file >> word && count <= limit) {
		wordList.push_back(word);
		count++;
	}
}

std::vector<std::string> WordBank::getList(std::string key) {
	int size = (int)key.length();
	const char *keyArray = key.c_str();
	std::vector<std::string> passedStrings;

	for (std::string& word : wordList) {
		if (word.length() != size)
			continue;

		int repeat = -1;
		for (int i = 0; i < key.length(); i++) {
			int charNum = (int)(keyArray[i]);
			if (charNum > 47 && charNum < 58) {
				repeat = charNum - 48 > repeat ?
					repeat = charNum - 48 : repeat;
			}
		}
		//Need to check for repeats in key

		bool passed = true;
		const char *wordArray = word.c_str();
		for (int i = 0; i < size; i++) {
			if ((int)keyArray[i] > 64 && wordArray[i] != keyArray[i]) {
				passed = false;
				goto exitLoops;
			}
		}

		//Check common letters
		for (int i = 0; i <= repeat; i++) {
			char letter = (char)NULL;
			for (int j = 0; j < key.length(); j++) {
				if (keyArray[j] == (char)(i+48)) {
					if (letter == (char)NULL) {
						letter = wordArray[j];
					} else {
						if(wordArray[j] != letter) {
							passed = false;
							goto exitLoops;
						}
					}
				}
			}
		}
exitLoops:
		if (passed)
			passedStrings.push_back(word);
	}

	return passedStrings;
}

std::vector<std::string> WordBank::getList(std::string key, std::vector<std::string> blacklist) {
	std::vector<std::string> list = getList(key);
	for (int i = 0; i < list.size(); i++) {
		for (int j = 0; j < blacklist.size(); j++) {
			if (list[i] == blacklist[j]) {
				list.erase(list.begin() + i);
				i--;
				break;
			}
		}
	}
	return list;
}
