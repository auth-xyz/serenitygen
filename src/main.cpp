#include <iostream>
#include <unordered_map>
#include <dpp/dpp.h>

#include "headers/logger.h"
#include "headers/env_reader.h"
#include "headers/command_registry.h"

// Commands
#include "headers/embed.h"
#include "headers/ban.h"
#include "headers/kick.h"
#include "headers/unban.h"
#include "headers/mute.h"
#include "headers/unmute.h"
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
      bot.guild_command_create(
        dpp::slashcommand("embed", "test command", bot.me.id),
        guild_id
      );
      bot.guild_command_create(
        dpp::slashcommand("ban", "Bans specified user", bot.me.id)
        .add_option(dpp::command_option(dpp::co_user, "user", "user to ban", true))
        .add_option(dpp::command_option(dpp::co_string, "reason", "the reasoning for your actions.", true)),
        guild_id
      );
      bot.guild_command_create(
        dpp::slashcommand("kick", "Kicks a specified user", bot.me.id)
        .add_option(dpp::command_option(dpp::co_user, "user", "user to kick", true)),
        guild_id
      );
      bot.guild_command_create(
          dpp::slashcommand("unban", "Unbans user", bot.me.id)
          .add_option(dpp::command_option(dpp::co_user, "user", "user to unban", true)),
          guild_id
      );
      bot.guild_command_create(
          dpp::slashcommand("mute", "mutes a user", bot.me.id)
          .add_option(dpp::command_option(dpp::co_user, "user", "user to mute", true))
          .add_option(dpp::command_option(dpp::co_integer, "duration", "<minutes only>", true)),
          guild_id
      );
      bot.guild_command_create(
          dpp::slashcommand("unmute", "unmutes a user", bot.me.id)
          .add_option(dpp::command_option(dpp::co_user, "user", "user to mute", true)),
          guild_id
      );
    }

  });

  bot.start(dpp::st_wait);
}
