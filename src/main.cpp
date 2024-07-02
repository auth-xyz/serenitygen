#include <iostream>
#include <unordered_map>
#include <dpp/dpp.h>

#include "libs/env_reader.h"
#include "libs/command_registry.h"

// Commands
#include "commands/embed.h"
#include "commands/author.h"
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
  cmd_reg.register_command("author", handle_author_command);

  bot.on_slashcommand([&cmd_reg](const dpp::slashcommand_t& event){
    cmd_reg.handle_command(event);
  });

  bot.on_ready([&bot](const dpp::ready_t& event){
    dpp::snowflake guild_id = 1070169312284917860; 

    if (dpp::run_once<struct register_bot_commands>()) {
      bot.guild_command_create(
        dpp::slashcommand("embed", "eijfwia", bot.me.id),
        guild_id
      );
      bot.guild_command_create(
        dpp::slashcommand("author", "shows bot author description", bot.me.id),
        guild_id
      );
    }


  });

  bot.start(dpp::st_wait);
}
