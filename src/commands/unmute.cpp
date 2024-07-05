#include "../headers/unmute.h"
#include <dpp/dpp.h>
#include <chrono>

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

    dpp::embed mod_embed;
    mod_embed.set_title("User Unmuted")
      .set_description("A user has been unmuted.")
      .add_field("User ID", std::to_string(user_id), true)
      .set_thumbnail(moderator_pfp)
      .set_footer(dpp::embed_footer().set_text("Serenity © - 2024"))
      .set_color(0x00FF00); // Green color for unmute

    // Perform the unmute operation (remove timeout)
    bot->guild_member_timeout(event.command.guild_id, user_id, 0, [event, bot, mod_embed](const dpp::confirmation_callback_t& timeout_callback) {
      if (timeout_callback.is_error()) {
        event.edit_response("Failed to unmute the user.");
      } else {
        // Send embed to moderator channel
        dpp::message mod_message;
        mod_message.set_channel_id(1070569664599556146); // Private channel ID
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
