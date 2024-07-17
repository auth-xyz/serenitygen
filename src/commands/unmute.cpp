#include <dpp/dpp.h>
#include <chrono>

#include "../headers/commands/unmute.hpp"
#include "../headers/libraries/embed_utils.hpp"

dpp::task<void> handle_unmute_command(const dpp::slashcommand_t& event) {
    // Acknowledge the interaction to avoid "Application did not respond" error
    event.thinking();

    dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));

    // Retrieve bot instance from event context
    dpp::cluster* bot = event.from->creator;

    // Fetch target user's details
    try {
        auto user_identified = co_await bot->co_user_get(user_id);

        if (user_identified.is_error()) {
            event.edit_response("Failed to retrieve user details.");
            co_return;
        }

        dpp::user_identified target_user = std::get<dpp::user_identified>(user_identified.value);
        std::string user_pfp = target_user.get_avatar_url();
        std::string moderator_pfp = event.command.usr.get_avatar_url();

        dpp::embed mod_embed = EmbedUtils::create_basic_embed("User " + target_user.username + " was Unmuted", "his soul was spared", 0x00FF00, user_pfp);
        mod_embed.add_field("Moderator", event.command.usr.username, true);

        // Unmute the user
        auto unmute_result = co_await bot->co_guild_member_timeout(event.command.guild_id, user_id, 0);

        if (unmute_result.is_error()) {
            event.edit_response("Failed to unmute the user.");
            std::cerr << "Failed to unmute user. Error: " << unmute_result.get_error().message << std::endl;
        } else {
            // Send embed to moderator channel
            dpp::message mod_message;
            mod_message.set_channel_id(1070176260891889725); // Private channel ID
            mod_message.add_embed(mod_embed);

            auto mod_message_result = co_await bot->co_message_create(mod_message);

            if (mod_message_result.is_error()) {
                event.edit_response("Failed to send mod message.");
                std::cerr << "Failed to send mod message. Error: " << mod_message_result.get_error().message << std::endl;
            } else {
                // Edit the initial response to indicate the user has been unmuted
                event.edit_response("User has been unmuted.");
            }
        }
    } catch (const std::exception& e) {
        event.edit_response("An error occurred: " + std::string(e.what()));
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}
