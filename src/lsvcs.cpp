#include "Utils.hpp"
#include "Commit.hpp"
#include "Staging.hpp"
#include "Blob.hpp"
#include "Graphviz.hpp"
#include "SHA256.hpp"
#include "Branch.hpp"
#include "Checkout.hpp"
Commit commitObj;
Checkout checkout;
Vis viz;
Staging staging;
std::string currHeadPosition = "main";
std::map<std::string, std::vector<std::string>>mp;
std::map<std::string, int>functions;
std::string changeHead(std::string branchName){
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
void loadFunctions(){
    std::string FUNCTION_LIST[] = {
        "init",
        "commit",
        "add",
        "vis",
        "checkout",
        "branch"
    };
    for(auto e: FUNCTION_LIST){
        functions[e] = ACTIVE;
    }
}
bool _init(){ //Whenever a initialization is made the tree structure must get initialized with all the directories, sub-direcories and files.

    fs::path single_folder = ".LSVCS";
    try{
        if(fs::create_directory(single_folder)){
            std::cout << "\033[0;32m***** Version control system intitalized successfully :) ******\033[0m\n";
            fs::path blob_object = ".LSVCS/blob"; //Creating Blob Object
            fs::create_directory(blob_object);
            fs::path tree_object = ".LSVCS/tree";
            fs::create_directory(tree_object); //Creating Tree Object
            // fs::path commit_object = ".LSVCS/commit";
            // fs::create_directory(commit_object); //Creating Commit Object

        }
        else{
            std::cout <<"\033[0;34mOops! => *** Already in action ;) ***\033[0m\n";
        }
        return SUCCESS;
    }
    catch(const fs::filesystem_error&e){
        std::cerr << "Oops! => "<<e.what() <<"\n";
        return FAILURE;
    }

}
int main(int argc, char *argv[]){
    bool processSuccess = false;
    std::vector<std::string>arguements(argc-1);
    for(int i = 1 ; i<argc ; i++){
        arguements[i-1] = argv[i];
    }
    loadFunctions();
    if(functions.find(arguements[0])!=functions.end()){
        if(functions[arguements[0]] == ACTIVE){
            if(arguements[0] == "init")
                processSuccess = _init();
            if(arguements[0] == "commit"){
                if(arguements.size() != 1)
                
                processSuccess = commitObj.commitit(arguements[1]);
                else std::cout <<"No message parameter\n";
            }
            if(arguements[0] == "checkout"){
                if(arguements.size() == 2)
                processSuccess = checkout.checkit(arguements[1]);
                else if(arguements.size()==3){
                    if(arguements[1]=="-b"){
                        std::cout <<"Welcome to new branch\n";
                        std::string newBranchHash = changeHead(arguements[2]);
                        processSuccess = checkout.checkit(newBranchHash);
                    }
                }
                else std::cout <<"No parameter\n";
                
            }
            if(arguements[0] == "vis")
                processSuccess = viz.showTree();
            if(arguements[0] == "add")
                processSuccess = staging.add();
            if(arguements[0] == "branch"){
                if(arguements.size() != 1)
                processSuccess = createBranch(arguements[1]);
                else std::cout <<"No parameter\n";
            }
        }
        else std::cout<<"Currently under maintenance" <<"\n";
    }
    else{
        std::cout <<"Undefined function"<<"\n";
    }
    if(processSuccess==FAILURE){
        std::cout << "Not able to initialize "<<"\n";
        return 0;
    }
}