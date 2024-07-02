#include <iostream>
#include <unordered_map>
#include "env_reader.h"
#include <dpp/dpp.h>

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

  bot.on_slashcommand([](const dpp::slashcommand_t& event){
    if (event.command.get_command_name() == "ping") {
      event.reply("pong!");
    }
  });

  bot.on_ready([&bot](const dpp::ready_t& event){
    if (dpp::run_once<struct register_bot_commands>()) {
      bot.global_command_create(dpp::slashcommand("ping", "Ping pong", bot.me.id));
    }
  });

  bot.start(dpp::st_wait);
  
  //  return 0;
}


