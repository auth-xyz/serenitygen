#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <filesystem> // For checking if file exists

#include "../headers/env_reader.h"

// Function to parse a dotenv file and return the environment variables as a map
std::unordered_map<std::string, std::string> parseEnvFile(const std::string& filename) {
    std::unordered_map<std::string, std::string> envMap;

    // Check if the file exists
    if (!std::filesystem::exists(filename)) {
        std::cerr << "File does not exist: " << filename << std::endl;
        return envMap;
    }

    std::ifstream file(filename);

    // Check if the file could be opened
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return envMap;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines or lines starting with a comment (#)
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string key, value;

        // Split the line at the first '=' character into key and value
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            // Trim whitespace from key and value
            key.erase(0, key.find_first_not_of(" \t\n\r"));
            key.erase(key.find_last_not_of(" \t\n\r") + 1);
            value.erase(0, value.find_first_not_of(" \t\n\r"));
            value.erase(value.find_last_not_of(" \t\n\r") + 1);

            // Check for invalid keys (empty key after trimming)
            if (key.empty()) {
                std::cerr << "Invalid line in file: " << line << std::endl;
                continue;
            }

            envMap[key] = value;
        } else {
            // Log an error for invalid lines that don't contain an '='
            std::cerr << "Invalid line in file: " << line << std::endl;
        }
    }

    file.close();
    return envMap;
}

