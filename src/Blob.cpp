#include "Blob.hpp"
#include "headers.hpp"
std::string Blob::createBlob(const std::string& path){ //Function implementation to create Blob Objects.
     std::ifstream FILE(path);
                std::stringstream token;
                token << FILE.rdbuf();
                std::string data = sha256(token.str());
     std::ofstream BLOBFILE("./.LSVCS/blob/"+data+".txt");
                BLOBFILE << token.str();
                FILE.close();
                BLOBFILE.close();
    return data;
}