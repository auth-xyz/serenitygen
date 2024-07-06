#include <dpp/dpp.h>
#include <chrono>

#include "../headers/commands/unmute.hpp"
#include "../headers/libraries/embed_utils.hpp"

void handle_unmute_command(const dpp::slashcommand_t& event) {
  // Acknowledge the interaction to avoid "Application did not respond" error
  event.thinking();

  // Check if the 'user' parameter exists
  if (event.get_parameter("user").index() == std::variant_npos) {
    event.edit_response("Please mention a user to unmute.");
    return;
  }

  // Extract user ID from parameters
  dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));

  // Retrieve bot instance from event context
  dpp::cluster* bot = event.from->creator;

  // Fetch target user's details
  bot->user_get(user_id, [event, bot, user_id](const dpp::confirmation_callback_t& callback) {
    if (callback.is_error()) {
      event.edit_response("Failed to retrieve user details.");
      return;
    }

    dpp::user_identified target_user = std::get<dpp::user_identified>(callback.value);
    std::string user_pfp = target_user.get_avatar_url();
    std::string moderator_pfp = event.command.usr.get_avatar_url();

    dpp::embed mod_embed = EmbedUtils::create_basic_embed("User " + target_user.username + " was Unmuted", "his soul was spared", 0x00FF00,user_pfp);
    mod_embed.add_field("Moderator", event.command.usr.username, true);
    
    bot->guild_member_timeout(event.command.guild_id, user_id, 0, [event, bot, mod_embed](const dpp::confirmation_callback_t& timeout_callback) {
      if (timeout_callback.is_error()) {
        event.edit_response("Failed to unmute the user.");
      } else {
        // Send embed to moderator channel
        dpp::message mod_message;
        mod_message.set_channel_id(1070176260891889725); // Private channel ID
        mod_message.add_embed(mod_embed);
        bot->message_create(mod_message, [event](const dpp::confirmation_callback_t& mod_callback) {
          if (mod_callback.is_error()) {
            event.edit_response("Failed to send mod message.");
          } else {
            // Edit the initial response to indicate the user has been unmuted
            event.edit_response("User has been unmuted.");
          }
        });
      }
    });
  });
}
