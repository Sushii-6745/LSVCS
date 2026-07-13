#include "Graphviz.hpp"
#include "Utils.hpp"
bool Vis::showTree(){ //Function implementation to show a graphical commit history tree structure.
    std :: ofstream DOT("./.LSVCS/DOT.dot");
    std::vector<std :: string>branches, branchname;
    if (fs::exists("./.LSVCS/commit/branch") && fs::is_directory("./.LSVCS/commit/branch")) {
        for (const auto& entry : fs::directory_iterator("./.LSVCS/commit/branch")) {
            if (fs::is_regular_file(entry.status())) {
                std::ifstream DATA(entry.path().string());
                std::stringstream token;
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
        std::string hash = e;
        while(hash!="NULL"){
            std::ifstream FILE("./.LSVCS/commit/"+hash+".txt");
            std::string line;
            std::string parHash = "", message="";
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