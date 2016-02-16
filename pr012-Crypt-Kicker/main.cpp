//
//  main.cpp
//  pr012-Crypt-Kicker
//
//  Created by Salmon on 2015. 10. 5..
//  Copyright © 2015년 SalmonJoon. All rights reserved.
//

#include <iostream>
#include <string>
#include <set>

class Word : public std::string {
protected:
    size_t itsPatternSize;
    int * itsPattern;
public:
    Word(std::string _str);
    ~Word();
    bool patternCompare(Word& compWord);
    void T_PrintPattern();
};
Word::Word(std::string _str) {
    assign(_str.c_str());
    itsPatternSize = this->size();
    if (itsPatternSize <= 0)
        itsPattern = nullptr;
    else
        itsPattern = new int[itsPatternSize];
    
    int alphabetMarker[26] = {0,};
    int alphabetCount = 1;
    int patternIdx = 0;
    
    while (patternIdx < itsPatternSize) {
        char curAlphabet = (*this)[patternIdx];
        int curAlphabetIdx = curAlphabet - 'a';
        if (alphabetMarker[curAlphabetIdx] == 0) {
            alphabetMarker[curAlphabetIdx] = alphabetCount++;
        }
        itsPattern[patternIdx++] = alphabetMarker[curAlphabetIdx];
    }
}
Word::~Word() {
    delete [] itsPattern;
}
bool Word::patternCompare(Word &compWord) {
    if (itsPatternSize == compWord.itsPatternSize) {
        for(int i=0; i<itsPatternSize; i++) {
            if( itsPattern[i] != compWord.itsPattern[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}
void Word::T_PrintPattern() {
    for(int i=0; i<itsPatternSize; i++) {
        std::cout << (*this)[i] << " ";
    }
    std::cout << std::endl;
    for(int i=0; i<itsPatternSize; i++) {
        std::cout << itsPattern[i] << " ";
    }
    std::cout << std::endl;
}

typedef std::set<Word> WORD_SET;

class CryptKickerDictionary {
protected:
    WORD_SET wordSet;
public:
    CryptKickerDictionary();
    ~CryptKickerDictionary();
    int RegisterWord(std::string* insertWord);
    int UnregisterWord(std::string* removeWord);
    int DecryptLine(std::string* srcLine, std::string* dstLine);
    int DecryptWord(std::string* word);
    void T_PrintWords();
};

CryptKickerDictionary::CryptKickerDictionary() { }
CryptKickerDictionary::~CryptKickerDictionary() { }
int CryptKickerDictionary::RegisterWord(std::string *insertWord) {
    wordSet.insert(*insertWord);
    return 0;
}
int CryptKickerDictionary::UnregisterWord(std::string *removeWord) {
    wordSet.erase(*removeWord);
    return 0;
}
int CryptKickerDictionary::DecryptLine(std::string *srcLine, std::string *dstLine) {
    dstLine->assign(*srcLine);
    
    char wordBuffer[80];
    WORD_SET srcWordSet;
    for(int i=0, bufferCount = 0; srcLine->c_str()[i] != 0 && i < srcLine->length(); i++) {
        if (srcLine->c_str()[i] == ' ') {
            Word curWord(srcLine->substr(i-bufferCount,i));
            srcWordSet.insert(curWord);
            bufferCount = 0;
            continue;
        }
        wordBuffer[i] = srcLine->c_str()[i];
        bufferCount++;
    }
    
    auto it = srcWordSet.begin();
    while ( it != srcWordSet.end()) {
        std::cout << *it << std::endl;
        it++;
    }
    return 0;
}
int CryptKickerDictionary::DecryptWord(std::string *word) {
    size_t wordSize = word->size();
    int i = 0, idx_start = 0, count = 0;
    
    for (auto pos = wordSet.begin(); pos != wordSet.end(); ++pos) {
        if ( pos->size() == wordSize ) {
            if (count == 0) idx_start = i;
            count++;
        }
        i++;
    }
    //std::cout << wordSize << " : " << idx_start << " + " << count << std::endl;
    return count;
}
void CryptKickerDictionary::T_PrintWords() {
    auto it = wordSet.begin();
    while (it != wordSet.end()) {
        std::cout << *it << std::endl;
        it++;
    }
}

int main(int argc, const char * argv[]) {
    CryptKickerDictionary crypter;
    int inputWordSize;
    std::string inputWord;
    std::cin >> inputWordSize;
    for(int i=0; i<inputWordSize; ++i) {
        std::cin >> inputWord;
        crypter.RegisterWord(&inputWord);
    }
    //crypter.T_PrintWords(); // Check Dictionary
    
    char c;
    std::cin.get(c);
    
    const int MAX_LINE_INPUT = 81;
    std::string inputLine, outputLine;
    char input[MAX_LINE_INPUT];
    while ( std::cin.getline(input, MAX_LINE_INPUT)) {
        inputLine.assign(input);
        if (inputLine.size() == 0) break;
        crypter.DecryptLine(&inputLine, &outputLine);
        std::cout << outputLine << std::endl;
    }
    //std::string word = "Hell";
    //crypter.DecryptWord(&word);
    return 0;
}

/*
 Word w1("kaabca");
 Word w2("kbbdcb");
 Word w3("fffaaagggk");
 Word w4("kbbdca");
 w1.T_PrintPattern();
 w2.T_PrintPattern();
 w3.T_PrintPattern();
 w4.T_PrintPattern();
 
 std::cout << "w1 : w2 = " << w1.patternCompare(w2) << std::endl;
 std::cout << "w3 : w4 = " << w3.patternCompare(w4) << std::endl;
 std::cout << "w1 : w4 = " << w1.patternCompare(w4) << std::endl;
 */

