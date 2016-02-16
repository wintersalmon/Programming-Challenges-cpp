//
//  main.cpp
//  pr016-Yahtzee
//
//  Created by Salmon on 2014. 11. 26..
//  Copyright (c) 2014년 SalmonJoon. All rights reserved.
//

#include <iostream>

namespace Yahtzee_namespace {
    int printCount = 0;
    const int MAX_ROUND = 13;
    const int MAX_CATEGORY = 13;
    class NumberList {
    protected:
        int maxSize,curSize;
        int *numbers;
    public:
        NumberList(int);
        ~NumberList();
        int Size();
        void Reset();
        int Insert(int);
        int Remove(int,int&);
        int Get(int);
    };
    NumberList::NumberList(int _maxSize) {
        maxSize = _maxSize;
        numbers = new int[maxSize];
        Reset();
    }
    NumberList::~NumberList() {
        delete [] numbers;
    }
    void NumberList::Reset() {
        for(int i=0; i<maxSize; ++i) {
            numbers[i] = 0;
        }
        curSize = 0;
    }
    int NumberList::Insert(int num) {
        if( curSize == 0) {
            numbers[0] = num;
            return ++curSize;
        } else if(curSize < maxSize) {
            int temp=0, cur=0, prev=num;
            for(int i=0; i<curSize; ++i) {
                cur = numbers[i];
                if(prev<cur) {
                    temp = cur;
                    numbers[i] = prev;
                    prev = temp;
                }
            }
            numbers[curSize] = prev;
            return ++curSize;
        } else {
            return 0;
        }
    }
    int NumberList::Remove(int idx,int &rtnValue) {
        if(idx>=0 && idx<maxSize) {
            rtnValue = numbers[idx];
            for(int i=idx+1; i<curSize; ++i) {
                numbers[i-1] = numbers[i];
            }
            --curSize;
            return 1;
        }
        return 0;
    }
    int NumberList::Get(int idx) {
        if(idx>=0 && idx<maxSize)
            return numbers[idx];
        return -1;
    }
    int NumberList::Size() {
        return curSize;
    }
    
    class YahtzeeCategoryScoreRule {
    protected:
        int AddAllSelectedNumbers(NumberList* list,int number) {
            int total = 0;
            for(int i=0; i<list->Size(); ++i) {
                if(list->Get(i) == number) {
                    total += number;
                }
            }
            return total;
        }
        int AddAllNumbers(NumberList* list) {
            int total = 0;
            for(int i=0; i<list->Size(); ++i) {
                total += list->Get(i);
            }
            return total;
        }
        int HasSameNumbers(NumberList* list, int sameNumbercount) {
            int count = 0, target=list->Get(0), ans=0;
            for(int i=1,cur=0; i<list->Size(); ++i) {
                cur = list->Get(i);
                if(cur == target) {
                    if(count == sameNumbercount-2)
                        ans=target;
                    else
                        count++;
                } else {
                    target = cur;
                    count = 0;
                }
            }
            return ans;
        }
        int HasStreightNumbers(NumberList* list, int streightCount) {
            int total = 0;
            int count = 0, prev = list->Get(0);
            for(int i=1,cur; i<list->Size(); ++i) {
                cur = list->Get(i);
                if(prev + 1 == cur) {
                    count++;
                    if(count==streightCount-1)
                        total =  count;
                }
                else if(prev != cur) {
                    count = 0;
                }
                prev = cur;
            }
            return total;
        }
        int HasFullHoush(NumberList* list) {
            int nums[5];
            for(int i=0; i<5; ++i) {
                nums[i] = list->Get(i);
            }
            if(nums[0]==nums[1] && nums[1]!=nums[2] && nums[2]==nums[3] && nums[3]==nums[4] )
                return 40;
            else if( nums[0]==nums[1] && nums[1]==nums[2] && nums[2]!=nums[3] && nums[3]==nums[4])
                return 40;
            else
                return 0;
        }
    public:
        YahtzeeCategoryScoreRule() {}
        ~YahtzeeCategoryScoreRule() {}
        int GetScore(int,NumberList*);
    };
    int YahtzeeCategoryScoreRule::GetScore(int categoryNumber, Yahtzee_namespace::NumberList * list) {
        int score = 0;
        switch(categoryNumber) {
            case 0:case 1:case 2:case 3:case 4:case 5:
                score = AddAllSelectedNumbers(list,categoryNumber+1);
                break;
            case 6:
                score = AddAllNumbers(list);
                break;
            case 7:
                score = HasSameNumbers(list, 3);
                if(score) score *= 3;
                break;
            case 8:
                score = HasSameNumbers(list, 4);
                if(score) score *= 4;
                break;
            case 9:
                if( HasSameNumbers(list, 5) )
                    score = 50;
                break;
            case 10:
                if( HasStreightNumbers(list,4) )
                    score = 25;
                break;
            case 11:
                if( HasStreightNumbers(list,5) )
                    score = 35;
                break;
            case 12:
                if( HasFullHoush(list) )
                    score = 40;
                break;
            default:
                break;
        }
        return score;
    }
    
    class Yahtzee {
    protected:
        int roundScoreTable[MAX_ROUND][MAX_CATEGORY+1];
        YahtzeeCategoryScoreRule *rule;
        NumberList *inputNumbers[MAX_ROUND];
    protected:
        int bestScore, curScore;
        bool bBestScoreBonus, bCurScoreBonus;
        int bestCombi[MAX_ROUND][2];
        int curCombi[MAX_ROUND][2];
        void GetCurCombiScore(int&,bool&);
        void FindBestScoreCombi(NumberList*,NumberList*,int);
    public:
        Yahtzee();
        ~Yahtzee();
        NumberList* GetRoundNumbers(int);
        int CalculateScore();
        void PrintBestScore();
        void PrintCurScore();
    };
    Yahtzee::Yahtzee() {
        rule = new YahtzeeCategoryScoreRule();
        for(int i=0; i<MAX_ROUND; ++i)
            inputNumbers[i] = new NumberList(5);
    }
    Yahtzee::~Yahtzee() {
        delete rule;
        for(int i=0; i<MAX_ROUND; ++i)
            delete inputNumbers[i];
    }
    NumberList* Yahtzee::GetRoundNumbers(int num) {
        if( num>=0 && num<13) {
            return inputNumbers[num];
        }
        return 0;
    }
    
    int Yahtzee::CalculateScore() {
        // create Table
        for(int round=0; round<MAX_ROUND; ++round) {
            for(int category=0; category<MAX_CATEGORY; ++category) {
                roundScoreTable[round][category] = rule->GetScore(category, inputNumbers[round]);
            }
        }for(int i=0; i<MAX_ROUND; ++i) {
            bestCombi[i][0] = i;
            bestCombi[i][1] = i;
        }
        int currentCategory;
        bool bCategorySelected[13] = {false,};
        GetCurCombiScore(bestScore,bBestScoreBonus);
        for(int round=MAX_ROUND-1; round>=0; --round) {
            for(int category=MAX_CATEGORY-1; category>=0; --category) {
                if(bCategorySelected[category] == false) {
                    currentCategory = category;
                }
            }
            for(int category=MAX_CATEGORY-1; category>=0; --category) {
                if(bCategorySelected[category] == false) {
                    
                }
            }
        }
        /*
         // match all
         NumberList *roundList = new NumberList(13);
         NumberList *categoryList = new NumberList(13);
         for(int i=0; i<MAX_ROUND; ++i) {
         roundList->Insert(i);
         categoryList->Insert(i);
         }
         FindBestScoreCombi(roundList, categoryList, 0);
         delete roundList;
         delete categoryList;
         */
        // return best
        return bestScore;
    }
    void Yahtzee::FindBestScoreCombi(NumberList* round, NumberList* category, int count) {
        if( round->Size() == 0 && category->Size() == 0 ) {
            GetCurCombiScore(curScore, bCurScoreBonus);
            if(curScore > bestScore) {
                bestScore = curScore;
                bBestScoreBonus = bCurScoreBonus;
                for(int i=0; i<MAX_ROUND; ++i) {
                    bestCombi[i][0] = curCombi[i][0];
                    bestCombi[i][1] = curCombi[i][1];
                }
            }
            PrintCurScore();
            return;
        } else {
            int r_idx, c_idx;
            for(int r=0; r<round->Size(); ++r) {
                for(int c=0; c<category->Size(); ++c) {
                    round->Remove(r, r_idx);
                    category->Remove(c,c_idx);
                    curCombi[count][0] = r_idx;
                    curCombi[count][1] = c_idx;
                    FindBestScoreCombi(round, category, count+1);
                    round->Insert(r_idx);
                    category->Insert(c_idx);
                }
            }
        }
    }
    void Yahtzee::GetCurCombiScore(int& totalScore, bool& bBonus) {
        totalScore = 0;
        for(int i=0,round,category; i<6; ++i) {
            round = curCombi[i][0];
            category = curCombi[i][1];
            totalScore += roundScoreTable[round][category];
        }
        if(totalScore >= 63) {
            bBonus = true;
            totalScore += 35;
        } else {
            bBonus = false;
        }
        for(int i=6,round,category; i<MAX_ROUND; ++i) {
            round = curCombi[i][0];
            category = curCombi[i][1];
            totalScore += roundScoreTable[round][category];
        }
    }
    
    
    /*
     int bestScore;
     int bestCombi[MAX_ROUND][2];
     int curCombi[MAX_ROUND][2];
     int GetCurCombiScore();
     int FindBestScoreCombi(NumberList*,NumberList*,int);
     */
    void Yahtzee::PrintCurScore() {
        std::cout << printCount++ << ":";
        for(int i=0; i<MAX_ROUND; ++i) {
            std::cout <<roundScoreTable[ curCombi[i][0] ][ curCombi[i][1] ] << " ";
        }
        if(bCurScoreBonus) {
            std::cout << "35 ";
        } else
            std::cout << "0 ";
        std::cout << curScore << std::endl;
    }
    void Yahtzee::PrintBestScore() {
        for(int i=0; i<MAX_ROUND; ++i) {
            std::cout <<roundScoreTable[ bestCombi[i][0] ][ bestCombi[i][1] ] << " ";
        }
        if(bBestScoreBonus) {
            std::cout << "35 ";
        } else
            std::cout << "0 ";
        std::cout << bestScore << std::endl;
    }
}


int main(int argc, const char * argv[]) {
    Yahtzee_namespace::Yahtzee *testSubject;
    testSubject = new Yahtzee_namespace::Yahtzee();
    for(int i=0; i<Yahtzee_namespace::MAX_ROUND; ++i) {
        Yahtzee_namespace::NumberList *list;
        list = testSubject->GetRoundNumbers(i);
        for(int i=0,num; i<5; ++i) {
            num = rand()%6+1;
            list->Insert(num);
        }
    }
    for(int i=0; i<Yahtzee_namespace::MAX_ROUND; ++i) {
        Yahtzee_namespace::NumberList *list;
        list = testSubject->GetRoundNumbers(i);
        if(i>0) std::cout << std::endl;
        std::cout << i << " : ";
        for(int i=0; i<5; ++i) {
            std::cout << list->Get(i) << ",";
        }
    }
    std::cout << "[" << testSubject->CalculateScore() << "]\n";
    testSubject->PrintBestScore();
    delete testSubject;
    
    return 0;
}

/*
 class Round {
 protected:
 NumberList* list;
 public:
 Round();
 ~Round();
 void Set(NumberList*);
 NumberList* GetNumList();
 };
 Round::Round() { }
 Round::~Round() { }
 void Round::Set(NumberList* _list) {
 list = _list;
 }
 NumberList* Round::GetNumList() {
 return list;
 }
 
 class Category {
 protected:
 int itsNumber;
 public:
 Category(int);
 ~Category();
 void CaculateRound(Round*);
 };
 Category::Category(int _num) {
 itsNumber = _num;
 }
 Category::~Category() {}
 void Category::CaculateRound(Round* _round) {
 return;
 }
 
 void PrintCombi(int* arr, int N) {
 for(int i=0; i<N; ++i) {
 std::cout << arr[i] << ",";
 }
 std::cout << std::endl;
 }
 
 void Combi(int *set, int *arr, int setSize, int arrSize, int C) {
 if(arrSize == 0) {
 PrintCombi(arr,C);
 }
 if(setSize<arrSize)
 return;
 else {
 arr[arrSize-1] = set[setSize-1];
 Combi(set,arr,setSize-1,arrSize-1,C);
 Combi(set,arr,setSize-1,arrSize,C);
 }
 }
 */
