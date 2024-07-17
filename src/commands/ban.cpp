#include "../headers/commands/ban.hpp"
#include "../headers/libraries/embed_utils.hpp"

#include <dpp/dpp.h>

dpp::task<void> handle_ban_command(const dpp::slashcommand_t& event) {
   // Get the user ID and reason from the parameters
    dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));
    std::string reason = (event.get_parameter("reason").index() != std::variant_npos)
        ? std::get<std::string>(event.get_parameter("reason"))
        : "No reason provided";

    // Check if the event.from or event.from->creator is null
    if (!event.from || !event.from->creator) {
        event.reply("Bot instance not found.");
        co_return;
    }

    dpp::cluster* bot = event.from->creator;

    // Fetch the target user's details
    try {
        auto user_identified = co_await bot->co_user_get(user_id);

        if (!user_identified.is_error()) {
            dpp::user_identified target_user = std::get<dpp::user_identified>(user_identified.value);
            std::string user_pfp = target_user.get_avatar_url();
            std::string moderator_pfp = event.command.usr.get_avatar_url();

            // Create the embeds
            dpp::embed mod_embed = EmbedUtils::create_moderator_embed("User Banned", reason, user_pfp);
            mod_embed.add_field("Moderator", event.command.usr.username, true);
            mod_embed.add_field("User ID", std::to_string(user_id), false);

            dpp::embed public_embed = EmbedUtils::create_moderator_embed("User Banned", "dumb dumb got the ban treatment", user_pfp);
            public_embed.add_field("User", target_user.username);

            // Ban the user
            auto ban_result = co_await bot->co_guild_ban_add(event.command.guild_id, user_id, 0);

            if (ban_result.is_error()) {
                event.reply("Failed to ban the user.");
                std::cerr << "Failed to ban user. Error: " << ban_result.get_error().message << std::endl;
                co_return;
            }

            // Send the embed to the moderator channel
            dpp::message mod_message;
            mod_message.set_channel_id(1070176260891889725); // Private channel ID
            mod_message.add_embed(mod_embed);

            auto mod_message_result = co_await bot->co_message_create(mod_message);

            if (mod_message_result.is_error()) {
                event.reply("Failed to send mod message.");
                std::cerr << "Failed to send mod message. Error: " << mod_message_result.get_error().message << std::endl;
            }

            // Send the embed to the public channel
            dpp::message public_message;
            public_message.set_channel_id(1070188919192305744); // Public channel ID
            public_message.add_embed(public_embed);

            auto public_message_result = co_await bot->co_message_create(public_message);

            if (public_message_result.is_error()) {
                event.reply("Failed to send public message.");
                std::cerr << "Failed to send public message. Error: " << public_message_result.get_error().message << std::endl;
            }

            event.reply("User has been banned.");
        } else {
            event.reply("Failed to retrieve user details.");
            std::cerr << "Failed to retrieve user details. Error: " << user_identified.get_error().message << std::endl;
        }
    } catch (const std::exception& e) {
        event.reply("An error occurred: " + std::string(e.what()));
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}
