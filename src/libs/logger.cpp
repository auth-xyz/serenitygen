#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

#include "../headers/logger.h"

void log_message(const std::string& user, const std::string& message) {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm* now_tm = std::localtime(&now_time);

  std::cout << "[" << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S") << "] [" << user << "]: " << message << std::endl;
}
