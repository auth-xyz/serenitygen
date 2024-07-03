#include <iostream>
#include <unordered_map>
#include <dpp/dpp.h>

#include "headers/env_reader.h"
#include "headers/command_registry.h"

// Commands
#include "headers/embed.h"
#include "headers/ban.h"
#include "headers/kick.h"
//

std::string envfile() {
  std::string envFilePath = "../.env";
  std::unordered_map<std::string, std::string> env = parseEnvFile(envFilePath);

  std::string bot_token = env["DISCORD_TOKEN"];

  return bot_token;
}

const std::string BOT_TOKEN = envfile();

int main() {
  dpp::cluster bot(BOT_TOKEN);
  bot.on_log(dpp::utility::cout_logger());
  
  CommandRegistry cmd_reg;
  cmd_reg.register_command("embed", handle_embed_command);
  cmd_reg.register_command("ban", handle_ban_command);
  cmd_reg.register_command("kick", handle_kick_command);

  bot.on_slashcommand([&cmd_reg](const dpp::slashcommand_t& event){
    cmd_reg.handle_command(event);
  });

  bot.on_ready([&bot](const dpp::ready_t& event){
    dpp::snowflake guild_id = 1070169312284917860;
    bot.global_bulk_command_delete();

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
              .add_option(dpp::command_option(dpp::co_user, "user", "user to kick", true))
              .add_option(dpp::command_option(dpp::co_string, "reason", "the reasoning for your actions.", true)),
               guild_id
          );
    }

  });

  bot.start(dpp::st_wait);
}
