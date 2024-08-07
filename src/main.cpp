#include <iostream>
#include <unordered_map>
#include <dpp/dpp.h>

#include "headers/libraries/logger.hpp"
#include "headers/libraries/env_reader.hpp"
#include "headers/libraries/command_registry.hpp"

// Commands
#include "headers/commands/embed.hpp"
#include "headers/commands/ban.hpp"
#include "headers/commands/kick.hpp"
#include "headers/commands/unban.hpp"
#include "headers/commands/mute.hpp"
#include "headers/commands/unmute.hpp"
//

std::pair<std::string, std::string> envfile() {
  std::string envFilePath = "../.env";
  std::unordered_map<std::string, std::string> env = parseEnvFile(envFilePath);

  if (env.find("DISCORD_TOKEN") == env.end()) {
    throw std::runtime_error("DISCORD_TOKEN not found in .env");
  }

  std::string bot_token = env["DISCORD_TOKEN"];
  std::string guild_id = env["GUILD_ID"];

  return std::make_pair(bot_token, guild_id);
}

const std::pair<std::string, std::string> result = envfile();

int main() {
  std::string BOT_TOKEN = result.first;
  std::string GUILD_ID = result.second;
  dpp::cluster bot(BOT_TOKEN);
  bot.on_log(dpp::utility::cout_logger());

  CommandRegistry cmd_reg;
  cmd_reg.register_command("embed", handle_embed_command);

  cmd_reg.register_command("ban", handle_ban_command);
  cmd_reg.register_command("unban", handle_unban_command);
  cmd_reg.register_command("kick", handle_kick_command);
  cmd_reg.register_command("mute", handle_mute_command);
  cmd_reg.register_command("unmute", handle_unmute_command);

  bot.on_slashcommand([&cmd_reg](const dpp::slashcommand_t& event){
    std::string command_name = event.command.get_command_name();
    std::string username = event.command.usr.username;

    log_message(username, "used " + command_name + " command");

    cmd_reg.handle_command(event);
  });

  bot.on_ready([&bot, GUILD_ID](const dpp::ready_t& event) {
    if (dpp::run_once<struct clear_bot_commands>()) { bot.global_bulk_command_delete(); }

    dpp::snowflake guild_id = GUILD_ID;
    log_message("INFO", "Serenity successfully logged in.");

    if (dpp::run_once<struct register_bot_commands>()) {
      std::vector<dpp::slashcommand> commands = {
        dpp::slashcommand("embed", "test command", bot.me.id),
        dpp::slashcommand("ban", "Bans the provided user", bot.me.id)
          .add_option(dpp::command_option(dpp::co_user, "user", "user to ban", true))
          .add_option(dpp::command_option(dpp::co_string, "reason", "the reasoning for such actions", true)),
        dpp::slashcommand("unban", "Unbans the provided user", bot.me.id)
          .add_option(dpp::command_option(dpp::co_user, "user", "user to forgive", true)),
        dpp::slashcommand("mute", "mutes the provided user", bot.me.id)
          .add_option(dpp::command_option(dpp::co_user, "user", "user to shut up", true))
          .add_option(dpp::command_option(dpp::co_integer, "duration", "duration in minutes", true)),
        dpp::slashcommand("unmute", "unmutes the provided user", bot.me.id)
          .add_option(dpp::command_option(dpp::co_user, "user", "user to unmute", true)),
        dpp::slashcommand("kick", "kicks the provided user", bot.me.id)
          .add_option(dpp::command_option(dpp::co_user, "user", "user to kick", true)),
      };

      bot.guild_bulk_command_create(commands, guild_id);
    }

  });

  bot.start(dpp::st_wait);
}
