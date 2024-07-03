#include "kick.h"
#include <dpp/dpp.h>

void handle_kick_command(const dpp::slashcommand_t& event) {
    if (event.get_parameter("user").index() == std::variant_npos) {
        event.reply("Please mention a user to kick.");
        return;
    }

    dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));
    std::string reason = event.get_parameter("reason").index() != std::variant_npos 
                             ? std::get<std::string>(event.get_parameter("reason")) 
                             : "No reason provided";
    
    dpp::cluster* bot = event.from->creator;

    // Fetch the target user's details
    bot->user_get(user_id, [&event, bot, user_id, reason](const dpp::confirmation_callback_t& callback) {
        if (callback.is_error()) {
            event.reply("Failed to retrieve user details.");
            return;
        }

        dpp::user target_user = std::get<dpp::user_identified>(callback.value);
        std::string user_pfp = target_user.get_avatar_url();
        std::string moderator_pfp = event.command.usr.get_avatar_url();

        dpp::embed mod_embed;
        mod_embed.set_title("User Kicked")
                 .set_description("A user has been kicked.")
                 .add_field("User ID", std::to_string(user_id), true)
                 .add_field("Reason", reason, false)
                 .set_thumbnail(moderator_pfp)
                 .set_footer(dpp::embed_footer().set_text("Serenity © - 2024"))
                 .set_color(0xFFA500); // Orange color for kick

        // Get the guild ID from the event or another source
        dpp::snowflake guild_id = event.command.guild_id;

        // Perform the kick operation
        bot->get_guild(guild_id, [bot, user_id, reason, mod_embed, event](const dpp::confirmation_callback_t& guild_callback, dpp::guild* guild) {
            if (guild_callback.is_error()) {
                event.reply("Failed to get guild information.");
                return;
            }

            // Kick the user
            guild->remove_guild_member(user_id, reason, [event, bot, mod_embed](const dpp::confirmation_callback_t& kick_callback) {
                if (kick_callback.is_error()) {
                    event.reply("Failed to kick the user.");
                } else {
                    // Send the embed to the moderator channel
                    dpp::message mod_message;
                    mod_message.set_channel_id(1070569664599556146); // Private channel ID
                    mod_message.add_embed(mod_embed);
                    bot->message_create(mod_message, [event](const dpp::confirmation_callback_t& mod_callback) {
                        if (mod_callback.is_error()) {
                            event.reply("Failed to send mod message.");
                        }
                    });

                    // Reply to the user who initiated the command
                    event.reply("User has been kicked.");
                }
            });
        });
    });
}