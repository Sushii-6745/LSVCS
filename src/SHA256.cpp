#include "SHA256.hpp"
#include "Utils.hpp"
std::string sha256(const std::string &str){ //Function implementation to encrypt string via SHA256 Encryption algorithm.
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(
        reinterpret_cast<const unsigned char*>(str.c_str()),
        str.size(),
        hash
    );
    std::stringstream ss;
    for(int i = 0 ; i<SHA256_DIGEST_LENGTH; i++){
        ss << std::hex <<std::setw(2) <<std::setfill('0') <<(int)hash[i];
    }
    return ss.str();
}