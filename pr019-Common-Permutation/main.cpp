//
//  main.cpp
//  pr019-Common-Permutation
//
//  Created by Salmon on 2014. 12. 10..
//  Copyright (c) 2014년 SalmonJoon. All rights reserved.
//

#include <iostream>
#include <string>

namespace PR019 {
    class Permutation {
    protected:
        int maxElementListSize;
        int elementCount;
        int *elementList;
    public:
        Permutation(int);
        ~Permutation();
        void Empty();
        int Size();
        int GetMaxElementListSize();
        int GetElement(int);
        bool IncreaseElement(int);
        bool DecreaseElement(int);
        bool SetElementCount(int,int);
    };
    Permutation::Permutation(int _size) {
        if(_size<0) _size=1;
        maxElementListSize = _size;
        elementList = new int[maxElementListSize];
        Empty();
    }
    Permutation::~Permutation() {
        delete elementList;
    }
    void Permutation::Empty() {
        for(int i=0; i<maxElementListSize; ++i)
            elementList[i] = 0;
        elementCount = 0;
    }
    int Permutation::Size() {
        return elementCount;
    }
    int Permutation::GetMaxElementListSize() {
        return maxElementListSize;
    }
    int Permutation::GetElement(int index) {
        if(index<0 || index>=maxElementListSize)
            return 0;
        return elementList[index];
    }
    bool Permutation::IncreaseElement(int index) {
        if(index<0 || index>=maxElementListSize)
            return false;
        ++elementList[index];
        ++elementCount;
        return true;
    }
    bool Permutation::DecreaseElement(int index) {
        if(index<0 || index>=maxElementListSize)
            return false;
        --elementList[index];
        --elementCount;
        return true;
    }
    bool Permutation::SetElementCount(int index, int newCount) {
        if(index<0 || index>=maxElementListSize)
            return false;
        if(newCount<0)
            return false;
        int prevCount = elementList[index];
        elementList[index] = newCount;
        elementCount = elementCount - prevCount + newCount;
        return true;
    }
    
    const int ASCII_CODE_MAX_SIZE = 256;
    
    class stringPermutation : public Permutation {
    protected:
    public:
        stringPermutation();
        ~stringPermutation();
        int ConvertStringToElement(std::string*);
        int ConverElementToString(std::string*);
        void PrintElementToString();
    };
    stringPermutation::stringPermutation() : Permutation(ASCII_CODE_MAX_SIZE) {
    }
    stringPermutation::~stringPermutation() {
    }
    int stringPermutation::ConvertStringToElement(std::string *_str) {
        Empty();
        int sourceDataSize = (int)_str->size();
        const char *sourceData = _str->c_str();
        for(int i=0, elementNumber; i<sourceDataSize; ++i) {
            elementNumber = sourceData[i];
            IncreaseElement(elementNumber);
        }
        return Size();
    }
    int stringPermutation::ConverElementToString(std::string *_str) {
        if(elementCount>0) {
            char *elementArr = new char[elementCount];
            for(int elementNumber=0,elementStringSize=0; elementNumber < maxElementListSize; ++elementNumber) {
                if( GetElement(elementNumber) > 0) {
                    for(int i=0; i<elementList[elementNumber]; ++i) {
                        elementArr[elementStringSize] = (char) elementNumber;
                        ++elementStringSize;
                    }
                }
            }
            _str->assign(elementArr);
            delete elementArr;
            return (int)_str->size();
        } else {
            return 0;
        }
    }
    void stringPermutation::PrintElementToString() {
        for(int elementNum=0; elementNum<maxElementListSize; ++elementNum) {
            if(elementList[elementNum]) {
                for(int i=0; i<elementList[elementNum]; ++i)
                    std::cout << (char) elementNum;
            }
        }
    }
    
    class MostCommonPermutation{
    protected:
    public:
        MostCommonPermutation();
        ~MostCommonPermutation();
        int Create(PR019::Permutation*,PR019::Permutation*,PR019::Permutation*);
    };
    MostCommonPermutation::MostCommonPermutation(){
    }
    MostCommonPermutation::~MostCommonPermutation(){
    }
    int MostCommonPermutation::Create(PR019::Permutation *permResult, PR019::Permutation *permOne, PR019::Permutation *permTwo) {
        if(permOne->GetMaxElementListSize() != permTwo->GetMaxElementListSize())
            return 0;
        permResult->Empty();
        int maxElementSize = permOne->GetMaxElementListSize();
        for(int elementNumber=0; elementNumber<maxElementSize; elementNumber++) {
            int elementCount = permOne->GetElement(elementNumber) < permTwo->GetElement(elementNumber) ? permOne->GetElement(elementNumber) : permTwo->GetElement(elementNumber);
            if( elementCount ) {
                permResult->IncreaseElement(elementNumber);
                //permResult->SetElementCount(elementNumber, elementCount);
            }
        }
        return permResult->Size();
    }
}

int main(int argc, const char * argv[]) {
    const int MaxLineSize = 1000;
    int caseCount=0;
    char inputLine[2][MaxLineSize];
    std::string inputStr[2], outputStr;
    PR019::stringPermutation inputPerm[2];
    PR019::stringPermutation outputPerm;
    PR019::MostCommonPermutation mostCommonPerm;
    
    while(true) {
        std::cin.getline(inputLine[0], MaxLineSize, '\n');
        if(inputLine[0][0] == 0)
            break;
        std::cin.getline(inputLine[1], MaxLineSize, '\n');
        
        for(int i=0; i<2; ++i) {
            inputStr[i].assign(inputLine[i]);
            inputPerm[i].ConvertStringToElement(&inputStr[i]);
        }
        
        mostCommonPerm.Create(&outputPerm,&inputPerm[0],&inputPerm[1]);
        //outputPerm.ConverElementToString(&outputStr);
        if(caseCount++>0)
            std::cout << std::endl;
        //std::cout << outputStr;
        outputPerm.PrintElementToString();
    }
    return 0;
}
