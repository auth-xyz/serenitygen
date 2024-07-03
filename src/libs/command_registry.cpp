#include "../headers/command_registry.h"

void CommandRegistry::register_command(const std::string& name, const CommandHandler& handler) {
    commands[name] = handler;
}

void CommandRegistry::handle_command(const dpp::slashcommand_t& event) {
    auto it = commands.find(event.command.get_command_name());
    if (it != commands.end()) {
        it->second(event);
    } else {
        event.reply("Unknown command!");
    }
}

