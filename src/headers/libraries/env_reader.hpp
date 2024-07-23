#pragma once
#include <string>
#include <unordered_map>

std::unordered_map<std::string, std::string> parseEnvFile(const std::string& filename);