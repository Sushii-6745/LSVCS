#include "Utils.hpp"
void dummy(std::string dirPath){
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
std::string dummy2(){
    dummy(".");
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
void setCurrHeadPosition(){ //Checks if the HEAD is on the correct branch and initializes it to the correct branch
        std::string HEADpos;
        if(fs::exists("./.LSVCS/commit/HEAD.txt")){
        std::ifstream HEAD("./.LSVCS/commit/HEAD.txt");
        std::stringstream token;
        token << HEAD.rdbuf();
        HEADpos = token.str();
        }
        else{
            return ;
        }
        fs::path branch = "./.LSVCS/commit/branch";
        if(fs::exists(branch)){
            for(const auto& entry : fs::directory_iterator(branch)){
                std::ifstream FILE(entry.path().string());
                std::stringstream branches;
                branches << FILE.rdbuf();
                if(branches.str() == HEADpos){
                    currHeadPosition = entry.path().stem().string();
                    break;
                }
            }
        }
        else{
            return;
        }

}
