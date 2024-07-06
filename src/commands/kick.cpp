#include "../headers/commands/kick.hpp"
#include "../headers/libraries/embed_utils.hpp"

#include <dpp/dpp.h>

void handle_kick_command(const dpp::slashcommand_t& event) {
  // Check if the parameter 'user' exists
  if (event.get_parameter("user").index() == std::variant_npos) {
    event.reply("Please mention a user to kick.");
    return;
  }

  // Extract user ID and reason from parameters
  dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));

  // Retrieve bot instance from event context
  dpp::cluster* bot = event.from->creator;

  // Fetch target user's details
  bot->user_get(user_id, [event, bot, user_id](const dpp::confirmation_callback_t& callback) {
    if (callback.is_error()) {
      event.reply("Failed to retrieve user details.");
      return;
    }

    // Extract user details if successful
    dpp::user_identified target_user = std::get<dpp::user_identified>(callback.value);
    std::string user_pfp = target_user.get_avatar_url();
    std::string moderator_pfp = event.command.usr.get_avatar_url();

    // Create embed for moderation message
    dpp::embed mod_embed = EmbedUtils::create_basic_embed("User " + target_user.username + " was Kicked", "goof ball got the kick treatment", 0xFFA500, user_pfp);
    mod_embed.add_field("Moderator", event.command.usr.username);

    // Get guild ID from event context
    dpp::snowflake guild_id = event.command.guild_id;

    // Perform kick operation
    bot->guild_member_kick(guild_id, user_id, [event, mod_embed, bot](const dpp::confirmation_callback_t& kick_callback) {
        if (kick_callback.is_error()) {
            event.reply("Failed to kick the user.");
        } else {
            // Send embed to moderator channel
            dpp::message mod_message;
            mod_message.set_channel_id(1070176260891889725); // Private channel ID
            mod_message.add_embed(mod_embed);

            bot->message_create(mod_message, [event](const dpp::confirmation_callback_t& mod_callback) {
                if (mod_callback.is_error()) {
                    event.reply("Failed to send mod message.");
                } else {
                    // Reply to the user who initiated the command
                    event.reply("User has been kicked.");
                }
            });
        }
    });




  });
}
