#include "Commit.hpp"
#include "Utils.hpp"
#include "SHA256.hpp"
#include "Branch.hpp"
#include "defaults.hpp"

bool Commit::commitit(std::string &message){
    setCurrHeadPosition();
    std::string staging_area = "./.LSVCS/stage_area.txt";
    if(!fs::exists(staging_area)){
        std::cout <<"Files have not been added. :(\n";
        return SUCCESS;
    }
    std::string treeHash = dummy2();
    struct commit{
        std::string treeHash;
        std::string parentHash;
        std::string commitHash;
        std::string commitMessage;
    } currCommit;

    currCommit.treeHash = treeHash;
    currCommit.commitMessage = message;
    bool IsDirExists = false;
    fs::path commitDir= "./.LSVCS/commit";
    if(fs::is_directory(commitDir)){
        IsDirExists = true;
    }
    else{
        try{
            if(fs::create_directory(commitDir)){
                std::ofstream HEAD("./.LSVCS/commit/HEAD.txt");
                createBranch("main");
                HEAD.close();
                currCommit.parentHash = "NULL";
            }
        }
        catch(const fs::filesystem_error&e){
            std::cerr <<"Oops! => "<<e.what() <<"\n";
            return FAILURE;
        }
    }
    if(IsDirExists){
        std::ifstream HEAD("./.LSVCS/commit/HEAD.txt");
        std::string prevCommit ="";
        if(HEAD.is_open()){
            std::stringstream token;
            token << HEAD.rdbuf();
            prevCommit = token.str();
        }
        HEAD.close();
        currCommit.parentHash = prevCommit;
    }

    std::string CommitHash = currCommit.treeHash+currCommit.parentHash+currCommit.commitMessage;
    currCommit.commitHash = sha256(CommitHash);


    std::ifstream STAGING_AREA("./.LSVCS/stage_area.txt");
    std::stringstream token;
    token << STAGING_AREA.rdbuf();
    std::string StageHash = token.str();
    if(currCommit.treeHash != StageHash){
        std::cout << "WARNING: Unmodified file, earlier staged data is being commited :(\n";
    }
    if(fs::exists("./.LSVCS/commit/"+currCommit.commitHash+".txt")){
        fs::remove("./.LSVCS/stage_area.txt");
        std::cout << "Already commited ;)\n";
        return SUCCESS;
    }
    std::string commitFile = "./.LSVCS/commit/" + currCommit.commitHash +".txt";
    std::ofstream FILE(commitFile);
    if(FILE.is_open()){
        FILE << "TREE_HASH_: "<<currCommit.treeHash<<"\nHASH_: "<<currCommit.commitHash<<"\n"<<"PARENT_HASH_: "<<currCommit.parentHash<<"\n"<<"MESSAGE: "<<currCommit.commitMessage<<"\n";
        FILE.close();
        std::ofstream HEAD("./.LSVCS/commit/HEAD.txt");
        HEAD << currCommit.commitHash;
        std::ofstream BRANCH("./.LSVCS/commit/branch/"+currHeadPosition+".txt");
        BRANCH << currCommit.commitHash;
        BRANCH.close();
        HEAD.close();
        std::cout <<"Committed successfully\n";
        fs::remove("./.LSVCS/stage_area.txt");
        return SUCCESS;
    }
    else{
        std::cout <<"Oops! => Error in commit :[\n";
        return FAILURE;
    }
     
}