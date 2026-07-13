#include "Checkout.hpp"
#include "Utils.hpp"

bool Checkout::checkit(std::string &hash){  
        std::string currhash = "";
        std::ifstream HEAD("./.LSVCS/commit/HEAD.txt");
        if(HEAD.is_open()){
            std::stringstream token;
            token << HEAD.rdbuf();
            currhash = token.str();
            HEAD.close();
        }
        std::string path = "./.LSVCS/commit/";
        std::ifstream FILE_curr(path+hash+".txt");
        std::string treeHash = "";
        if(FILE_curr.is_open()){
            std::string token;
            while(FILE_curr >> token){
                if(token == "TREE_HASH_:") continue;
                else {
                    break;
                }
            }
            treeHash = token;
            FILE_curr.close();
        }
        else{
            return FAILURE;
        }
        std::string dirPath = ".";
        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if(entry.path().string() == "./.LSVCS" || entry.path().string() == "." || entry.path().string() =="./exec.cpp" || entry.path().string() == "./JSONparser" || entry.path().string() == "./JSONparser.cpp"|| entry.path().string() == "./JSONparser.hpp" || entry.path().string() == "./exec") continue;
            std::string command = "rm -rf "+entry.path().string();
            system(command.c_str());
        }
        fileS::FileStructure parser;
        std::string treepath = "./.LSVCS/tree/"+treeHash+".des";
        parser.read(treepath);
        if(parser.isParseSuccess){
            auto &fdata = parser.data();
            std::map<std::string, std::vector<std::string>>mp;
            for (const auto& [k, v] : fdata){
                std::vector<std::string>children;
                for (const auto& p : v){
                    children.push_back(p->data);
                }
                mp[k] = children;
            }
            for(auto [p,e]: mp){
                if(p == "./.LSVCS" || p == "." || p =="./exec.cpp" || p == "./JSONparser" || p == "./JSONparser.cpp"|| p == "./JSONparser.hpp" || p == "./exec") continue;

                if(e[0].substr(0,14) == "./.LSVCS/blob/"){
                    std::string file_data;
                    std::ifstream BLOB(e[0]);
                    std::stringstream token;
                    token << BLOB.rdbuf();
                    file_data = token.str();
                    BLOB.close();
                    std::ofstream MAIN(p);
                    MAIN << file_data;
                    MAIN.close();
                }
                else if(!fs::exists(p)){
                    fs::create_directory(p);
                }
            }
        }
        else{
            return FAILURE;
        }
return SUCCESS;

}