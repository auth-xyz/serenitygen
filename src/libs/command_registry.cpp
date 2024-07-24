#include "../headers/libraries/command_registry.hpp"

void CommandRegistry::register_command(const std::string& name, const CommandHandler& handler) {
    auto it = commands.find(name);
    if (it != commands.end()) {
        // Command with the same name already exists
        throw std::runtime_error("Command already registered: " + name);
    }
    commands[name] = handler;
}

dpp::task<void> CommandRegistry::handle_command(const dpp::slashcommand_t& event) const {
    auto it = commands.find(event.command.get_command_name());
    if (it != commands.end()) {
        co_await it->second(event);
    } else {
        event.co_reply("Unknown command!");
    }
}
