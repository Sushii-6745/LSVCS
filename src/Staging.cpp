#include "Utils.hpp"
#include "SHA256.hpp"
#include "Staging.hpp"
        void dummy_onlystaging (std::string dirPath){
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
                    dummy_onlystaging(path);
                }
            }
        }
}
std::string dummy2_onlystaging(){
    dummy_onlystaging(".");
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
        
bool Staging::add(){
    std::string stageAreaTreeHash = dummy2_onlystaging(); 
    std::string path = "./.LSVCS/stage_area.txt";
    if(fs::exists(path)){
        std::ifstream CHECK(path);
        if(CHECK.is_open()){
                std::stringstream token;
                token << CHECK.rdbuf();
                if(token.str() == stageAreaTreeHash){
                    std::cout <<"Already Added ;)\n";
                }
                else{
                    std::cout << "WARNING : Data Overwritten in staging area.\n";

                }
        }
        else{
            return FAILURE;
        }
    }
    std::ofstream FILE(path);
    if(FILE.is_open()){
        FILE << stageAreaTreeHash;
        std::cout << "Added to the staging area.\n";
    }
    return SUCCESS;
}