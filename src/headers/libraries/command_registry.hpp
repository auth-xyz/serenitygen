#pragma once

#include <dpp/dpp.h>
#include <unordered_map>
#include <string>
#include <functional>

class CommandRegistry {
public:
    using CommandHandler = std::function<dpp::task<void>(const dpp::slashcommand_t&)>;

    void register_command(const std::string& name, const CommandHandler& handler);
    dpp::task<void> handle_command(const dpp::slashcommand_t& event) const;

private:
    std::unordered_map<std::string, CommandHandler> commands;
};

