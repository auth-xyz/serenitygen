#include "../headers/commands/unban.hpp"
#include "../headers/libraries/embed_utils.hpp"

#include <dpp/dpp.h>

dpp::task<void> handle_unban_command(const dpp::slashcommand_t& event) {
    // Get the user ID from the parameters
    dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));

    // Check if the event.from or event.from->creator is null
    if (!event.from || !event.from->creator) {
        event.reply("Bot instance not found.");
        co_return;
    }

    dpp::cluster* bot = event.from->creator;

    // Unban the user
    try {
        auto unban_result = co_await bot->co_guild_ban_delete(event.command.guild_id, user_id);

        if (unban_result.is_error()) {
            event.reply("Failed to unban the user.");
            std::cerr << "Failed to unban user. Error: " << unban_result.get_error().message << std::endl;
            co_return;
        }

        // Create the embed
        std::string moderator_pfp = event.command.usr.get_avatar_url();

        dpp::embed unban_embed = EmbedUtils::create_public_embed("User " + std::to_string(user_id) + " was unbanned", "A soul was freed from hell", moderator_pfp);
        unban_embed.add_field("Moderator", event.command.usr.username, true);

        // Send the embed to the moderator channel
        dpp::message mod_message;
        mod_message.set_channel_id(1070176260891889725); // Private channel ID
        mod_message.add_embed(unban_embed);

        auto mod_message_result = co_await bot->co_message_create(mod_message);

        if (mod_message_result.is_error()) {
            event.reply("Failed to send mod message.");
            std::cerr << "Failed to send mod message. Error: " << mod_message_result.get_error().message << std::endl;
        }

        // Send a reply indicating success
        event.reply("User has been unbanned.");
    } catch (const std::exception& e) {
        event.reply("An error occurred: " + std::string(e.what()));
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}
