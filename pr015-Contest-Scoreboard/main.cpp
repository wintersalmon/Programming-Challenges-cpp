//
//  main.cpp
//  pr015-Contest-Scoreboard
//
//  Created by Salmon on 2014. 10. 26..
//  Copyright (c) 2014년 SalmonJoon. All rights reserved.
//
#include <iostream>
#include <vector>
#include <map>

class SubmitResult {
public:
    enum TAG {
        NONE,
        CORRECT,
        INCORRECT,
        CLARIFICATION_REQUEST,
        UNJUDGED,
        ERRONEOUS_SUBMISSION,
        //C, I, R, U, E
        //Correct, Incorrect, clarification Request, Unjudged, and Erroneous submission.
    };
protected:
    int itsTeamId;
    int itsProblemId;
    int itsSubmitTime;
    TAG itsResultTag;
    SubmitResult * itsLinkNode;
    int CountAllNodeUnder();
    int CountAllPenaltyTime();
public:
    SubmitResult(int teamId, int problemId, int submitTime, char resultTag);
    ~SubmitResult();
    void init(int teamId, int problemId, int submitTime, char resultTag);
    int getTeamId();
    int getProbemId();
    int getSubmitTime();
    int PenaltyTime();
    TAG ResultTag();
    int LinkRelatedSubmission(SubmitResult * submit);
    bool hasSolved();
};
SubmitResult::SubmitResult(int teamId, int problemId, int submitTime, char resultTag) {
    init(teamId, problemId, submitTime, resultTag);
}
SubmitResult::~SubmitResult() {
}
void SubmitResult::init(int teamId, int problemId, int submitTime, char resultTag) {
    itsTeamId = teamId;
    itsProblemId = problemId;
    itsSubmitTime = submitTime;
    
    TAG inputSubmitTag = NONE;
    switch (resultTag) {
        case 'C':case 'c':
            inputSubmitTag = CORRECT;
            break;
        case 'I':case 'i':
            inputSubmitTag = INCORRECT;
            break;
        case 'R':case 'r':
            inputSubmitTag = CLARIFICATION_REQUEST;
            break;
        case 'U':case 'u':
            inputSubmitTag = UNJUDGED;
            break;
        case 'E':case 'e':
            inputSubmitTag = ERRONEOUS_SUBMISSION;
            break;
    }
    itsResultTag = inputSubmitTag;
    
    itsLinkNode = nullptr;
}
int SubmitResult::CountAllNodeUnder() {
    if (itsLinkNode == nullptr) {
        return 0;
    } else {
        return 1 + itsLinkNode->CountAllNodeUnder();
    }
}
int SubmitResult::LinkRelatedSubmission(SubmitResult *submit) {
    // TODO : 아직 손볼곳이 남았다
    // 지금 상태는 링크가 실패할 경우를 처리하지 못할것이다
    if (itsProblemId != submit->itsProblemId)
        return 0;
    
    if (itsLinkNode != nullptr)
        itsLinkNode->LinkRelatedSubmission(submit);
    else
        itsLinkNode = submit;
    
    return CountAllNodeUnder();
}
int SubmitResult::getTeamId(){
    return itsTeamId;
}
int SubmitResult::getProbemId() {
    return itsProblemId;
}
int SubmitResult::getSubmitTime() {
    return itsSubmitTime;
}
int SubmitResult::PenaltyTime() {
    if (itsResultTag == CORRECT) {
        return itsSubmitTime;
    } else if (itsLinkNode == nullptr) {
        return 0;
    } else if (itsResultTag == INCORRECT) {
        return 20 + itsLinkNode->PenaltyTime();
    } else {
        return itsLinkNode->PenaltyTime();
    }
}
SubmitResult::TAG SubmitResult::ResultTag() {
    return itsResultTag;
}
bool SubmitResult::hasSolved() {
    if (itsResultTag == CORRECT)
        return true;
    
    if(itsLinkNode != nullptr)
        return itsLinkNode->hasSolved();
    
    return false;
}

class Team {
protected:
    int itsId;
    std::map<int,SubmitResult*> resultSet;
public:
    Team(int id);
    ~Team();
    int getId();
    int TotalSolvedProblemCount();
    int TotalPenaltyTime();
    int Submit(SubmitResult * submit);
    void Reset();
};
Team::Team(int id) {
    itsId = id;
}
Team::~Team() {
}
int Team::getId() {
    return itsId;
}
int Team::TotalSolvedProblemCount() {
    int solveCount = 0;
    for(auto it = resultSet.begin(); it != resultSet.end(); it++) {
        if (it->second->hasSolved()) {
            solveCount++;
        }
    }
    return solveCount;
}
int Team::TotalPenaltyTime() {
    int penaltyTime;
    for(auto it = resultSet.begin(); it != resultSet.end(); it++) {
        if (it->second->hasSolved()) {
            penaltyTime += it->second->PenaltyTime();
        }
    }
    return penaltyTime;
}
int Team::Submit(SubmitResult *submit) {
    if ( submit->getTeamId() != itsId )
        return 0;
    auto it = resultSet.find(submit->getProbemId());
    if (it != resultSet.end()) {
        it->second->LinkRelatedSubmission(submit);
        //insert under it
    } else {
        resultSet.insert( std::pair<int,SubmitResult*>(submit->getProbemId(), submit));
    }
    return submit->getProbemId();
}
void Team::Reset() {
    resultSet.clear();
}

class ScoreBoard {
    
};


namespace ContestScoreboardNamespace {
    
    enum SubmissionResultTag {
        Correct = 0,
        Incorrect,
        ClarificationRequest,
        Unjudged,
        ErroneousSubmission,
        InputCodeCorrect = 'C',
        InputCodeIncorrect = 'I',
        InputCodeClarificationRequest = 'R',
        InputCodeUnjudged = 'U',
        InputCodeErroneousSubmission = 'E'
    };
    //const char SubmissionResultChar[5] = { 'C', 'I', 'R', 'U', 'E'};
    
    class SubmissionInfo {
    protected:
        int teamNumber;
        int problemNumber;
        int submissionTime;
        int submissionResult;
    public:
        SubmissionInfo();
        ~SubmissionInfo();
        void Set(int,int,int,int);
        int GetTeamNum();
        int GetProblemNum();
        int GetSubmissionTime();
        int GetSubmissionResult();
    };
    SubmissionInfo::SubmissionInfo() {
        teamNumber = 0;
        problemNumber = 0;
        submissionTime = 0;
        submissionResult = 0;
    }
    SubmissionInfo::~SubmissionInfo() {
    }
    void SubmissionInfo::Set(int teamNum, int probNum, int submitTime, int result) {
        teamNumber = teamNum;
        problemNumber = probNum;
        submissionTime = submitTime;
        submissionResult = result;
    }
    
    
    class SubmissionHistory {
    protected:
        std::vector<SubmissionInfo*> *history;
    public:
        SubmissionHistory();
        ~SubmissionHistory();
        int Size();
        int Insert(SubmissionInfo*);
        int Get(int,SubmissionInfo**);
        int Remove(SubmissionInfo*);
        void Clear();
    };
    
    class TeamScoreRecorder {
    protected:
        int teamNumber;
        int numOfSolvedProblems;
        int penaltyTime;
    public:
        TeamScoreRecorder(int);
        ~TeamScoreRecorder();
        int GetTeamNumber();
        int GetNumOfSolvedProblems();
        int GetPenaltyTime();
        void IncreaseNumOfSolvedProblems();
        void IncreasePenaltyTime(int);
        void Reset();
    };
    
    class ContestScoreboard {
    protected:
        int numOfTeamInContest;
        TeamScoreRecorder **teamRecords;
        SubmissionHistory *submitHistory;
        void CalculateTeamResult(TeamScoreRecorder*,SubmissionHistory*);
        void SortTeamInorder();
    public:
        ContestScoreboard(int);
        ~ContestScoreboard();
        void Reset();
        void PrintResult();
        void SetSubmissionHistory(SubmissionHistory*);
    };
}

// Class Printers
void PrintSubmitResult(SubmitResult * submit);

// Class Testers
void TestSubmitResult();

int main(int argc, const char * argv[]) {
    TestSubmitResult();
    return 0;
}

void PrintSubmitResult(SubmitResult * submit) {
    if (submit == nullptr) std::cout << "Pointer ERROR\n";
    std::cout << "[T:" << submit->getTeamId();
    std::cout << ", P:" << submit->getProbemId();
    std::cout << ", S:" << submit->getSubmitTime();
    std::cout << ", R:" << submit->ResultTag();
    std::cout << " - ";
    std::cout << submit->hasSolved() << "/" << submit->PenaltyTime();
    std::cout << "]\n";
}

void TestSubmitResult() {
    // Test SubmitResult
    const int TEST_COUNT = 3;
    int team, problem, time;
    char tag;
    SubmitResult * submit[TEST_COUNT];
    for(int i=0; i<TEST_COUNT; i++) {
        std::cin >> team >> problem >> time >> tag;
        submit[i] = new SubmitResult(team,problem,time,tag);
        PrintSubmitResult(submit[i]);
        if(i>=1) {
            submit[0]->LinkRelatedSubmission(submit[i]);
        }
    }
    PrintSubmitResult(submit[0]);
    
    for(int i=0; i<TEST_COUNT; i++) {
        delete submit[i];
    }
    
}
