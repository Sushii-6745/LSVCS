#include "Branch.hpp"
#include "Utils.hpp"
bool createBranch(const std::string &branchName){ //Function declaration to create branches.
fs::path branchdir = "./.LSVCS/commit/branch";
    if(!fs::exists(branchdir)){
        fs::create_directory(branchdir);
    }
    std::ofstream FILE("./.LSVCS/commit/branch/"+branchName+".txt");
    std::ifstream HEAD("./.LSVCS/commit/HEAD.txt");
    std::stringstream token;
    token << HEAD.rdbuf();
    FILE << token.str();
    HEAD.close();
    FILE.close();
    return SUCCESS;
}