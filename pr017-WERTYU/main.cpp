//
//  main.cpp
//  pr017-WERTYU
//
//  Created by Salmon on 2014. 12. 9..
//  Copyright (c) 2014년 SalmonJoon. All rights reserved.
//

#include <iostream>

namespace WERTYU {
    
    class KeyTranslater {
    protected:
        const char *keyboardKeyValueArr[4] = {
            "1234567890-=", "QWERTYUIOP[]\\", "ASDFGHJKL\'", "ZXCVBNM,./"
        };
        int isInLine(char,const char*);
    public:
        KeyTranslater();
        ~KeyTranslater();
        void Translate();
    };
    KeyTranslater::KeyTranslater() {
    }
    KeyTranslater::~KeyTranslater() {
    }
    int KeyTranslater::isInLine(char key, const char *line) {
        int index = 0;
        while (true) {
            if(key == line[index]) {
                break;
            } else if(line[index] == 0) {
                break;
            }
            index++;
        }
        return index;
    }
    void KeyTranslater::Translate() {
        char InputKey;
        while(true) {
            std::cin.get(InputKey);
            if(InputKey == 0) return;
            else if(InputKey == 32 || InputKey == 10) std::cout << InputKey;
            else {
                for(int line=0, keyIndex; line<4; line++) {
                    keyIndex = 0;
                    keyIndex = isInLine(InputKey,keyboardKeyValueArr[line]);
                    if(keyIndex) {
                        std::cout << keyboardKeyValueArr[line][keyIndex-1];
                        break;
                    }
                }
            }
        }
        return;
    }
}


int main(int argc, const char * argv[]) {
    WERTYU::KeyTranslater *test = new WERTYU::KeyTranslater();
    test->Translate();
    delete test;
}
/*
 for(int line=0; line<4 ; ++line) {
 for(int idx=0; ; idx++) {
 if(WERTYU::keyboardKeyValueArr[line][idx] == 0) break;
 std::cout << "[" << idx << "]" << WERTYU::keyboardKeyValueArr[line][idx] << "\n";
 }
 std::cout << "\n";
 }
 return 0;
 */
