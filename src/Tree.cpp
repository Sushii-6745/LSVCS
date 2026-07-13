#include "Utils.hpp"
#include "Tree.hpp"
#include "SHA256.hpp"
void Tree::dummy(std::string dirPath){
    if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
            for (const auto& entry : fs::directory_iterator(dirPath)) {
                std::string type = entry.is_directory() ? "[DIR] " : "[FILE]";
                if(entry.path().string() == "./.LSVCS") continue;
                mp[dirPath].push_back(entry.path().string());
                if(type == "[FILE]"){
                    std::ifstream FILE(entry.path().string());
                    std::stringstream token;
                    token << FILE.rdbuf();
                    std::string data = sha256(token.str());
                    std::ofstream BLOBFILE("./.LSVCS/blob/"+data+".txt");
                    BLOBFILE << token.str();
                    FILE.close();
                    BLOBFILE.close();
                    mp[entry.path().string()].push_back("./.LSVCS/blob/"+data+".txt");
                }
                if(type == "[DIR] "){
                    std::string path = entry.path().string();
                    dummy(path);
                }
            }
        }
}
std::string Tree::dummy2(){
    Tree::dummy(".");
    fileS::FileStructure parser;
    parser.create(mp, "./.LSVCS/tree/temp.des");
    std::ifstream FILE("./.LSVCS/tree/temp.des");
    std::stringstream token;
    token << FILE.rdbuf();
    std::string treeHash = sha256(token.str());
    FILE.close();
    std::ofstream TREE("./.LSVCS/tree/"+treeHash+".des");
    TREE << token.str();
    fs::remove("./.LSVCS/tree/temp.des");
    TREE.close();
    return treeHash;
}
std::string Tree::changeHead(std::string branchName){
    std::ifstream BRANCH("./.LSVCS/commit/branch/"+branchName+".txt");
    std::stringstream token;
    std::string currBranch;
    token << BRANCH.rdbuf();
    currBranch = token.str();
    BRANCH.close();
    std::ofstream HEAD("./.LSVCS/commit/HEAD.txt");
    HEAD << currBranch;
    HEAD.close();
    currHeadPosition = branchName;
    return currBranch;
}