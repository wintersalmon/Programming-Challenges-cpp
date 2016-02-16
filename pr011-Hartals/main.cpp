//
//  main.cpp
//  pr011-Hartals
//
//  Created by Salmon on 2014. 10. 14..
//  Copyright (c) 2014년 SalmonJoon. All rights reserved.
//
#include <iostream>

class BitField {
protected:
    int blockSize;
    int blockCount;
    int maxSize;
    int curSize;
    int *field;
    bool IndexInBound(int index) {
        if(index<0 || index>=curSize)
            return false;
        return true;
    }
    void CreateNewField(int size) {
        maxSize = size;
        blockCount = maxSize / blockSize;
        if((maxSize % blockSize) > 0)
            blockCount++;
        field = new int[blockCount];
        curSize = maxSize;
        Clear();
    }
public:
    BitField(int);
    ~BitField();
    void Set(int,bool);
    void SetByMultipleNumber(int,bool);
    void SetByArithmeticSeries(int,int,bool);
    bool Get(int);
    int GetCount();
    int Size();
    void AdjustMaxSize(int);
    void Clear();
    void And(BitField*);
    void Or(BitField*);
};
BitField::BitField(int size) {
    blockSize = sizeof(int);
    CreateNewField(size);
}
BitField::~BitField() {
    delete [] field;
}
void BitField::Set(int index, bool value) {
    if( IndexInBound(index) == false) return;
    int blockNumber = index / blockSize;
    int blockIndex = index % blockSize;
    int saveValue = 1 << blockIndex;
    if(value)
        field[blockNumber] = field[blockNumber] | saveValue;
    else
        field[blockNumber] = field[blockNumber] & (~saveValue);
}
void BitField::SetByMultipleNumber(int multipleNumber,bool value) {
    for(int i=0; i<curSize; ++i) {
        if( ( (i+1)%multipleNumber) == 0) {
            Set(i,value);
        }
    }
}
void BitField::SetByArithmeticSeries(int startNumber, int increaseNumber, bool value) {
    for(int n=0; n<curSize; ++n) {
        if( n%increaseNumber == startNumber) {
            Set(n,value);
        }
    }
}
bool BitField::Get(int index) {
    if( IndexInBound(index) == false)
        return false;
    int blockNumber = index / blockSize;
    int blockIndex = index % blockSize;
    int rtnValue = field[blockNumber];
    if( rtnValue & (1<<blockIndex) )
        return true;
    else
        return false;
}
int BitField::GetCount() {
    int count = 0;
    for(int index=0,blockNumber=0,blockIndex=0; index<curSize; ++index, ++blockIndex) {
        if(blockIndex==blockSize) {
            blockIndex = 0;
            blockNumber++;
        }
        if( field[blockNumber] & (1<<blockIndex)) {
            count++;
        }
    }
    return count;
}
int BitField::Size() {
    return curSize;
}
void BitField::AdjustMaxSize(int size) {
    if(size > maxSize) {
        delete field;
        CreateNewField(size);
    } else {
        curSize = size;
        blockCount = curSize / blockSize;
        if((curSize % blockSize) > 0)
            blockCount++;
        Clear();
    }
}
void BitField::Clear() {
    for(int block=0; block<blockCount; ++block)
        field[block] = field[block]^field[block];
}
void BitField::And(BitField* _bitField) {
    for(int block=0; block<blockCount && block<_bitField->blockCount; ++block) {
        field[block] = field[block] & _bitField->field[block];
    }
}
void BitField::Or(BitField* _bitField) {
    for(int block=0; block<blockCount && block<_bitField->blockCount; ++block) {
        field[block] = field[block] | _bitField->field[block];
    }
}

#define DAY_SIZE_MAX 3650
#define HARTAL_SIZE_MAX 100

int main(int argc, const char * argv[]) {
    int testCount = 0;
    int daySize = 0;
    int hartalSize;
    BitField *hartalField[HARTAL_SIZE_MAX];
    BitField *notHartalField = new BitField(DAY_SIZE_MAX);
    notHartalField->SetByArithmeticSeries(0, 1, true);
    notHartalField->SetByArithmeticSeries(5, 7, false);
    notHartalField->SetByArithmeticSeries(6, 7, false);
    for(int i=0; i<HARTAL_SIZE_MAX; ++i)
        hartalField[i] = new BitField(DAY_SIZE_MAX);
    
    std::cin >> testCount;
    while( testCount-- ) {
        std::cin >> daySize;
        std::cin >> hartalSize;
        for(int i=0,num; i<hartalSize; ++i) {
            std::cin >> num;
            hartalField[i]->AdjustMaxSize(daySize);
            hartalField[i]->SetByMultipleNumber(num, true);
        }
        for(int i=1; i<hartalSize; ++i) {
            hartalField[0]->Or(hartalField[i]);
        }
        hartalField[0]->And(notHartalField);
        std::cout << hartalField[0]->GetCount() << std::endl;
    }
    return 0;
}
