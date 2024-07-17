#include "../headers/commands/mute.hpp"
#include "../headers/libraries/embed_utils.hpp"

#include <dpp/dpp.h>
#include <chrono>
#include <iostream>

dpp::task<void> handle_mute_command(const dpp::slashcommand_t& event) {
    event.thinking();
    // Extract user ID and duration from parameters
    dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));
    int64_t duration_minutes = std::get<int64_t>(event.get_parameter("duration"));

    // Calculate the timeout end time
    auto now = std::chrono::system_clock::now();
    auto timeout_end = std::chrono::system_clock::to_time_t(now + std::chrono::minutes(duration_minutes));

    // Retrieve bot instance from event context
    dpp::cluster* bot = event.from->creator;

    // Fetch target user's details
    try {
        auto user_identified = co_await bot->co_user_get(user_id);

        if (!user_identified.is_error()) {
            dpp::user_identified target_user = std::get<dpp::user_identified>(user_identified.value);
            std::string user_pfp = target_user.get_avatar_url();
            std::string moderator_pfp = event.command.usr.get_avatar_url();

            dpp::embed mod_embed = EmbedUtils::create_basic_embed("User " + target_user.username + " was Muted", "Goof got the :shh: treatment", 0xFFFF00, user_pfp);
            mod_embed.add_field("Moderator", event.command.usr.username, true);
            mod_embed.add_field("Duration", std::to_string(duration_minutes) + " minute(s)", true);

            // Perform the mute operation (timeout)
            auto timeout_result = co_await bot->co_guild_member_timeout(event.command.guild_id, user_id, timeout_end);

            if (timeout_result.is_error()) {
                event.edit_response("Failed to mute the user.");
                std::cerr << "Failed to mute user. Error: " << timeout_result.get_error().message << std::endl;
                co_return;
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
                    // Reply to the user who initiated the command
                    event.edit_response("User has been muted.");
                }
            }
        } else {
            event.reply("Failed to retrieve user details.");
            std::cerr << "Failed to retrieve user details. Error: " << user_identified.get_error().message << std::endl;
        }
    } catch (const std::exception& e) {
        event.reply("An error occurred: " + std::string(e.what()));
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}


