#include <dpp/dpp.h>
#include <chrono>

#include "../headers/commands/unmute.hpp"
#include "../headers/libraries/embed_utils.hpp"

dpp::task<void> handle_unmute_command(const dpp::slashcommand_t& event) 
{
    event.co_thinking();

    dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));
    dpp::cluster* bot = event.from->creator;

    auto user_identified = co_await bot->co_user_get(user_id);
    if (user_identified.is_error()) 
    {
        std::cerr << "Failed to fetch target id" << std::endl;
        event.co_edit_response("s");
        co_return;
    }

    dpp::user_identified target_user = std::get<dpp::user_identified>(user_identified.value);
    dpp::user moderator = event.command.usr;

    std::string target_pfp = target_user.get_avatar_url();
    std::string moderator_pfp = moderator.get_avatar_url();

    dpp::embed __embed__ = EmbedUtils::create_public_embed("User unmuted", "**User:** " + target_user.username + "\n**Moderator:** " + moderator.username, target_pfp);
    dpp::message __msg__;

    __msg__.set_channel_id(1070176260891889725);
    __msg__.add_embed(__embed__);

    try 
    {
        auto unmute_result = co_await bot->co_guild_member_timeout(event.command.guild_id, user_id, 0);
        if (unmute_result.is_error()) 
        {
            printf("Failed to apply timeout to targetted user");
            event.co_edit_response("Failed to mute user");
            co_return;
        }
    } catch (const std::exception& e) 
    {
        printf(e.what());
        event.co_edit_response("An error occurred while processing the command");
        co_return;
    }
}