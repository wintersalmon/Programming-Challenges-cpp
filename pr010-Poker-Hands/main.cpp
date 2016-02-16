//
//  main.cpp
//  pr010-Poker-Hands
//
//  Created by Salmon on 2014. 10. 12..
//  Copyright (c) 2014년 Salmon. All rights reserved.
//

#include <iostream>
#include <cmath>

class Card {
public:
    enum NAME{
        SUIT,
        RANK,
        SIZE,
        SUIT_MIN = 1,
        SUIT_MAX = 5,
        RANK_MIN = 1,
        RANK_MAX = 14
    };
protected:
    int data[SIZE];
    bool Set(int,int);
    bool SetSuit(int);
    bool SetRank(int);
public:
    Card();
    ~Card();
    bool SetSuit(char);
    bool SetRank(char);
    int GetSuit();
    int GetRank();
    int Compare(Card&);
};
Card::Card() {
    data[SUIT] = 0;
    data[RANK] = 1;
}
Card::~Card() {
}
bool Card::Set(int type, int value) {
    if(type == SUIT) {
        if(value < SUIT_MIN || value >= SUIT_MAX)
            return false;
        data[SUIT] = value;
    } else if(type == RANK) {
        if(value < RANK_MIN || value >= RANK_MAX)
            return false;
        data[RANK] = value;
    }
    return true;
}
bool Card::SetSuit(char letter) {
    int value = 0;
    switch (letter) {
        case 'C':
            value = 1;
            break;
        case 'D':
            value = 2;
            break;
        case 'H':
            value = 3;
            break;
        case 'S':
            value = 4;
            break;
        default:
            break;
    }
    return Set(SUIT,value);
}
bool Card::SetRank(char letter) {
    int value = 0;
    if (letter>='2' && letter <= '9') {
        value = letter - '2' + 1;
    } else {
        switch (letter) {
            case 'T':
                value = 9;
                break;
            case 'J':
                value = 10;
                break;
            case 'Q':
                value = 11;
                break;
            case 'K':
                value = 12;
                break;
            case 'A':
                value = 13;
                break;
            default:
                break;
        }
    }
    return Set(RANK,value);
}
int Card::GetSuit() {
    return data[SUIT];
}
int Card::GetRank() {
    return data[RANK];
}
int Card::Compare(Card& compCard) {
    if( data[RANK] > compCard.data[RANK]) {
        return 1;
    } else if ( data[RANK] < compCard.data[RANK] ) {
        return -1;
    } else {
        if( data[SUIT] > compCard.data[SUIT]) {
            return 1;
        } else if( data[SUIT] < compCard.data[SUIT]) {
            return -1;
        }
        return 0;
    }
}
class Hand {
protected:
    int maxSize;
    int curSize;
    Card** cards;
public:
    Hand(int);
    ~Hand();
    int Size();
    bool AddCard(Card*);
    Card* GetCard(int);
    void Empty();
    void Sort();
};
Hand::Hand(int size) {
    maxSize = size;
    curSize = 0;
    cards = new Card*[maxSize];
}
Hand::~Hand() {
    delete [] cards;
}
int Hand::Size() {
    return curSize;
}
void Hand::Sort() {
    Card *tempCard;
    for(int src=0; src<curSize-1; ++src) {
        for(int dst=src+1; dst<curSize; ++dst) {
            if( cards[src]->Compare(*cards[dst]) < 0 ) {
                tempCard = cards[dst];
                cards[dst] = cards[src];
                cards[src] = tempCard;
            }
        }
    }
}
bool Hand::AddCard(Card* _card) {
    if( !_card || (curSize >= maxSize) )
        return false;
    cards[curSize++] = _card;
    return true;
}
Card* Hand::GetCard(int idx) {
    if(idx<0 || idx>=maxSize)
        return 0;
    return cards[idx];
}
void Hand::Empty() {
    for(int i=0; i<maxSize; ++i) {
        cards[i] = 0;
    }
    curSize = 0;
}

class HandValueExtractor {
protected:
    int *checkRankField;
    int *checkSuitField;
    int GetValue(int num,int basic=5) {
        return num << (basic*4);
        /*num*(1<<27)*/
    }
    int GetStraightFlushRuleValue(Hand*,int[]);
    int GetFourCardRuleValue(Hand*,int[]);
    int GetFullHouseRuleValue(Hand*,int[]);
    int GetFlushRuleValue(Hand*,int[]);
    int GetStraightRuleValue(Hand*,int[]);
    int GetThreeCardRuleValue(Hand*,int[]);
    int GetTwoPairRuleValue(Hand*,int[]);
    int GetOnePairRuleValue(Hand*,int[]);
    int GetHighCardRuleValue(Hand*,int[]);
    int (HandValueExtractor::**GetValueByRule)(Hand*,int[]);
    int funcPointArrSize;
public:
    HandValueExtractor();
    ~HandValueExtractor();
    int ExtractValue(Hand*);
};
HandValueExtractor::HandValueExtractor() {
    checkRankField = new int[Card::RANK_MAX];
    checkSuitField = new int[Card::SUIT_MAX];
    GetValueByRule = new (int(HandValueExtractor::*[9])(Hand*,int[]));
    GetValueByRule[0] = &HandValueExtractor::GetStraightFlushRuleValue;
    GetValueByRule[1] = &HandValueExtractor::GetFourCardRuleValue;
    GetValueByRule[2] = &HandValueExtractor::GetFullHouseRuleValue;
    GetValueByRule[3] = &HandValueExtractor::GetFlushRuleValue;
    GetValueByRule[4] = &HandValueExtractor::GetStraightRuleValue;
    GetValueByRule[5] = &HandValueExtractor::GetThreeCardRuleValue;
    GetValueByRule[6] = &HandValueExtractor::GetTwoPairRuleValue;
    GetValueByRule[7] = &HandValueExtractor::GetOnePairRuleValue;
    GetValueByRule[8] = &HandValueExtractor::GetHighCardRuleValue;
    funcPointArrSize = 9;
}
HandValueExtractor::~HandValueExtractor() {
    delete [] GetValueByRule;
    delete [] checkRankField;
    delete [] checkSuitField;
}
int HandValueExtractor::ExtractValue(Hand* _hand) {
    for(int i=0; i<Card::RANK_MAX; ++i) {
        checkRankField[i] = 0;
    }
    for(int i=0; i<Card::SUIT_MAX; ++i) {
        checkSuitField[i] = 0;
    }
    for(int i=0,rank,suit; i<_hand->Size(); ++i) {
        rank = _hand->GetCard(i)->GetRank();
        suit = _hand->GetCard(i)->GetSuit();
        checkRankField[rank]++;
        checkSuitField[suit]++;
    }
    int value = 0;
    for(int i=0; i<funcPointArrSize && !value; ++i) {
        value = (this->*GetValueByRule[i])(_hand,checkRankField);
        if(value) {
            value += GetValue(8-i);
        }
    }
    return value;
}
int HandValueExtractor::GetStraightFlushRuleValue(Hand* _hand, int arr[]) {
    int value = 0;
    if( GetStraightRuleValue(_hand,arr) && GetFlushRuleValue(_hand,arr)) {
        value += GetStraightRuleValue(_hand,arr);
    }
    return value;
}
int HandValueExtractor::GetFourCardRuleValue(Hand* _hand, int arr[]) {
    int value = 0, fourCardRank = 0;
    for(int i=Card::RANK_MIN; i<Card::RANK_MAX; ++i) {
        if(arr[i] == 4)
            fourCardRank = i;
    }
    if( fourCardRank ) {
        value += fourCardRank;
    }
    return value;
}
int HandValueExtractor::GetFullHouseRuleValue(Hand* _hand, int arr[]) {
    int value = 0, threeCardRank=0, twoCardRank=0;
    for(int i=Card::RANK_MIN; i<Card::RANK_MAX; ++i) {
        if(arr[i] == 3) {
            threeCardRank = i;
        } else if(arr[i] == 2) {
            twoCardRank = i;
        }
    }
    
    if( threeCardRank && twoCardRank) {
        value += threeCardRank;
    }
    return value;
}
int HandValueExtractor::GetFlushRuleValue(Hand* _hand, int arr[]) {
    int value = 0;
    int count=0, curSuit=0, prevSuit = _hand->GetCard(0)->GetSuit();
    for(int i=1; i<_hand->Size(); ++i) {
        curSuit = _hand->GetCard(i)->GetSuit();
        if(curSuit == prevSuit) count++;
        else count = 0;
    }
    if(count == 4) {
        value += GetHighCardRuleValue(_hand,arr);
    }
    return value;
}
int HandValueExtractor::GetStraightRuleValue(Hand* _hand, int arr[]) {
    int value=0, straightRank=0, count=0;
    for(int i=Card::RANK_MIN; i<Card::RANK_MAX; ++i) {
        if( arr[i-1]==1 && arr[i]==1 ) {
            ++count;
            if(count ==4) {
                straightRank = i;
            } else if(count == 5) {
                straightRank = i;
                --count;
            }
        } else {
            count = 0;
        }
    }
    if( straightRank) {
        value += straightRank;
    }
    return value;
}
int HandValueExtractor::GetThreeCardRuleValue(Hand* _hand, int arr[]) {
    int value = 0, threeCardRank=0;
    for(int i=Card::RANK_MIN; i<Card::RANK_MAX; ++i) {
        if( arr[i] == 3) {
            threeCardRank = i;
        }
    }
    
    if( threeCardRank) {
        value += threeCardRank;
    }
    return value;
}
int HandValueExtractor::GetTwoPairRuleValue(Hand* _hand, int arr[]) {
    int value = 0, twoPairRank[2]={0,}, higestRankThatIsntPair=0;
    for(int i=Card::RANK_MIN; i<Card::RANK_MAX; ++i) {
        if( arr[i] == 2) {
            if(!twoPairRank[0]) {
                twoPairRank[0] = i;
            } else if( !twoPairRank[1]) {
                twoPairRank[1] = i;
            } else if(twoPairRank[0] && twoPairRank[1]) {
                twoPairRank[0] = twoPairRank[1];
                twoPairRank[1] = i;
            }
        } else if( arr[i] == 1) {
            higestRankThatIsntPair = i;
        }
    }
    if( twoPairRank[0] && twoPairRank[1] ) {
        value += GetValue(twoPairRank[1],2);
        value += GetValue(twoPairRank[0],1);
        value += GetValue(higestRankThatIsntPair,0);
    }
    return value;
}
int HandValueExtractor::GetOnePairRuleValue(Hand* _hand,int arr[]) {
    int value = 0, pairRank=0, highestRankThatIsntPair[3] = {0,};
    for(int i=Card::RANK_MIN,c=0; i<Card::RANK_MAX; ++i) {
        if( arr[i] == 2) {
            pairRank = i;
        } else if( arr[i] == 1) {
            if( c<3 ) {
                highestRankThatIsntPair[c++] = i;
            } else {
                highestRankThatIsntPair[0] = highestRankThatIsntPair[1];
                highestRankThatIsntPair[1] = highestRankThatIsntPair[2];
                highestRankThatIsntPair[2] = i;
            }
        }
    }
    if( pairRank && highestRankThatIsntPair[2] ) {
        value += GetValue(pairRank,3);
        for(int i=2; i>=0; --i) {
            value += GetValue(highestRankThatIsntPair[i],i);
        }
    }
    return value;
}
int HandValueExtractor::GetHighCardRuleValue(Hand* _hand,int arr[]) {
    int value = 0, handSize;
    if(_hand->Size() >= 5) handSize = 5;
    else handSize = _hand->Size();
    for(int i=0, cardRank; i<handSize; ++i) {
        cardRank = _hand->GetCard(i)->GetRank();
        value += GetValue(cardRank,handSize-i-1);
    }
    return value;
}

int main(int argc, const char * argv[]) {
    const int NumOfHand = 2;
    const int NumOfCardPerPlayer = 5;
    HandValueExtractor *extractor = new HandValueExtractor();
    Hand* _hand[NumOfHand] ={0,};
    Card* _card[NumOfHand*NumOfCardPerPlayer] = {0,};
    int handValue[NumOfHand];
    for(int i=0; i<NumOfHand; ++i) {
        _hand[i] = new Hand(NumOfCardPerPlayer);
    }
    for(int i=0; i<NumOfHand*NumOfCardPerPlayer; ++i) {
        _card[i] = new Card();
    }
    for(int h=0; h<NumOfHand; ++h) {
        for(int c=0, index=0; c<NumOfCardPerPlayer; ++c) {
            index = NumOfCardPerPlayer*h + c;
            _hand[h]->AddCard(_card[index]);
        }
    }
    char line[60];
    while( std::cin.getline(line, 60, '\n') ) {
        for(int i=0; i<10; ++i) {
            char rank = line[i*3];
            char suit = line[1+i*3];
            _card[i]->SetRank(rank);
            _card[i]->SetSuit(suit);
        }
        for(int i=0; i<NumOfHand; ++i) {
            _hand[i]->Sort();
        }
        
        for(int i=0; i<NumOfHand; ++i) {
            handValue[i] = extractor->ExtractValue(_hand[i]);
        }
        int result = handValue[0] - handValue[1];
        if(result > 0)
            std::cout << "Black wins.\n";
        else if(result < 0)
            std::cout << "White wins.\n";
        else
            std::cout << "Tie.\n";
    }
    
    for(int i=0; i<2; ++i) {
        delete _hand[i];
    }
    for(int i=0; i<10; ++i) {
        delete _card[i];
    }
    delete extractor;
    
    return 0;
}
