#ifndef COMMANDREGISTRY_H
#define COMMANDREGISTRY_H

#include <dpp/dpp.h>
#include <unordered_map>
#include <functional>

using CommandHandler = std::function<void(const dpp::slashcommand_t& event)>;

class CommandRegistry {
public:
    void register_command(const std::string& name, const CommandHandler& handler);
    void handle_command(const dpp::slashcommand_t& event);

private:
    std::unordered_map<std::string, CommandHandler> commands;
};

#endif

