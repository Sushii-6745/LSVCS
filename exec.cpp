//This is the start of my project slcs - (Smart Local Control System). Nothing too smart about it now
//Now the first basic step is to able to create a hidden folder which will hold  essentials.
//Folder now we will  be adding three essential text files to this hidden folder.
//Commits , Blobs , Trees and rather than running the program I will bw invoking them using CLI
// Currently the paths have been hardcoded , I will soon make it dynamic. (NEXT TASK).
//Rather than storing a single file , I will storing complete codebase.
//There is a complete change in the mental model. So basically I need ro implement tree object, which contains hashes for blobs, and other subtrees hashes, Then it creaes its own hash on basis of these data, after this the commit hash is created with the help of tree hash+ parent hash + message + Author + TimeStamp. 

//The main challenge will be to read tree hash data and recusively generate old data from blob object
//So, I will need a format to store directory and file structure in a way so that it can be invoked later.
//So either the first option is to create my own format and a code to parse the format on my own, or use an external library.
/*
    So I think I will create my own parser library. If I am unable to create one, I will use an external library.
    I created my parser library, for now only valid for this program will make it valid for normal JSON object.
*/ 
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include "JSONparser.hpp"

#define ACTIVE 1
#define DEACTIVE 0
#define SUCCESS true
#define FAILURE false
using namespace std;
namespace fs = filesystem;
// string objects[1]={"./.LSVCS/tree.txt"};
map<string, int>functions;
string sha256(const string&str){
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(
        reinterpret_cast<const unsigned char*>(str.c_str()),
        str.size(),
        hash
    );
    stringstream ss;
    for(int i = 0 ; i<SHA256_DIGEST_LENGTH; i++){
        ss << hex <<setw(2) <<setfill('0') <<(int)hash[i];
    }
    return ss.str();
}
bool _init(){ //Whenever a initialization is made the tree structure must get initialized with all the directories, sub-direcories and files.

    fs::path single_folder = ".LSVCS";
    try{
        if(fs::create_directory(single_folder)){
            cout << "\033[0;32m***** Version control system intitalized successfully :) ******\033[0m\n";
            fs::path blob_object = ".LSVCS/blob"; //Creating Blob Object
            fs::create_directory(blob_object);
            fs::path tree_object = ".LSVCS/tree";
            fs::create_directory(tree_object); //Creating Tree Object
            // fs::path commit_object = ".LSVCS/commit";
            // fs::create_directory(commit_object); //Creating Commit Object

        }
        else{
            cout <<"\033[0;34mOops! => *** Already in action ;) ***\033[0m\n";
        }
        return SUCCESS;
    }
    catch(const fs::filesystem_error&e){
        cerr << "Oops! => "<<e.what() <<"\n";
        return FAILURE;
    }

}
long long int stringHash(string input){
    long long uniqueKey = 0;
    for(int i = 0 ; i<input.size() ; i--){
        uniqueKey += (input[i]-'a');
        uniqueKey *= 26;
    }
    return uniqueKey/26;
}
void loadFunctions(){
    string FUNCTION_LIST[] = {
        "init", //Done
        "commit", //Done
        "add", //Done
        "log", //Underwork
        "vis", //UnderWork
        "dev", //Left
        "checkout", //Done
        "branch"
    };
    for(auto e: FUNCTION_LIST){
        functions[e] = ACTIVE;
    }
}
bool IsHashNew(string hash){
    //Iterate through throgh every hash until and unless reaching NULL parent
    ofstream HEAD("./.LSVCS/commit/HEAD.txt");
    string currCommit ="";
    if(HEAD.is_open()){
        stringstream token;
        token << HEAD.rdbuf();
        currCommit = token.str();
    }
    HEAD.close();
    string path = "./.LSVCS/commit/";
    if(currCommit == hash){
        return true;
    }
    while(currCommit != "NULL"){
        ifstream FILE(path+currCommit+".txt");
        string token;
        int flag = 1;
        bool countDown = false;
        if(FILE.is_open()){
            while(FILE >> token && flag){
                if(countDown){
                    flag--;
                    currCommit = token;
                }
                if(token == "PARENT_HASH_:") countDown = true;
            }
        }
        if(currCommit == hash) return true;
    }
    return false;

}
string currHeadPosition = "main";
bool _branch(string branchName){
    fs::path branchdir = "./.LSVCS/commit/branch";
    if(!fs::exists(branchdir)){
        fs::create_directory(branchdir);
    }
    ofstream FILE("./.LSVCS/commit/branch/"+branchName+".txt");
    ifstream HEAD("./.LSVCS/commit/HEAD.txt");
    stringstream token;
    token << HEAD.rdbuf();
    FILE << token.str();
    HEAD.close();
    FILE.close();
    return SUCCESS;
}
string changeHead(string branchName){
    ifstream BRANCH("./.LSVCS/commit/branch/"+branchName+".txt");
    stringstream token;
    string currBranch;
    token << BRANCH.rdbuf();
    currBranch = token.str();
    BRANCH.close();
    ofstream HEAD("./.LSVCS/commit/HEAD.txt");
    HEAD << currBranch;
    HEAD.close();
    currHeadPosition = branchName;
    return currBranch;
}
void setCurrHeadPosition(){
        string HEADpos;
        if(fs::exists("./.LSVCS/commit/HEAD.txt")){
        ifstream HEAD("./.LSVCS/commit/HEAD.txt");
        stringstream token;
        token << HEAD.rdbuf();
        HEADpos = token.str();
        }
        else{
            return ;
        }
        fs::path branch = "./.LSVCS/commit/branch";
        if(fs::exists(branch)){
            for(const auto& entry : fs::directory_iterator(branch)){
                ifstream FILE(entry.path().string());
                stringstream branches;
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

fs::path root_dir = ".";
map<string, vector<string>>mp;
void dummy(std::string dirPath){
    if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
            // Iterate non-recursively through the folder
            for (const auto& entry : fs::directory_iterator(dirPath)) {
                std::string type = entry.is_directory() ? "[DIR] " : "[FILE]";
                if(entry.path().string() == "./.LSVCS") continue;
                // std::cout << type << " " << entry.path().string() << "\n";
                mp[dirPath].push_back(entry.path().string());
                if(type == "[FILE]"){
                    ifstream FILE(entry.path().string());
                    stringstream token;
                    token << FILE.rdbuf();
                    string data = sha256(token.str());
                    ofstream BLOBFILE("./.LSVCS/blob/"+data+".txt");
                    BLOBFILE << token.str();
                    FILE.close();
                    BLOBFILE.close();
                    mp[entry.path().string()].push_back("./.LSVCS/blob/"+data+".txt");
                }
                if(type == "[DIR] "){
                    dummy(entry.path().string());
                }
            }
        }
}
string dummy2(){
    dummy(".");
    json::JSONParser parser;
    parser.create(mp, "./.LSVCS/tree/temp.des");
    ifstream FILE("./.LSVCS/tree/temp.des");
    stringstream token;
    token << FILE.rdbuf();
    string treeHash = sha256(token.str());
    FILE.close();
    ofstream TREE("./.LSVCS/tree/"+treeHash+".des");
    TREE << token.str();
    fs::remove("./.LSVCS/tree/temp.des");
    TREE.close();
    return treeHash;
}
bool _commit(string message=""){//Now, it is time to add the message parameter,  so that every commit conveys some message
               //The standard format will be like ./exec commit "Added message feature", so it need to have a starting quote and an ending quote, otherwise invalid.
    // ->Main functionality create a history of the commit
    // ->Actually stores the commit
    // ->Updates the position of HEAD and tree
    /****IMPORTANT FUNCTIONS (to be implemented later)*******
     * Check if there is no issue in commiting.
     * Commit on the right branch
     */
    setCurrHeadPosition();
    string staging_area = "./.LSVCS/stage_area.txt";
    if(!fs::exists(staging_area)){
        cout <<"Files have not been added. :(\n";
        return SUCCESS;
    }
    string treeHash = dummy2();
    struct commit{
        string treeHash;
        string parentHash;
        string commitHash;
        string commitMessage;
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
                ofstream HEAD("./.LSVCS/commit/HEAD.txt");
                _branch("main");
                HEAD.close();
                currCommit.parentHash = "NULL";
            }
        }
        catch(const fs::filesystem_error&e){
            cerr <<"Oops! => "<<e.what() <<"\n";
            return FAILURE;
        }
    }
    if(IsDirExists){
        ifstream HEAD("./.LSVCS/commit/HEAD.txt");
        string prevCommit ="";
        if(HEAD.is_open()){
            stringstream token;
            token << HEAD.rdbuf();
            prevCommit = token.str();
        }
        HEAD.close();
        currCommit.parentHash = prevCommit;
    }

    string CommitHash = currCommit.treeHash+currCommit.parentHash+currCommit.commitMessage;
    currCommit.commitHash = sha256(CommitHash);


    ifstream STAGING_AREA("./.LSVCS/stage_area.txt");
    stringstream token;
    token << STAGING_AREA.rdbuf();
    string StageHash = token.str();
    if(currCommit.treeHash != StageHash){
        cout << "WARNING: Unmodified file, earlier staged data is being commited :(\n";
    }
    if(fs::exists("./.LSVCS/commit/"+currCommit.commitHash+".txt")){
        fs::remove("./.LSVCS/stage_area.txt");
        cout << "Already commited ;)\n";
        return SUCCESS;
    }
    string commitFile = "./.LSVCS/commit/" + currCommit.commitHash +".txt";
    ofstream FILE(commitFile);
    if(FILE.is_open()){
        FILE << "TREE_HASH_: "<<currCommit.treeHash<<"\nHASH_: "<<currCommit.commitHash<<"\n"<<"PARENT_HASH_: "<<currCommit.parentHash<<"\n"<<"MESSAGE: "<<currCommit.commitMessage<<"\n";
        FILE.close();
        ofstream HEAD("./.LSVCS/commit/HEAD.txt");
        HEAD << currCommit.commitHash;
        ofstream BRANCH("./.LSVCS/commit/branch/"+currHeadPosition+".txt");
        BRANCH << currCommit.commitHash;
        BRANCH.close();
        HEAD.close();
        cout <<"Committed successfully\n";
        fs::remove("./.LSVCS/stage_area.txt");
        return SUCCESS;
    }
    else{
        cout <<"Oops! => Error in commit :[\n";
        return FAILURE;
    }
     
}
bool _checkout(string hash){
/*Check-out method in real git helps in changing the HEAD to the latest commit in each of the branch
and If raw commit hash is provided to the function it creates a detached HEAD state and helps in viewing each commit
If using detached HEAD state and creating a change in commit and try to commit changes , the commit is considered as an orphan commit
This orphan commit doesn't belongs to any branch and is then eventually cleaned by Git's Garbage collector. Since I am not planning to develop
a garbage collector soon, the plan will be to avoid commit until current position doesn't matches the latest commit's hash. For now checkout function will be to just check out*/   
        string currhash = "";
        ifstream HEAD("./.LSVCS/commit/HEAD.txt");
        if(HEAD.is_open()){
            stringstream token;
            token << HEAD.rdbuf();
            currhash = token.str();
            HEAD.close();
        }
        string path = "./.LSVCS/commit/";
        ifstream FILE_curr(path+hash+".txt");
        string treeHash = "";
        if(FILE_curr.is_open()){
            string token;
            while(FILE_curr >> token){
                if(token == "TREE_HASH_:") continue;
                else {
                    break;
                }
            }
            treeHash = token;
            // cout << treeHash <<"\n";
            FILE_curr.close();
        }

        else{
            return FAILURE;
        }
        string dirPath = ".";
        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if(entry.path().string() == "./.LSVCS" || entry.path().string() == "." || entry.path().string() =="./exec.cpp" || entry.path().string() == "./JSONparser" || entry.path().string() == "./JSONparser.cpp"|| entry.path().string() == "./JSONparser.hpp" || entry.path().string() == "./exec") continue;
            string command = "rm -rf "+entry.path().string();
            system(command.c_str());
            // cout << entry.path().string() <<"\n";
        }

        json::JSONParser parser;
        string treepath = "./.LSVCS/tree/"+treeHash+".des";
        parser.read(treepath);
        if(parser.isParseSuccess){
            auto &fdata = parser.data();
            map<string, vector<string>>mp;
            for (const auto& [k, v] : fdata){
                vector<string>children;
                for (const auto& p : v){
                    children.push_back(p->data);
                }
                mp[k] = children;
            }
            for(auto [p,e]: mp){
                if(p == "./.LSVCS" || p == "." || p =="./exec.cpp" || p == "./JSONparser" || p == "./JSONparser.cpp"|| p == "./JSONparser.hpp" || p == "./exec") continue;

                if(e[0].substr(0,14) == "./.LSVCS/blob/"){
                    //Confirm file
                    string file_data;
                    ifstream BLOB(e[0]);
                    stringstream token;
                    token << BLOB.rdbuf();
                    file_data = token.str();
                    BLOB.close();
                    ofstream MAIN(p);
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
        //path = "./.LSVCS/blob/"+commit_no+"/"+hash+".txt";
        // ifstream FILE(path);
        // string entireData ="";
        // if(FILE.is_open()){
            // stringstream token;
            // token << FILE.rdbuf();
            // entireData = token.str();
            // FILE.close();
        // }
        // else return FAILURE;
        // 
        // path = "./exec.cpp";
        // ofstream FILE_INPUT(path);
        // if(FILE_INPUT.is_open()){
            // cout << "WARNING : DETACHED HEAD STATE => LATEST HASH: "<<currhash<<" CURRENT HASH: "<<hash <<"\n";
            // FILE_INPUT<< entireData;
            // FILE.close();
        // }
return SUCCESS;

}
bool _vis(){
    bool updateTree = true;
    if(!updateTree){
        return FAILURE;
    }
    ofstream DOT("./.LSVCS/DOT.dot");


    vector<string>branches, branchname;
    if (fs::exists("./.LSVCS/commit/branch") && fs::is_directory("./.LSVCS/commit/branch")) {
        for (const auto& entry : fs::directory_iterator("./.LSVCS/commit/branch")) {
            if (fs::is_regular_file(entry.status())) {
                ifstream DATA(entry.path().string());
                stringstream token;
                token << DATA.rdbuf();
                branches.push_back(token.str());
                branchname.push_back(entry.path().stem().string());
            }
        }
    }


    if(DOT.is_open()){
    DOT << "digraph Git {\n";
    DOT << "node [shape=box];\n";
    size_t idx = -1;
    for(auto e : branches){
        idx++;
        if(branchname[idx] == currHeadPosition){
            DOT <<"\""<<branchname[idx]<<"\"" <<" [shape=ellipse, style=filled, fillcolor=lightblue];\n";
            DOT << "\"" << e.substr(0,7) << "\" [label=\"" << e.substr(0,7) << "\"];\n";
            DOT << "\"" << branchname[idx] << "\" -> \"" << e.substr(0,7) << "\";\n";
        }
        else{
            DOT <<"\""<<branchname[idx]<<"\""<<" [shape=ellipse, style=filled, fillcolor=lightgreen];\n";
            DOT << "\"" << e.substr(0,7) << "\" [label=\"" << e.substr(0,7) << "\"];\n";
            DOT << "\"" << branchname[idx] << "\" -> \"" << e.substr(0,7) << "\";\n";
        }
        string hash = e;
        while(hash!="NULL"){
            ifstream FILE("./.LSVCS/commit/"+hash+".txt");
            string line;
            string parHash = "", message="";
            while (getline(FILE, line)) {
            if(line.substr(0,14)=="PARENT_HASH_: "){
                parHash = line.substr(14);
            }
            if(line.substr(0,9)=="MESSAGE: "){
                message = line.substr(9);
            }
            }
            // DOT << "\"" << hash << "\" "<< "[label=\""<< hash.substr(0,7)<< "\\n"<< message<< "\"];\n";
            if(parHash != "NULL"){
            DOT << "\"" << hash.substr(0,7) << "\" [label=\"" << hash.substr(0,7) << "\\n"<< message<< "\"];\n";
            DOT << "\"" << parHash.substr(0,7) << "\" [label=\"" << parHash.substr(0,7) << "\"];\n";
            DOT << "\"" << hash.substr(0,7) << "\" -> \"" << parHash.substr(0,7) << "\";\n";
            }      
            hash = parHash;
        }
    }
    DOT <<"\"HEAD\" [shape=diamond, style=filled, fillcolor=tomato fontcolor=white];\n";
    DOT <<"\"HEAD\" -> \""<< currHeadPosition << "\";\n";
    DOT << "}\n";
    DOT.close();
    system("dot -Tpng ./.LSVCS/DOT.dot -o ./.LSVCS/DOT.png");
    system("xdg-open ./.LSVCS/DOT.png");
    return SUCCESS;
    }
    else{
        return FAILURE;
    }
}
bool _add(){
    //Initially my code used to directly commit the code base, In this function I will create a staging-area like subsystem, so that commititng of the code base requires adding it to the staging area first, If the staging area is empty then no commits, after adding the codebase to the staging area, if change is done in the original code base a warning will be displayed showing current code base differs from staged codebase, and then the commit will be executed, if there is no modification in the added codebase then it will be committed successfully without any warnings.
    //I will create a text file names staging area, in which I will be storing hash of the data. If staging area is empty, no commits, if staging area hash != commit hash, commit staging area data with WARNINGS, if both matches commit without any warnings.
    //And if add is called two times, data will be overwritten.
    //I can do that tempbuffer.txt function here creating that file in this function, and commit function will reduce to just storing the data and deleting it.

    string stageAreaTreeHash = dummy2(); //This is hardcoded initially, So basically I want to store all of the data in the working directory, but I have to think of a way 
    //to make hash unique for each commit. 
    string path = "./.LSVCS/stage_area.txt";
    if(fs::exists(path)){
        ifstream CHECK(path);
        if(CHECK.is_open()){
                stringstream token;
                token << CHECK.rdbuf();
                if(token.str() == stageAreaTreeHash){
                    cout <<"Already Added ;)\n";
                }
                else{
                    cout << "WARNING : Data Overwritten in staging area.\n";

                }
        }
        else{
            return FAILURE;
        }
    }
    ofstream FILE(path);
    if(FILE.is_open()){
        FILE << stageAreaTreeHash;
        cout << "Added to the staging area.\n";
    }
    return SUCCESS;
}

int main(int argc, char *argv[]){
    bool processSuccess = false;
    vector<string>arguements(argc-1);
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
                processSuccess = _commit(arguements[1]);
                else cout <<"No message parameter\n";
            }
            if(arguements[0] == "checkout"){
                if(arguements.size() == 2)
                processSuccess = _checkout(arguements[1]);
                else if(arguements.size()==3){
                    if(arguements[1]=="-b"){
                        cout <<"Welcome to new branch\n";
                        string newBranchHash = changeHead(arguements[2]);
                        processSuccess = _checkout(newBranchHash);
                    }
                }
                else cout <<"No parameter\n";
                
            }
            if(arguements[0] == "vis")
                processSuccess = _vis();
            if(arguements[0] == "add")
                processSuccess = _add();
            if(arguements[0] == "branch"){
                if(arguements.size() != 1)
                processSuccess = _branch(arguements[1]);
                else cout <<"No parameter\n";
            }
        }
        else cout<<"Currently under maintenance" <<"\n";
    }
    else{
        cout <<"Undefined function"<<"\n";
    }
    if(processSuccess==FAILURE){
        cout << "Not able to initialize "<<"\n";
        return 0;
    }
}
