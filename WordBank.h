//
// Created by rneptune on 9/18/19.
//

#ifndef MONOALPHABETICALDECYPHERER_WORDBANK_H
#define MONOALPHABETICALDECYPHERER_WORDBANK_H


#include <vector>
#include <string>

class WordBank {
public:
	WordBank(std::string,int);
	// ? means any one letter, TODO: * means possibly multiple letters
	std::vector<std::string> getList(std::string);
	std::vector<std::string> getList(std::string, std::vector<std::string> blacklist);
private:
	std::vector<std::string> wordList;
};


#endif //MONOALPHABETICALDECYPHERER_WORDBANK_H
