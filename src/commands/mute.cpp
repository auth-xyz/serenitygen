#include "../headers/mute.h"
#include "../headers/embed_utils.h"

#include <dpp/dpp.h>
#include <chrono>

void handle_mute_command(const dpp::slashcommand_t& event) {
  event.thinking();
  // Check if the 'user' and 'duration' parameters exist
  if (event.get_parameter("user").index() == std::variant_npos || event.get_parameter("duration").index() == std::variant_npos) {
    event.reply("Please mention a user to mute and specify the duration.");
    return;
  }

  // Extract user ID and duration from parameters
  dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));
  int64_t duration_minutes = std::get<int64_t>(event.get_parameter("duration"));

  // Calculate the timeout end time
  auto now = std::chrono::system_clock::now();
  auto timeout_end = std::chrono::system_clock::to_time_t(now + std::chrono::minutes(duration_minutes));

  // Retrieve bot instance from event context
  dpp::cluster* bot = event.from->creator;

  // Fetch target user's details
  bot->user_get(user_id, [event, bot, user_id, timeout_end, duration_minutes](const dpp::confirmation_callback_t& callback) {
    if (callback.is_error()) {
      event.reply("Failed to retrieve user details.");
      return;
    }

    dpp::user_identified target_user = std::get<dpp::user_identified>(callback.value);
    std::string user_pfp = target_user.get_avatar_url();
    std::string moderator_pfp = event.command.usr.get_avatar_url();

    dpp::embed mod_embed = EmbedUtils::create_basic_embed("User Muted", "Goof got the :shh: treatment", 0xFFFF00, user_pfp);
    mod_embed.add_field("User", target_user.username);
    mod_embed.add_field("Duration", std::to_string(duration_minutes));

    // Perform the mute operation (timeout)
    bot->guild_member_timeout(event.command.guild_id, user_id, timeout_end, [event, bot, mod_embed](const dpp::confirmation_callback_t& timeout_callback) {
      if (timeout_callback.is_error()) {
        event.edit_response("Failed to mute the user.");
      } else {
        // Send embed to moderator channel
        dpp::message mod_message;
        mod_message.set_channel_id(1070569664599556146); // Private channel ID
        mod_message.add_embed(mod_embed);
        bot->message_create(mod_message, [event](const dpp::confirmation_callback_t& mod_callback) {
          if (mod_callback.is_error()) {
            event.edit_response("Failed to send mod message.");
          } else {
            // Reply to the user who initiated the command
            event.edit_response("User has been muted.");
          }
        });
      }
    });
  });
}
