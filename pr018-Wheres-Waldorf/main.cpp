//
//  main.cpp
//  pr018-Wheres-Waldorf
//
//  Created by Salmon on 2014. 12. 9..
//  Copyright (c) 2014년 SalmonJoon. All rights reserved.
//
#include <iostream>
#include <string>

namespace WheresWaldorf {
    class MyString : protected std::string {
    protected:
    public:
        MyString();
        ~MyString();
        char Get(int);
        int Size();
        int Set(std::string*);
        void TestPrint();
    };
    MyString::MyString() { }
    MyString::~MyString() { }
    char MyString::Get(int idx) {
        if(idx<0 || idx>= Size() )
            return 0;
        return this->c_str()[idx];
    }
    int MyString::Size() {
        return (int) this->size();
    }
    int MyString::Set(std::string* _str) {
        if(_str == 0) return 0;
        this->assign(*_str);
        return Size();
    }
    void MyString::TestPrint() {
        std::cout << *this;
    }
    
    const int dirMoveInfo[8][2] = {
        -1,0, -1,1, 0,1, 1,1, 1,0, 1,-1, 0,-1, -1,-1
    };
    
    class WordGrid {
    protected:
        char **grid;
        int maxRow, maxCol;
        int curRow, curCol;
        bool SearchAllDirection(WheresWaldorf::MyString*,int,int);
        //char isLowerLetter(char);
        int AbsoluteValue(int number) {
            return number<0 ? -number : number;
        }
        bool isSameWord(char c1, char c2) {
            if( c1 == c2 || AbsoluteValue( c1 - c2) == 32)
                return true;
            else
                return false;
        }
    public:
        WordGrid(int,int);
        ~WordGrid();
        int SetGridValue(int,int,char**);
        bool FindWord(WheresWaldorf::MyString*,int&,int&);
        void TestPrint();
    };
    WordGrid::WordGrid(int row, int col) {
        if(row <=0 || col<=0) {
            row = col = 0;
        } else {
            maxRow = row;
            maxCol = col;
        }
        grid = new char*[row];
        for(int r=0; r<maxRow; ++r) {
            grid[r] = new char[col];
        }
    }
    WordGrid::~WordGrid() {
        for(int r=0; r<maxRow; ++r) {
            delete [] grid[r];
        }
        delete [] grid;
    }
    /*
     char WordGrid::isLowerLetter(char letter) {
     if(letter>= 'A' && letter <= 'Z')
     letter = letter + 32;
     return letter;
     }
     */
    int WordGrid::SetGridValue(int row, int col, char** inputGrid) {
        if(row<=0 || row > maxRow || col<=0 || col>maxCol) return 0;
        for(int r=0; r<row; r++) {
            for(int c=0; c<col; c++) {
                grid[r][c] = inputGrid[r][c];
            }
        }
        curRow = row;
        curCol = col;
        return row*col;
    }
    bool WordGrid::FindWord(WheresWaldorf::MyString *keyWord, int &rowPos, int &colPos) {
        if(keyWord == 0)
            return false;
        for(int row=0; row<curRow; ++row) {
            for(int col=0; col<curCol; ++col) {
                if( isSameWord(grid[row][col],keyWord->Get(0)) ) {
                    if( SearchAllDirection(keyWord,row,col) ) {
                        rowPos = row;
                        colPos = col;
                        return true;
                    }
                }
            }
        }
        return false;
    }
    bool WordGrid::SearchAllDirection(WheresWaldorf::MyString* keyWord, int startRowPos, int startColPos) {
        int wordSize = keyWord->Size();
        int wordCount;
        for(int dir=0; dir<8; ++dir) {
            wordCount = 0;
            for(int i=0,row=startRowPos,col=startColPos; i<wordSize; ++i) {
                if(row<0 || col<0 || row>=maxRow || col>= maxCol)
                    break;
                if( isSameWord(grid[row][col],keyWord->Get(i)) ) {
                    wordCount++;
                }
                row += WheresWaldorf::dirMoveInfo[dir][0];
                col += WheresWaldorf::dirMoveInfo[dir][1];
            }
            if(wordCount == wordSize) {
                return true;
            }
        }
        return false;
    }
    void WordGrid::TestPrint() {
        for(int r=0; r<curRow; ++r) {
            for (int c=0; c<curCol; ++c) {
                std::cout << grid[r][c];
            }
            std::cout << std::endl;
        }
    }
}


int main(int argc, const char * argv[]) {
    WheresWaldorf::WordGrid *grid = new WheresWaldorf::WordGrid(50,50);
    WheresWaldorf::MyString **wordArr = new WheresWaldorf::MyString*[20];
    for(int i=0; i<20; ++i) {
        wordArr[i] = new WheresWaldorf::MyString();
    }
    int maxCount = 0;
    std::cin >> maxCount;
    //std::cin.get();
    //std::cin.get();
    for(int count=0; count<maxCount; ++count) {
        int row,col;
        char **str;
        
        std::cin >> row >> col;
        str = new char*[row];
        for(int r=0; r<row; ++r) {
            str[r] = new char[col];
            std::cin >> str[r];
        }
        grid->SetGridValue(row, col, str);
        
        int wordCount=0;
        std::cin >> wordCount;
        //std::cout << wordCount;
        for(int i=0; i<wordCount; ++i) {
            std::string inputWord;
            std::cin >> inputWord;
            //std::cout << inputWord;
            wordArr[i]->Set(&inputWord);
        }
        
        //grid->TestPrint();
        if(count>0) std::cout << std::endl;
        for(int i=0; i<wordCount; ++i) {
            if( grid->FindWord(wordArr[i], row, col) ) {
                //wordArr[i]->TestPrint();
                std::cout << row+1 << " " << col+1 << std::endl;
            }
        }
    }
    return 0;
}
