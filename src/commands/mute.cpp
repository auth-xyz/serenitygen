#include "../headers/commands/mute.hpp"
#include "../headers/libraries/embed_utils.hpp"

#include <dpp/dpp.h>
#include <chrono>
#include <iostream>

dpp::task<void> handle_mute_command(const dpp::slashcommand_t& event) 
{
    event.thinking();

    int64_t duration_minutes = std::get<int64_t>(event.get_parameter("duration"));
    auto now = std::chrono::system_clock::now();
    auto timeout_end = std::chrono::system_clock::to_time_t(now + std::chrono::minutes(duration_minutes));

    dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));
    dpp::cluster* bot = event.from->creator;
    dpp::embed __embed__;
    dpp::embed __errem__;
    dpp::message __mmsg__; // mmsg = moderator message

    auto user_identified = co_await bot->co_user_get(user_id);
    if (user_identified.is_error()) 
    {
        printf("Failed to acknowledge member");
        co_return;
    }

    try 
    {
        dpp::user_identified target_user = std::get<dpp::user_identified>(user_identified.value);
        dpp::user moderator = event.command.usr;
        std::string user_pfp = target_user.get_avatar_url();
        std::string moderator_pfp = moderator.get_avatar_url();

        __embed__ = EmbedUtils::create_moderator_embed("User Muted", "", user_pfp);
        __embed__.add_field("Moderator", moderator.username, true);
        __embed__.add_field("Duration", std::to_string(duration_minutes) + " minute(s)", true);
        __mmsg__.add_embed(__embed__);
        __mmsg__.set_channel_id(1070176260891889725);
        
        auto timeout_result = co_await bot->co_guild_member_timeout(event.command.guild_id, user_id, timeout_end);
        if (timeout_result.is_error())
        {
            dpp::embed p = EmbedUtils::create_error_embed("An error related to timeout_result occurred");
            auto error_msg = co_await bot->co_message_create(dpp::message().add_embed(p));
            printf("err");
            co_return;
        }


        auto __mmsg_result__ = co_await bot->co_message_create(__mmsg__);
        if (__mmsg_result__.is_error())
        {
            dpp::embed p = EmbedUtils::create_error_embed("An error related to __mmsg__ occurred");
            auto error_msg = co_await bot->co_message_create(dpp::message().add_embed(p));
            printf("err");
            co_return;
        } else 
        {
            event.reply("User muted");
            co_return;
        }
        
    } catch (const std::exception& e)
    {
        printf(e.what());
        co_return;
    }
}


