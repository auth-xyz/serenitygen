#include "../headers/command_registry.h"

void CommandRegistry::register_command(const std::string& name, const CommandHandler& handler) {
improved_cmdreg
    auto it = commands.find(name);
    if (it != commands.end()) {
        // Command with the same name already exists
        throw std::runtime_error("Command already registered: " + name);
    }
    commands[name] = handler;
}

void CommandRegistry::handle_command(const dpp::slashcommand_t& event) const {
    auto it = commands.find(event.command.get_command_name());
    if (it != commands.end()) {
        it->second(event);
    } else {
        event.reply("Unknown command!");
    }

  commands[name] = handler;
}