#include "../headers/commands/mute.hpp"
#include "../headers/libraries/embed_utils.hpp"

#include <dpp/dpp.h>
#include <chrono>
#include <iostream>

using namespace std::chrono;

dpp::task<void> handle_mute_command(const dpp::slashcommand_t& event) {
    try {
        // Check if the 'user' and 'duration' parameters exist
        if (event.get_parameter("user").index() == std::variant_npos || event.get_parameter("duration").index() == std::variant_npos) {
            event.reply("Please mention a user to mute and specify the duration.");
            co_return;
        }

        std::cout << "Parameters exist." << std::endl;

        // Extract user ID and duration from parameters
        const auto user_id = std::get<dpp::snowflake>(event.get_parameter("user"));
        const int64_t duration_minutes = std::get<int64_t>(event.get_parameter("duration"));

        std::cout << "User ID: " << user_id << ", Duration: " << duration_minutes << " minutes" << std::endl;

        // Calculate the timeout end time
        const auto now = system_clock::now();
        const auto timeout_end = system_clock::to_time_t(now + minutes(duration_minutes));

        // Retrieve bot instance from event context
        dpp::cluster* bot = event.from->creator;

        // Fetch target user's details
        const dpp::confirmation_callback_t user_result = co_await bot->co_user_get(user_id);
        if (user_result.is_error()) {
            event.reply("Failed to retrieve user details.");
            co_return;
        }

        dpp::user_identified target_user = std::get<dpp::user_identified>(user_result.value);
        std::string user_pfp = target_user.get_avatar_url();
        std::string moderator_pfp = event.command.usr.get_avatar_url();

        std::cout << "Fetched user details." << std::endl;

        dpp::embed mod_embed = EmbedUtils::create_basic_embed("User " + target_user.username + " was Muted", "Got the :shh: treatment", 0xFFFF00, user_pfp);
        mod_embed.add_field("Moderator", event.command.usr.username, true);
        mod_embed.add_field("Duration", std::to_string(duration_minutes) + " minute(s)", true);

        // Perform the mute operation (timeout)
        if (dpp::confirmation_callback_t timeout_result = co_await bot->co_guild_member_timeout(event.command.guild_id, user_id, timeout_end); timeout_result.is_error()) {
            event.edit_response("Failed to mute the user.");
            co_return;
        }

        std::cout << "User muted." << std::endl;

        // Send embed to moderator channel
        dpp::message mod_message;
        mod_message.set_channel_id(1070176260891889725); // Private channel ID
        mod_message.add_embed(mod_embed);

        dpp::confirmation_callback_t mod_message_result = co_await bot->co_message_create(mod_message);
        if (mod_message_result.is_error()) {
            event.edit_response("Failed to send mod message.");
            co_return;
        }

        std::cout << "Moderator message sent." << std::endl;

        // Reply to the user who initiated the command
        event.edit_response("User has been muted.");
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        event.edit_response("An error occurred while processing the command.");
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        event.edit_response("An unknown error occurred while processing the command.");
    }

    co_return;
}

