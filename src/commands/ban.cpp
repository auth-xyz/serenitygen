#include "../headers/commands/ban.hpp"
#include "../headers/libraries/embed_utils.hpp"
#include <dpp/dpp.h>

dpp::task<void> handle_ban_command(const dpp::slashcommand_t& event) {
    try
    {
        // Check if the user has BAN_MEMBERS permission
        dpp::permission perms = event.command.app_permissions;
        if (!(perms & dpp::p_ban_members)) {
            std::cout << "Attempt to ban failed" << std::endl;
            event.co_reply("You do not have permission to ban members.");
            co_return;
        }

        event.co_thinking();
        dpp::cluster* bot = event.from->creator;
        dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));
        std::string reason = (event.get_parameter("reason").index() != std::variant_npos)
            ? std::get<std::string>(event.get_parameter("reason"))
            : "No reason provided";
        
        auto user_identified = co_await bot->co_user_get(user_id);

        if (user_identified.is_error()) 
        {
            event.co_edit_response("User not found");
            co_return;
        }

        dpp::user_identified target_user = std::get<dpp::user_identified>(user_identified.value);
        dpp::user moderator = event.command.usr;

        std::string target_pfp = target_user.get_avatar_url();
        std::string moderator_pfp = moderator.get_avatar_url();

        auto ban_result = co_await bot->co_guild_ban_add(event.command.guild_id, user_id);

        if (ban_result.is_error()) 
        {
            event.co_edit_response("Failed to ban the user");
            co_return;
        }

        dpp::embed __embed__ = EmbedUtils::create_moderator_embed("User " + target_user.username + " was banned", "**Moderator: **" + moderator.username, target_pfp);
        dpp::message __message__;
        
        __embed__.add_field("Reason", reason, true);
        __message__.set_channel_id(1070176260891889725);
        __message__.add_embed(__embed__);

        auto mod_message_result = co_await bot->co_message_create(__message__);

        if (mod_message_result.is_error()) 
        {
            event.co_edit_response("User successfully banned, but failed to send Embed to mod channel");
            co_return;
        }
        
        event.co_edit_response("User successfully banned");
        co_return;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        event.co_edit_response("An error occurred while processing the command.\nContact a developer.");
        co_return;
    }
}

