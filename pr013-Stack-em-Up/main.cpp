//
//  main.cpp
//  pr013-Stack-em-Up
//
//  Created by Salmon on 2014. 10. 16..
//  Copyright (c) 2014년 SalmonJoon. All rights reserved.
//

#include <iostream>
#include <sstream>

namespace StackEmUp {
    enum NameTag {
        SRC = 0,
        DST = 1,
        SUIT = 0,
        RANK = 1,
        SUIT_MAX = 4,
        RANK_MAX = 13,
        CARD_MAX = SUIT_MAX * RANK_MAX
    };
    const char suit_str[SUIT_MAX][10] = {
        "Clubs",
        "Diamonds",
        "Hearts",
        "Spades"
    };
    const char rank_str[RANK_MAX][10] = {
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "10",
        "Jack",
        "Queen",
        "King",
        "Ace",
    };
    
    class Card{
    protected:
        int rank;
        int suit;
    public:
        Card(int);
        ~Card();
        int GetRank();
        int GetSuit();
    };
    Card::Card(int num) {
        if(num < 0 || num >= CARD_MAX)
            num = 0;
        rank = num % RANK_MAX;
        suit = num / RANK_MAX;
    }
    Card::~Card() {
    }
    int Card::GetRank() {
        return rank;
    }
    int Card::GetSuit() {
        return suit;
    }
    
    class ShuffleRule {
    protected:
        int curSize,maxSize;
        int **ruleList;
    public:
        ShuffleRule(int);
        ~ShuffleRule();
        void Clear();
        int Size();
        int AddRule(int,int);
        int GetSource(int);
        int GetDestination(int);
    };
    ShuffleRule::ShuffleRule(int _max) {
        maxSize = _max;
        curSize = 0;
        ruleList = new int*[maxSize];
        for(int i=0; i<maxSize; ++i)
            ruleList[i] = new int [2];
        Clear();
    }
    ShuffleRule::~ShuffleRule() {
        for(int i=0; i<maxSize; ++i)
            delete [] ruleList[i];
        delete [] ruleList;
    }
    void ShuffleRule::Clear() {
        curSize = 0;
    }
    int ShuffleRule::Size() {
        return curSize;
    }
    int ShuffleRule::AddRule(int src, int dst) {
        if(curSize >= maxSize)
            return -1;
        ruleList[curSize][SRC] = src;
        ruleList[curSize][DST] = dst;
        ++curSize;
        return 0;
    }
    int ShuffleRule::GetSource(int idx) {
        if(idx<0 || idx>=maxSize)
            return -1;
        return ruleList[idx][SRC];
    }
    int ShuffleRule::GetDestination(int idx) {
        if(idx<0 || idx>=maxSize)
            return -1;
        return ruleList[idx][DST];
    }
    
    class Deck {
    protected:
        int deckSize;
        int *cardList;
    public:
        Deck(int);
        ~Deck();
        void Reset();
        int Size();
        int Get(int);
        int Shuffle(ShuffleRule*);
        void PrintDeck();
    };
    Deck::Deck(int size) {
        deckSize = size;
        cardList = new int[deckSize];
        Reset();
    }
    Deck::~Deck() {
        delete [] cardList;
    }
    void Deck::Reset() {
        for(int i=0; i<deckSize; ++i) {
            cardList[i] = i;
        }
    }
    int Deck::Size() {
        return deckSize;
    }
    int Deck::Get(int idx) {
        if(idx<0 || idx>=deckSize)
            return -1;
        return cardList[idx];
    }
    int Deck::Shuffle(StackEmUp::ShuffleRule* rule) {
        if(rule == 0)
            return -1;
        int srcList[deckSize];
        int ruleSize = rule->Size();
        for(int i=0,srcIdx; i<ruleSize; ++i) {
            srcIdx = rule->GetSource(i);
            srcList[i] = cardList[srcIdx];
        }
        for(int i=0,dstIdx; i<ruleSize; ++i) {
            dstIdx = rule->GetDestination(i);
            cardList[dstIdx] = srcList[i];
        }
        return ruleSize;
    }
    void Deck::PrintDeck() {
        for(int i=0,suit=0,rank=0; i<deckSize; ++i) {
            rank = cardList[i]%StackEmUp::RANK_MAX;
            suit = cardList[i]/StackEmUp::RANK_MAX;
            std::cout << StackEmUp::rank_str[rank] << " of ";
            std::cout << StackEmUp::suit_str[suit] << std::endl;
        }
    }
}

int main(int argc, const char * argv[]) {
    int testCaseCount=0, testCaseMax=0;
    int ruleListSize = 0, ruleListSizeMax = 100;
    char inputLine[1000], c;
    
    StackEmUp::Deck *deck = new StackEmUp::Deck(StackEmUp::CARD_MAX);
    StackEmUp::ShuffleRule **rule = new StackEmUp::ShuffleRule*[ruleListSizeMax];
    for(int i=0; i<ruleListSizeMax; ++i)
        rule[i] = new StackEmUp::ShuffleRule(StackEmUp::CARD_MAX);
    
    std::cin >> testCaseMax;
    while(testCaseCount < testCaseMax) {
        deck->Reset();
        ruleListSize = 0;
        std::cin >> ruleListSize;
        for(int ruleNum=0; ruleNum<ruleListSize; ++ruleNum) {
            for(int idx=0,inputNum=0;idx<StackEmUp::CARD_MAX; ++idx) {
                std::cin >> inputNum;
                if( idx != (inputNum-1) )
                    rule[ruleNum]->AddRule(inputNum-1, idx);
            }
        }
        std::cin.get(c);
        for(bool bShuffle=true; bShuffle; ) {
            std::cin.getline(inputLine, 1000, '\n');
            if( inputLine[0] == '\n' || inputLine[0] == 0) {
                bShuffle = false;
                break;
            }
            int ruleNum;
            std::stringstream(inputLine) >> ruleNum;
            deck->Shuffle(rule[ruleNum-1]);
        }
        if(testCaseCount > 0)
            std::cout << std::endl;
        deck->PrintDeck();
        testCaseCount++;
        for(int i=0; i<ruleListSize; ++i) {
            rule[i]->Clear();
        }
    }
    for(int i=0; i<ruleListSizeMax; ++i) {
        delete rule[i];
    }
    delete [] rule;
    return 0;
}