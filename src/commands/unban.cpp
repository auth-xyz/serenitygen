#include "../headers/unban.h"
#include <dpp/dpp.h>

void handle_unban_command(const dpp::slashcommand_t& event) {
  // Check if the parameter 'user' exists
  if (event.get_parameter("user").index() == std::variant_npos) {
    event.reply("Please mention a user to unban.");
    return;
  }

  // Get the user ID from the parameters
  dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));

  // Check if the event.from or event.from->creator is null
  if (!event.from || !event.from->creator) {
    event.reply("Bot instance not found.");
    return;
  }

  dpp::cluster* bot = event.from->creator;

  // Unban the user
  bot->guild_ban_delete(event.command.guild_id, user_id, [event, bot, user_id](const dpp::confirmation_callback_t& unban_callback) {
    if (unban_callback.is_error()) {
      event.reply("Failed to unban the user.");
    } else {
      // Create the embed
      dpp::embed unban_embed;
      unban_embed.set_title("User Unbanned")
        .set_description("A user has been unbanned.")
        .add_field("User ID", std::to_string(user_id), true)
        .set_footer(dpp::embed_footer().set_text("Serenity © - 2024"))
        .set_color(0x00FF00); // Green color for success

      // Send the embed to the moderator channel
      dpp::message mod_message;
      mod_message.set_channel_id(1070569664599556146); // Private channel ID
      mod_message.add_embed(unban_embed);
      bot->message_create(mod_message);

      // Send a reply indicating success
      event.reply("User has been unbanned.");
    }
  });
}
