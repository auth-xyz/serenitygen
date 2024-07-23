#include "../headers/commands/unban.hpp"
#include "../headers/libraries/embed_utils.hpp"

#include <dpp/dpp.h>

dpp::task<void> handle_unban_command(const dpp::slashcommand_t& event) {
  dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));
  dpp::cluster* bot = event.from->creator;
  dpp::user moderator = event.command.usr;
  dpp::embed __embed__;
  dpp::message __msg__;

  std::string moderator_pfp = moderator.get_avatar_url();  

  try 
  {
    auto user_identified = co_await bot->co_user_get(user_id);
    auto unban_result = co_await bot->co_guild_ban_delete(event.command.guild_id, user_id);
    auto message_result = co_await bot->co_message_create(__msg__);
    if (user_identified.is_error())
    {
      printf("Failed to identify target");
      co_return;
    }
    if (message_result.is_error()) 
    {
      printf("Failed to send embed");
      co_return;
    }
    if (unban_result.is_error())
    {
      printf("Failed to acknowledge ban in guild");
      co_return;
    }

    dpp::user_identified target_user = std::get<dpp::user_identified>(user_identified.value);
    std::string target_pfp = target_user.get_avatar_url();

    __embed__ = EmbedUtils::create_public_embed("User unmuted", "**User:** " + target_user.username + "\n**Moderator:** " + moderator.username, target_pfp);
    __msg__.set_channel_id(1070176260891889725);
    __msg__.add_embed(__embed__);
  } catch(const std::exception& e) 
  {
    printf(e.what());
    co_return;
  }
}
