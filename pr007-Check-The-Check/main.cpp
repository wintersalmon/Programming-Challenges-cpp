//
//  main.cpp
//  pr007-Check-The-Check
//
//  Created by Salmon on 2014. 10. 5..
//  Copyright (c) 2014년 Salmon. All rights reserved.
//

#include <iostream>
#define Max_Board_Size 8

namespace BoardGameChess {
    enum {
        LINE = 0,
        ROW
    };
    enum COLOR {
        Black = 0,
        White,
        colorSize
    };
    enum TYPE {
        Pawn = 0,
        Rock,
        Bishop,
        Queen,
        King,
        Knight,
        typeSize
    };
};

class ChessBoard {
protected:
    int MaxBoardSize;
    char **board;
protected:
    bool isOnBoard(int line, int row) {
        if( line < 0 || line >= MaxBoardSize || row < 0 || row >= MaxBoardSize)
            return false;
        return true;
    }
public:
    ChessBoard(int size) {
        MaxBoardSize = size;
        board = new char*[MaxBoardSize];
        for(int i=0; i<MaxBoardSize; ++i)
            board[i] = new char[MaxBoardSize];
        Clear();
    }
    ~ChessBoard() {
        for(int i=0; i<MaxBoardSize; ++i)
            delete board[i];
        delete [] board;
    }
    void Clear() {
        for(int line=0; line<MaxBoardSize; ++line) {
            for( int row=0; row<MaxBoardSize; ++row) {
                board[line][row] = 0;
            }
        }
    }
    bool HasPieceOn(int line, int row) {
        if( !isOnBoard(line, row) )
            return false;
        if(board[line][row])
            return true;
        return false;
    }
    bool PutPieceOn(char piece, int line, int row) {
        if( !isOnBoard(line, row))
            return false;
        board[line][row] = piece;
        return true;
    }
    char Get(int line, int row) {
        if( !isOnBoard(line, row) )
            return 0;
        return board[line][row];
    }
};

class ChessPieceMoveRule {
protected:
    const int LINE, ROW;
    int dirSize;
    bool bContinuousMove;
    bool bCanJump;
    int **movePos;
public:
    ChessPieceMoveRule(int dir, bool bContinue, bool bJump) : LINE(0), ROW(1) {
        dirSize = dir;
        bContinuousMove = bContinue;
        bCanJump = bJump;
        movePos = new int*[dirSize];
        for(int i=0; i<dirSize; i++)
            movePos[i] = new int[2];
    }
    ~ChessPieceMoveRule() {
        for(int i=0; i<dirSize; i++)
            delete movePos[i];
        delete [] movePos;
    }
    bool CoordOnBoard(int coord) {
        if(coord < 0 || coord >= 8)
            return false;
        return true;
    }
    bool NothingInBetween(ChessBoard *board, int srcLine, int srcRow, int dstLine, int dstRow) {
        if(bCanJump)
            return true;
        else {
            int curLine, curRow;
            curLine = srcLine;
            curRow = srcRow;
            while( true ) {
                if( curLine > dstLine ) --curLine;
                else if( curLine < dstLine) ++curLine;
                
                if( curRow > dstRow ) --curRow;
                else if( curRow < dstRow ) ++curRow;
                if( curLine==dstLine && curRow==dstRow ) return true;
                else if( board->HasPieceOn(curLine, curRow)) {
                    return false;
                }
            }
            
        }
        return true;
    }
    int GetDirSize() {
        return dirSize;
    }
    bool CanMoveTo(ChessBoard *board, int srcLine, int srcRow, int dstLine, int dstRow) {
        int MaxCount;
        if (bContinuousMove) MaxCount = 99;
        else MaxCount = 1;
        
        for(int dir=0; dir < dirSize; dir++) {
            int line = srcLine, row = srcRow;
            for(int count=0; count<MaxCount && CoordOnBoard(line) && CoordOnBoard(row); count++) {
                line += movePos[dir][LINE];
                row += movePos[dir][ROW];
                if(line == dstLine && row == dstRow && NothingInBetween(board, srcLine, srcRow, line, row))
                {
                    return true;
                }
            }
        }
        return false;
    }
};

class PawnMoveRule : public ChessPieceMoveRule {
public:
    PawnMoveRule() : ChessPieceMoveRule(2,false,true) { }
    ~PawnMoveRule() {}
};
class BlackPawnMoveRule : public PawnMoveRule {
public:
    BlackPawnMoveRule()  {
        movePos[0][LINE] = 1;
        movePos[0][ROW] = -1;
        
        movePos[1][LINE] = 1;
        movePos[1][ROW] = 1;
    }
    ~BlackPawnMoveRule() {}
};
class WhitePawnMoveRule : public PawnMoveRule {
public:
    WhitePawnMoveRule() {
        movePos[0][LINE] = -1;
        movePos[0][ROW] = -1;
        
        movePos[1][LINE] = -1;
        movePos[1][ROW] = 1;
    }
    ~WhitePawnMoveRule() {}
};

class RockMoveRule : public ChessPieceMoveRule {
public:
    RockMoveRule() : ChessPieceMoveRule(4,true,false) { }
    ~RockMoveRule() {}
};
class BlackWhiteRockMoveRule : public RockMoveRule {
public:
    BlackWhiteRockMoveRule() {
        movePos[0][LINE] = -1;
        movePos[0][ROW] = 0;
        
        movePos[1][LINE] = 1;
        movePos[1][ROW] = 0;
        
        movePos[2][LINE] = 0;
        movePos[2][ROW] = -1;
        
        movePos[3][LINE] = 0;
        movePos[3][ROW] = 1;
    }
    ~BlackWhiteRockMoveRule() {}
};

class BishopMoveRule : public ChessPieceMoveRule {
public:
    BishopMoveRule() : ChessPieceMoveRule(4,true,false) {}
    ~BishopMoveRule() {}
};
class BlackWhiteBishopMoveRule : public BishopMoveRule {
public:
    BlackWhiteBishopMoveRule() {
        movePos[0][LINE] = -1;
        movePos[0][ROW] = -1;
        
        movePos[1][LINE] = -1;
        movePos[1][ROW] = 1;
        
        movePos[2][LINE] = 1;
        movePos[2][ROW] = -1;
        
        movePos[3][LINE] = 1;
        movePos[3][ROW] = 1;
    }
    ~BlackWhiteBishopMoveRule() {}
};

class QueenMoveRule : public ChessPieceMoveRule {
public:
    QueenMoveRule() : ChessPieceMoveRule(8,true,false) {}
    ~QueenMoveRule() {}
};
class BlackWhiteQueenMoveRule : public QueenMoveRule {
public:
    BlackWhiteQueenMoveRule() {
        movePos[0][LINE] = -1;
        movePos[0][ROW] = 0;
        
        movePos[1][LINE] = 1;
        movePos[1][ROW] = 0;
        
        movePos[2][LINE] = 0;
        movePos[2][ROW] = -1;
        
        movePos[3][LINE] = 0;
        movePos[3][ROW] = 1;
        
        movePos[4][LINE] = -1;
        movePos[4][ROW] = -1;
        
        movePos[5][LINE] = -1;
        movePos[5][ROW] = 1;
        
        movePos[6][LINE] = 1;
        movePos[6][ROW] = -1;
        
        movePos[7][LINE] = 1;
        movePos[7][ROW] = 1;
    }
    ~BlackWhiteQueenMoveRule();
};

class KingMoveRule : public ChessPieceMoveRule {
public:
    KingMoveRule() : ChessPieceMoveRule(8,false,false) {}
    ~KingMoveRule() {}
};
class BlackWhiteKingMoveRule : public KingMoveRule {
public:
    BlackWhiteKingMoveRule() {
        movePos[0][LINE] = -1;
        movePos[0][ROW] = 0;
        
        movePos[1][LINE] = 1;
        movePos[1][ROW] = 0;
        
        movePos[2][LINE] = 0;
        movePos[2][ROW] = -1;
        
        movePos[3][LINE] = 0;
        movePos[3][ROW] = 1;
        
        movePos[4][LINE] = -1;
        movePos[4][ROW] = -1;
        
        movePos[5][LINE] = -1;
        movePos[5][ROW] = 1;
        
        movePos[6][LINE] = 1;
        movePos[6][ROW] = -1;
        
        movePos[7][LINE] = 1;
        movePos[7][ROW] = 1;
    }
    ~BlackWhiteKingMoveRule() {}
};

class KnightMoveRule : public ChessPieceMoveRule {
public:
    KnightMoveRule() : ChessPieceMoveRule(8,false,true) {}
    ~KnightMoveRule() {}
};
class BlackWhiteKnightMoveRule : public KnightMoveRule {
public:
    BlackWhiteKnightMoveRule() {
        movePos[0][LINE] = -2;
        movePos[0][ROW] = -1;
        
        movePos[1][LINE] = -2;
        movePos[1][ROW] = 1;
        
        movePos[2][LINE] = -1;
        movePos[2][ROW] = -2;
        
        movePos[3][LINE] = -1;
        movePos[3][ROW] = 2;
        
        movePos[4][LINE] = 2;
        movePos[4][ROW] = -1;
        
        movePos[5][LINE] = 2;
        movePos[5][ROW] = 1;
        
        movePos[6][LINE] = 1;
        movePos[6][ROW] = -2;
        
        movePos[7][LINE] = 1;
        movePos[7][ROW] = 2;
    }
    ~BlackWhiteKnightMoveRule() {}
};

class ChessBoardCheck {
protected:
    int pieceCount;
    int kingMarker[2][2];
    bool kingInPosition[2];
    ChessBoard *board;
    ChessPieceMoveRule ***moveRules;
protected:
    int GetPieceColor(char);
    bool CanPieceMoveTo(char,int,int,int,int);
    bool isKingInCheck(int,int,int);
public:
    ChessBoardCheck();
    ~ChessBoardCheck();
    void ClearBoard();
    bool HasKing();
    bool isBoardBlank();
    bool PutPieceOnBoard(char,int,int);
    bool isBlackKingInCheck();
    bool isWhiteKingInCheck();
};
ChessBoardCheck::ChessBoardCheck() {
    board = new ChessBoard(8);
    moveRules = new ChessPieceMoveRule**[BoardGameChess::colorSize];
    for(int i=0; i<BoardGameChess::colorSize; ++i)
        moveRules[i] = new ChessPieceMoveRule*[BoardGameChess::typeSize];
    moveRules[BoardGameChess::Black][BoardGameChess::Pawn] = new BlackPawnMoveRule();
    moveRules[BoardGameChess::Black][BoardGameChess::Rock] = new BlackWhiteRockMoveRule();
    moveRules[BoardGameChess::Black][BoardGameChess::Bishop] = new BlackWhiteBishopMoveRule();
    moveRules[BoardGameChess::Black][BoardGameChess::Queen] = new BlackWhiteQueenMoveRule();
    moveRules[BoardGameChess::Black][BoardGameChess::King] = new BlackWhiteKingMoveRule();
    moveRules[BoardGameChess::Black][BoardGameChess::Knight] = new BlackWhiteKnightMoveRule();
    
    moveRules[BoardGameChess::White][BoardGameChess::Pawn] = new WhitePawnMoveRule();
    moveRules[BoardGameChess::White][BoardGameChess::Rock]
    = moveRules[BoardGameChess::Black][BoardGameChess::Rock];
    moveRules[BoardGameChess::White][BoardGameChess::Bishop]
    = moveRules[BoardGameChess::Black][BoardGameChess::Bishop];
    moveRules[BoardGameChess::White][BoardGameChess::Queen]
    = moveRules[BoardGameChess::Black][BoardGameChess::Queen];
    moveRules[BoardGameChess::White][BoardGameChess::King]
    = moveRules[BoardGameChess::Black][BoardGameChess::King];
    moveRules[BoardGameChess::White][BoardGameChess::Knight]
    = moveRules[BoardGameChess::Black][BoardGameChess::Knight];
    ClearBoard();
}
ChessBoardCheck::~ChessBoardCheck() {
    delete board;
    for(int i=0; i<BoardGameChess::typeSize; ++i)
        delete moveRules[BoardGameChess::Black][i];
    delete moveRules[BoardGameChess::White][BoardGameChess::Pawn];
    
    for(int i=0; i<BoardGameChess::colorSize; ++i)
        delete [] moveRules[i];
    delete moveRules;
}
void ChessBoardCheck::ClearBoard() {
    board->Clear();
    kingInPosition[BoardGameChess::Black] = false;
    kingInPosition[BoardGameChess::White] = false;
    pieceCount = 0;
}
bool ChessBoardCheck::HasKing() {
    if( kingInPosition[BoardGameChess::Black] && kingInPosition[BoardGameChess::White] )
        return true;
    return false;
}
bool ChessBoardCheck::isBoardBlank() {
    if( pieceCount == 0 ) return true;
    else return false;
}
bool ChessBoardCheck::PutPieceOnBoard(char piece, int line, int row) {
    if( piece == 'k' || piece == 'K') {
        int kingColor = GetPieceColor(piece);
        kingMarker[kingColor][BoardGameChess::LINE] = line;
        kingMarker[kingColor][BoardGameChess::ROW] = row;
        kingInPosition[kingColor] = true;
    }
    ++pieceCount;
    return board->PutPieceOn(piece,line,row);
}
int ChessBoardCheck::GetPieceColor(char piece) {
    if( piece >= 'a' && piece <= 'z')
        return BoardGameChess::Black;
    else
        return BoardGameChess::White;
}
bool ChessBoardCheck::CanPieceMoveTo(char piece, int srcLine, int srcRow, int dstLine, int dstRow) {
    int color,type;
    if( piece >= 'a' && piece <= 'z' ) {
        color = BoardGameChess::Black;
    } else {
        color = BoardGameChess::White;
        piece = piece - 'A' + 'a';
    }
    switch (piece) {
        case 'p':
            type = BoardGameChess::Pawn;
            break;
        case 'r':
            type = BoardGameChess::Rock;
            break;
        case 'b':
            type = BoardGameChess::Bishop;
            break;
        case 'q':
            type = BoardGameChess::Queen;
            break;
        case 'k':
            type = BoardGameChess::King;
            break;
        case 'n':
            type = BoardGameChess::Knight;
            break;
        default:
            type = 0;
    }
    
    return moveRules[color][type]->CanMoveTo(board, srcLine, srcRow, dstLine, dstRow);
}
bool ChessBoardCheck::isBlackKingInCheck() {
    if( !kingInPosition[BoardGameChess::Black] )
        return false;
    return isKingInCheck(BoardGameChess::Black,
                         kingMarker[BoardGameChess::Black][BoardGameChess::LINE],
                         kingMarker[BoardGameChess::Black][BoardGameChess::ROW]);
}
bool ChessBoardCheck::isWhiteKingInCheck() {
    if( !kingInPosition[BoardGameChess::White] )
        return false;
    return isKingInCheck(BoardGameChess::White,
                         kingMarker[BoardGameChess::White][BoardGameChess::LINE],
                         kingMarker[BoardGameChess::White][BoardGameChess::ROW]);
}

bool ChessBoardCheck::isKingInCheck(int kingColor, int kingInLine, int kingInRow) {
    for(int line=0; line<8; ++line) {
        for(int row=0; row<8; ++row) {
            if(board->HasPieceOn(line, row)) {
                char piece = board->Get(line, row);
                if( GetPieceColor(piece)!=kingColor && CanPieceMoveTo(piece,line,row,kingInLine,kingInRow) )
                    return true;
            }
        }
    }
    return false;
}


int main(int argc, const char * argv[]) {
    ChessBoardCheck *checker = new ChessBoardCheck();
    char c;
    int caseCount;
    int resultNumber;
    char resultMsg[3][10] = {"black", "white","no"};
    char inputLine[9];
    
    for(caseCount=0; true; caseCount++) {
        checker->ClearBoard();
        // Input
        if(caseCount>0) {
            std::cin.get(c);
        }
        for(int lineCount=0; lineCount<Max_Board_Size; lineCount++) {
            std::cin >> inputLine;
            for(int rowCount=0; rowCount<Max_Board_Size; rowCount++) {
                c = inputLine[rowCount];
                if( c != '.') {
                    checker->PutPieceOnBoard(c, lineCount, rowCount);
                }
            }
        }
        //if blank board exit
        if( checker->isBoardBlank() ) return 0;
        
        // Check
        if( checker->isBlackKingInCheck() ) resultNumber = 0;
        else if( checker->isWhiteKingInCheck() ) resultNumber = 1;
        else resultNumber = 2;
        
        //Print
        std::cout << "Game #" << caseCount+1 << ": ";
        std::cout << resultMsg[resultNumber] << " king is in check.\n";
    }
    return 0;
}