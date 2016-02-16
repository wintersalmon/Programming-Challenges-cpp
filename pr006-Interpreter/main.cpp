//
//  main.cpp
//  pr006-Interpreter
//
//  Created by Salmon on 2014. 10. 4..
//  Copyright (c) 2014년 Salmon. All rights reserved.
//

#include <iostream>
#include <stdio.h>

class Interpreter {
protected:
    char **REG;
    char **RAM;
    int REG_MAX, RAM_MAX;
    int commandPointer;
    int runCounter;
    bool (Interpreter::**comFunc)(int,int);
protected:
    void CheckOverFlow(int& num);
    int CharToInt(char c);
    int WordToInt(char* data);
    void Set(char* data, int num);
    void Inc(char* data, int num);
    void Mul(char* data, int num);
protected:
    bool TerminateProgram(int,int);
    
    bool SetRegWithNum(int,int);
    bool IncRegWithNum(int,int);
    bool MulRegWithNum(int,int);
    
    bool SetRegWithReg(int,int);
    bool IncRegWithReg(int,int);
    bool MulRegWithReg(int,int);
    
    bool SaveRegToRam(int,int);
    bool LoadRamToReg(int,int);
    bool JumpIf(int,int);
    
public:
    Interpreter();
    ~Interpreter();
    void Reset();
    int Load();
    int Run();
};
Interpreter::Interpreter() {
    REG_MAX  = 10;
    RAM_MAX  = 1000;
    REG = new char*[REG_MAX];
    for(int i=0; i<REG_MAX; i++) {
        REG[i] = new char[3];
    }
    RAM = new char*[RAM_MAX];
    for(int i=0; i<RAM_MAX; i++) {
        RAM[i] = new char[3];
    }
    comFunc = new (bool(Interpreter::*[10])(int,int));
    comFunc[1] = &Interpreter::TerminateProgram;
    comFunc[2] = &Interpreter::SetRegWithNum;
    comFunc[3] = &Interpreter::IncRegWithNum;
    comFunc[4] = &Interpreter::MulRegWithNum;
    comFunc[5] = &Interpreter::SetRegWithReg;
    comFunc[6] = &Interpreter::IncRegWithReg;
    comFunc[7] = &Interpreter::MulRegWithReg;
    comFunc[8] = &Interpreter::LoadRamToReg;
    comFunc[9] = &Interpreter::SaveRegToRam;
    comFunc[0] = &Interpreter::JumpIf;
}
Interpreter::~Interpreter() {
    delete [] comFunc;
    for(int i=0; i<REG_MAX; i++)
        delete [] REG[i];
    delete [] REG;
    for(int i=0; i<RAM_MAX; i++)
        delete [] RAM[i];
    delete [] RAM;
}
void Interpreter::Reset() {
    for(int i=0; i<REG_MAX; ++i) {
        for(int j=0; j<3; ++j) {
            REG[i][j] = '0';
        }
    }
    for(int i=0; i<RAM_MAX; ++i) {
        for(int j=0; j<3; ++j) {
            RAM[i][j] = '0';
        }
    }
    commandPointer = 0;
    runCounter = 0;
}
int Interpreter::Load() {
    int inputCount = 0;
    char input[100];
    while( std::cin.getline(input, 100) && (input[0]!=0) ) {
        for(int i=0; i<3; ++i) {
            RAM[inputCount][i] = input[i];
        }
        ++inputCount;
    }
    return inputCount;
}
int Interpreter::Run() {
    bool bRun = true;
    int command, arg1, arg2;
    while(bRun) {
        command = CharToInt(RAM[commandPointer][0]);
        arg1 = CharToInt(RAM[commandPointer][1]);
        arg2 = CharToInt(RAM[commandPointer][2]);
        bRun = (this->*comFunc[command])(arg1,arg2);
        ++runCounter;
    }
    return runCounter;
}
void Interpreter::CheckOverFlow(int& num) {
    if(num>=1000) num %= 1000;
}
int Interpreter::CharToInt(char c) {
    if(c>= '1' && c<= '9') {
        return (int) ( c - '1' + 1);
    } else {
        return 0;
    }
}
int Interpreter::WordToInt(char* data) {
    int num = 0;
    num = num + CharToInt( data[2] );
    num = num + CharToInt( data[1] ) * 10;
    num = num + CharToInt( data[0] ) * 100;
    return num;
}
void Interpreter::Set(char* data, int num) {
    CheckOverFlow(num);
    for(int i=2; i>=0; i--) {
        int value = num % 10;
        if( value )
            data[i] = '1' + value - 1;
        else
            data[i] = '0';
        num = num / 10;
    }
}
void Interpreter::Inc(char* data, int num) {
    num = num + WordToInt(data);
    Set(data,num);
}
void Interpreter::Mul(char* data, int num) {
    num = num * WordToInt(data);
    Set(data,num);
}
bool Interpreter::TerminateProgram(int dummy1, int dummy2) {
    commandPointer++;
    if( dummy1 == 0 && dummy2 == 0)
        return false;
    else
        return true;
}
bool Interpreter::SetRegWithNum(int regAdress, int num) {
    commandPointer++;
    Set(REG[regAdress], num);
    return true;
}
bool Interpreter::IncRegWithNum(int regAdress, int num) {
    commandPointer++;
    Inc(REG[regAdress], num);
    return true;
}
bool Interpreter::MulRegWithNum(int regAdress, int num) {
    commandPointer++;
    Mul(REG[regAdress], num);
    return true;
}

bool Interpreter::SetRegWithReg(int dstAdress,int srcAdress) {
    commandPointer++;
    int value = WordToInt(REG[srcAdress]);
    Set(REG[dstAdress], value);
    return true;
}
bool Interpreter::IncRegWithReg(int dstAdress, int srcAdress) {
    commandPointer++;
    int value = WordToInt(REG[srcAdress]);
    Inc(REG[dstAdress], value);
    return true;
}
bool Interpreter::MulRegWithReg(int dstAdress, int srcAdress) {
    commandPointer++;
    int value = WordToInt(REG[srcAdress]);
    Mul(REG[dstAdress], value);
    return true;
}

bool Interpreter::LoadRamToReg(int dstAdress, int adress) {
    commandPointer++;
    int ramAdress = WordToInt(REG[adress]);
    int value = WordToInt(RAM[ramAdress]);
    Set(REG[dstAdress], value);
    return true;
}
bool Interpreter::SaveRegToRam(int srcAdress, int adress) {
    commandPointer++;
    int ramAdress = WordToInt(REG[adress]);
    int value = WordToInt(REG[srcAdress]);
    Set(RAM[ramAdress], value);
    return true;
}
bool Interpreter::JumpIf(int dstAdress, int srcAdress) {
    int cmpValue = WordToInt(REG[srcAdress]);
    if( cmpValue ) {
        commandPointer = WordToInt(REG[dstAdress]);
    } else {
        commandPointer++;
    }
    return true;
}

int main()
{
    int numOfInputCase = 0, result = 0;
    char line[256];
    Interpreter *interpreter = new Interpreter();
    
    std::cin >> numOfInputCase;
    std::cin.getline(line,256);
    std::cin.getline(line,256);
    for(int i=0; i<numOfInputCase; i++) {
        //std::cin.get(c);
        interpreter->Reset();
        interpreter->Load();
        result = interpreter->Run();
        if(i>0) std::cout << std::endl;
        std::cout << result << std::endl;
    }
    return 0;
}