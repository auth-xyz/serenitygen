#ifndef ENV_READER_H
#define ENV_READER_H

#include <string>
#include <unordered_map>

std::unordered_map<std::string, std::string> parseEnvFile(const std::string& filename);

#endif // ENV_READER_H

