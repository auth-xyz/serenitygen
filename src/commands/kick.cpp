#include "../headers/commands/kick.hpp"
#include "../headers/libraries/embed_utils.hpp"

#include <dpp/dpp.h>

#include "../headers/commands/kick.hpp"
#include "../headers/libraries/embed_utils.hpp"

#include <dpp/dpp.h>

dpp::task<void> handle_kick_command(const dpp::slashcommand_t& event) {
    try {
        event.thinking();

        dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));
        dpp::cluster* bot = event.from->creator;

        auto user_identified = co_await bot->co_user_get(user_id);

        if (user_identified.is_error()) {
            event.co_edit_response("User not found");
            co_return;
        }

        dpp::user_identified target_user = std::get<dpp::user_identified>(user_identified.value);
        dpp::user moderator = event.command.usr;

        std::string target_pfp = target_user.get_avatar_url();
        std::string moderator_pfp = moderator.get_avatar_url();

        dpp::embed __embed__ = EmbedUtils::create_basic_embed("User " + target_user.username + " was Kicked", "Goof got the boot", 0xFF0000, target_pfp);
        __embed__.add_field("Moderator", moderator.username, true);

        dpp::message __message__;
        __message__.set_channel_id(1070176260891889725);
        __message__.add_embed(__embed__);

        auto mod_message_result = co_await bot->co_message_create(__message__);
        auto kick_result = co_await bot->co_guild_member_kick(event.command.guild_id, user_id);

        if (kick_result.is_error()) {
            event.co_edit_response("Failed to kick the user");
            co_return;
        }

        if (mod_message_result.is_error()) {
            event.co_edit_response("Couldn't send embed to mod channel");
            std::cerr << "Failed to send embed to private mod channel\n";
            co_return;
        }

        event.edit_response("User successfully kicked.");
        co_return;
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        event.co_edit_response("An error occurred while processing the command.");
        co_return;
    }
}

