//
//  main.cpp
//  pr014-Erdos-Numbers
//
//  Created by Salmon on 2014. 10. 17..
//  Copyright (c) 2014년 SalmonJoon. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>

//str.find(str_name) != std::string::npos

class StringList : protected std::vector<std::string*> {
public:
    enum StringListErrorTag {
        ERROR = -1
    };
public:
    StringList();
    ~StringList();
    int Size();
    int EmptyAllElement();
    int Insert(std::string*);
    int Remove(std::string*);
    std::string* Get(int);
};
StringList::StringList() {
}
StringList::~StringList() {
    EmptyAllElement();
}
int StringList::Size() {
    return (int) this->size();
}
int StringList::EmptyAllElement() {
    while( !this->empty() ) {
        std::string* back = this->back();
        this->pop_back();
        delete back;
    }
    return Size();
}
int StringList::Insert(std::string * str) {
    if(str == 0) {
        return ERROR;
    } else {
        this->push_back(str);
        return Size();
    }
}
int StringList::Remove(std::string* str) {
    if(str != 0) {
        std::vector<std::string*>::iterator it = this->begin();
        while( it != this->end() ) {
            if( *it == str ) {
                this->erase(it);
                return Size();
            }
        }
    }
    return ERROR;
}
std::string* StringList::Get(int idx) {
    if(idx<0 || idx> Size())
        return 0;
    return (*this)[idx];
}

class ErdosNumber {
protected:
    std::string *erdosNameStr;
    StringList *articleInfoList;
    StringList *authorNameList;
    std::vector<int> *articleErdosNumberList;
    std::vector<int> *authorErdosNumList;
    void SetErdosNumbers();
public:
    ErdosNumber();
    ~ErdosNumber();
    void Set(StringList*,StringList*);
    void PrintResult();
};
ErdosNumber::ErdosNumber() {
    articleInfoList = 0;
    authorNameList = 0;
    articleErdosNumberList = new std::vector<int>;
    authorErdosNumList = new std::vector<int>;
    erdosNameStr = new std::string("Erdos, P.");
}
ErdosNumber::~ErdosNumber() {
    delete articleErdosNumberList;
    delete authorErdosNumList;
    delete erdosNameStr;
}
void ErdosNumber::Set(StringList * articles, StringList * authors) {
    if( articles != 0 && authors != 0) {
        articleInfoList = articles;
        authorNameList = authors;
        articleErdosNumberList->clear();
        authorErdosNumList->clear();
        SetErdosNumbers();
    }
}
void ErdosNumber::SetErdosNumbers() {
    articleErdosNumberList->assign(articleInfoList->Size(), 0);
    authorErdosNumList->assign(authorNameList->Size(), 0);
    for(int articleCount=0; articleCount<articleInfoList->Size(); ++articleCount) {
        std::string* articleInfo = articleInfoList->Get(articleCount);
        if(articleInfo != 0 && articleInfo->find(*erdosNameStr) != std::string::npos) {
            (*articleErdosNumberList)[articleCount] = 1;
        }
    }
    for(int authorCount=0; authorCount<authorNameList->Size(); ++authorCount) {
        std::string* authorName = authorNameList->Get(authorCount);
        for(int articleCount=0; articleCount<articleInfoList->Size(); ++articleCount) {
            if( (*articleErdosNumberList)[articleCount] == 1 &&
               articleInfoList->Get(articleCount)->find(*authorName) != std::string::npos)
            {
                (*authorErdosNumList)[authorCount] = 1;
            }
        }
    }
    for(int articleCount=0; articleCount<articleInfoList->Size(); ++articleCount) {
        std::string* articleStr = articleInfoList->Get(articleCount);
        bool bHasErodosReferenceName;
        if( (*articleErdosNumberList)[articleCount] == 0) {
            bHasErodosReferenceName = false;
            for(int authorCount=0; authorCount<authorNameList->Size(); ++authorCount) {
                std::string* authorStr = authorNameList->Get(authorCount);
                if( articleStr->find(*authorStr) != std::string::npos && (*authorErdosNumList)[authorCount] == 1 ) {
                    bHasErodosReferenceName = true;
                }
            }
            if(bHasErodosReferenceName)
                (*articleErdosNumberList)[articleCount] = 2;
        }
    }
    for(int authorCount=0; authorCount<authorNameList->Size(); ++authorCount) {
        std::string* authorName = authorNameList->Get(authorCount);
        for(int articleCount=0; articleCount<articleInfoList->Size(); ++articleCount) {
            if( (*articleErdosNumberList)[articleCount] == 2 &&
               articleInfoList->Get(articleCount)->find(*authorName) != std::string::npos &&
               (*authorErdosNumList)[authorCount] != 1
               )
            {
                (*authorErdosNumList)[authorCount] = 2;
            }
        }
    }
}
void ErdosNumber::PrintResult() {
    for(int authorCount=0; authorCount<authorNameList->Size(); ++authorCount) {
        std::string *authorStr = authorNameList->Get(authorCount);
        std::string resultStr = "";
        switch( (*authorErdosNumList)[authorCount] ) {
            case 0:
                resultStr = "infinity";
                break;
            case 1:
                resultStr = "1";
                break;
            case 2:
                resultStr = "2";
                break;
            default:
                break;
        }
        std::cout << *authorStr << " ";
        std::cout << resultStr << std::endl;
    }
}
int main(int argc, const char * argv[]) {
    char c;
    int scenarioCount = 0, MaxScenarioCount = 0;
    int numOfArticles, numOfAuthors;
    char inputLine[1000];
    StringList *articleList = new StringList();
    StringList *authorList = new StringList();
    ErdosNumber *erdosTest = new ErdosNumber();
    
    std::cin >> MaxScenarioCount;
    while (scenarioCount < MaxScenarioCount) {
        std::cin >> numOfArticles >> numOfAuthors;
        std::cin.get(c);
        for(int i=0; i < numOfArticles; ++i) {
            std::cin.getline(inputLine, 1000, '\n');
            std::string *inputString = new std::string(inputLine);
            articleList->Insert(inputString);
        }
        for(int i=0; i < numOfAuthors; ++i) {
            std::cin.getline(inputLine, 1000, '\n');
            std::string *inputString = new std::string(inputLine);
            authorList->Insert(inputString);
        }
        erdosTest->Set(articleList, authorList);
        
        if(scenarioCount > 0)
            std::cout << std::endl;
        std::cout << "Scenario " << scenarioCount+1 << std::endl;
        erdosTest->PrintResult();
        
        articleList->EmptyAllElement();
        authorList->EmptyAllElement();
        ++scenarioCount;
    }
    return 0;
}