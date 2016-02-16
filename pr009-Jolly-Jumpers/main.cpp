//
//  main.cpp
//  pr009-Jolly-Jumpers
//
//  Created by Salmon on 2014. 10. 11..
//  Copyright (c) 2014년 Salmon. All rights reserved.
//

#include <iostream>

template <typename Data>
class DataField {
protected:
    int fieldSize;
    Data *field;
public:
    DataField(int);
    ~DataField();
    bool Set(int,Data);
    bool Get(int,Data&);
    int Size();
    void Clear();
};
template <typename Data>
DataField<Data>::DataField(int size) {
    fieldSize = size;
    field = new Data[fieldSize];
    Clear();
}
template <typename Data>
DataField<Data>::~DataField() {
    delete [] field;
}
template <typename Data>
bool DataField<Data>::Set(int idx, Data value) {
    if(idx<0 || idx>fieldSize) return false;
    field[idx] = value;
    return true;
}
template <typename Data>
bool DataField<Data>::Get(int idx, Data &d) {
    if(idx<0 || idx>fieldSize) return false;
    d = field[idx];
    return true;
}
template <typename Data>
int DataField<Data>::Size() {
    return fieldSize;
}
template <typename Data>
void DataField<Data>::Clear() {
    for(int i=0; i<fieldSize; ++i)
        field[i] = false;
}

typedef DataField<bool> boolField;
typedef DataField<int> intField;

class CheckJollyJumper {
protected:
    bool bIsJolly;
    int maxCheckSize;
    intField *numbers;
    boolField *checkField;
    int AbsoluteDifference(int n1, int n2) {
        int value = n1 - n2;
        if(value < 0) return value * -1;
        else return value;
    }
public:
    CheckJollyJumper(int);
    ~CheckJollyJumper();
    bool SetNumbers(intField*);
    bool isJullyJumper();
};
CheckJollyJumper::CheckJollyJumper(int size) {
    bIsJolly = false;
    if(size>=0) maxCheckSize = size;
    else maxCheckSize = 0;
    numbers = 0;
    checkField = new boolField(maxCheckSize);
}
CheckJollyJumper::~CheckJollyJumper() {
}
bool CheckJollyJumper::SetNumbers(intField* nums) {
    if( !nums || (nums->Size() <= 0) || (nums->Size() > maxCheckSize ))
        return false;
    
    numbers = nums;
    bIsJolly = false;
    checkField->Clear();
    bool bChecked = false;
    
    if(nums->Size() == 1) {
        bIsJolly = true;
    } else {
        int size = nums->Size();
        int cur,prev,value;
        nums->Get(0, cur);
        for(int i=1; i<size; ++i) {
            prev = cur;
            nums->Get(i, cur);
            value = AbsoluteDifference(prev, cur);
            checkField->Get(value, bChecked);
            if(value<=0 || value >= size || bChecked ) {
                bIsJolly = false;
                return true;
            }
            checkField->Set(value,true);
        }
        bIsJolly = true;
    }
    return true;
}
bool CheckJollyJumper::isJullyJumper() {
    return bIsJolly;
}

int main(int argc, const char * argv[]) {
    bool bJolly;
    intField *numbers;
    CheckJollyJumper *jolleyChecker = new CheckJollyJumper(3000);
    int inputCount;
    while( std::cin >> inputCount) {
        if(inputCount>=1) {
            numbers = new intField(inputCount);
            for(int count=0, num; count < inputCount; ++count) {
                std::cin >> num;
                numbers->Set(count, num);
            }
            jolleyChecker->SetNumbers(numbers);
            bJolly = jolleyChecker->isJullyJumper();
            delete numbers;
            numbers = 0;
        } else {
            bJolly = false;
        }
        if( bJolly ) {
            std::cout << "Jolly\n";
        } else {
            std::cout << "Not jolly\n";
        }
    }
    return 0;
}
