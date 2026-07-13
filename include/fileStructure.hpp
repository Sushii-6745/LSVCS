#pragma once
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <memory>
namespace fs = std::filesystem;
namespace fileS{
class FileStructure{
    private:
        std::string filedata="", trimdata="";
        long long int start = -1;
        long long int end = -1;
        struct Node{
                std::string data;
                struct Node* parent;
        };
        std::map<std::string, std::vector<std::unique_ptr<Node>>>dataTree;
        std::unique_ptr<Node> root;
        Node* rootPtr;
        bool _checkformat(std::string filedata){
                long long int cnt_bracket = 0;
                std::string br_seq = "";
                for(auto br : filedata){
                    if(br == '{' || br =='}')
                        br_seq+=br;
                }
                for(auto br : br_seq){
                    if(br == '{') cnt_bracket+=1;
                    else if(br == '}') cnt_bracket-=1;
                    if(cnt_bracket < 0){
                        std::cout << "Error wrong format.\n";
                        return false;
                    }
                }
                start = trimdata.find('{');
                end = trimdata.rfind('}');
                for(int idx = start ; idx <= end ; idx++){
                        if(trimdata[idx]==':'){
                            if(trimdata[idx+1]!='{')
                            {
                                std::cout <<"Error wrong format.\n";
                                return false;
                            }
                        }
                }
                for(int idx = start; idx<=end ; idx++){
                        if(filedata[idx]==':'){
                            if(filedata[idx-1]=='{' || filedata[idx-1]=='}' || filedata[idx-1]==','){
                                std::cout <<"Error wrong format.\n";
                                return false;
                            }
                        }
                }
                for(int idx = start; idx < end ; idx++){
                    if(filedata[idx]=='}'){
                        if(filedata[idx+1]!=','){
                            std::cout <<"Error wrong format comma issue.\n";
                            return false;
                        }
                    }
                }
            return true;
        }
        void _optimize(std::string &filedata){
           long long length = end-start-1;
           std::string trimdata=trimdata.substr(start, length);
        }
        int it = 0;
        void _create_data_tree(Node* node){
                if (trimdata[it] == '{')++it;
                while (it < trimdata.size()){
                    if (trimdata[it] == '}'){
                                 ++it;
                                return;
                    }
            if (trimdata[it] == ',' || trimdata[it] == ':'){
                    ++it;
                    continue;
            }
            if (trimdata[it] == '"'){
                    ++it;
                    std::string obj_name;
                    while (it < trimdata.size() && trimdata[it] != '"')
                        obj_name += trimdata[it++];
                    ++it;
                    auto child = std::make_unique<Node>(Node{obj_name, node});
                    Node* childPtr = child.get();
                    dataTree[node->data].push_back(move(child));
                    if (it < trimdata.size() && trimdata[it] == ':'){
                            ++it;
                            if (it < trimdata.size() && trimdata[it] == '{')
                                    _create_data_tree(childPtr);
                    }
            }
            else{
                    ++it;
            }
        }
    }
    public:
    FileStructure(){
        root = std::make_unique<Node>(Node{".", nullptr});
        rootPtr = root.get();
        }
        bool isParseSuccess = false;
        bool read(std::string path){
            filedata="";
            trimdata="";
                std::ifstream FILE(path);
                if(path.substr(path.size()-4,4)!=".des"){
                        std::cout << "Not my type >~<!\n";
                        return false;
                }
                if(FILE.is_open()){
                    std::stringstream token;
                    std::string word;
                    while(FILE >> word){
                        trimdata+=word;
                    }
                    token << FILE.rdbuf();
                    filedata = token.str();
                    
                    FILE.close();
                }
                else{
                    std::cout << "Unable to parse file\n";
                }
           if( _checkformat(trimdata)==false){
                return isParseSuccess=false;
           }
        //    _optimize(trimdata);
           _create_data_tree(rootPtr);
            return isParseSuccess=true;
        }
        std::map<std::string, std::vector<std::unique_ptr<Node>>>& data(){
             return dataTree;
        }

        void create(std::map<std::string, std::vector<std::string>>&dataset, std::string path){
                    std::ofstream FILE(path);
                    if(FILE.is_open()){
                        std::cout << "File created successfully \n";
                    }
                    std::string root = ".";
                    std::string file_data = "{";
                    auto f = [&](std::string node, auto &&f)->void
                    {
                            if(node!=root)
                            file_data+=":{";
                            for(auto child : dataset[node]){
                                    
                                    std::string child_name = child;
                                    file_data+="\"";
                                    file_data+=child_name;
                                    file_data+="\"";
                                    f(child, f);
                                    
                            }
                            if(node != root)
                            file_data+="},";
                    };
                    f(root, f);
                    file_data+="}";
                    FILE << file_data;
                    std::cout << "Data Written successfully\n";
                    FILE.close();
        }
};
}