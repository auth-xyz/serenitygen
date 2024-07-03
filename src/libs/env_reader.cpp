#include <fstream>
#include <sstream>
#include <string>
#include <iostream> // Include <iostream> for std::cerr
#include <unordered_map>
#include "env_reader.h"

std::unordered_map<std::string, std::string> parseEnvFile(const std::string& filename) {
    std::unordered_map<std::string, std::string> envMap;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl; 
        return envMap;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            envMap[key] = value;
        }
    }

    file.close();
    return envMap;
}

