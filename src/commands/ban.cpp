#include "../headers/commands/ban.hpp"
#include "../headers/libraries/embed_utils.hpp"

#include <dpp/dpp.h>

void handle_ban_command(const dpp::slashcommand_t& event) {
  // Check if the parameter 'user' exists
  if (event.get_parameter("user").index() == std::variant_npos) {
    event.reply("Please mention a user to ban.");
    return;
  }

  // Get the user ID and reason from the parameters
  dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));
  std::string reason = (event.get_parameter("reason").index() != std::variant_npos)
    ? std::get<std::string>(event.get_parameter("reason")) 
    : "No reason provided";

  // Check if the event.from or event.from->creator is null
  if (!event.from || !event.from->creator) {
    event.reply("Bot instance not found.");
    return;
  }

  dpp::cluster* bot = event.from->creator;

  // Fetch the target user's details
  bot->user_get(user_id, [event, bot, user_id, reason](const dpp::confirmation_callback_t& callback) {
    if (callback.is_error()) {
      event.reply("Failed to retrieve user details.");
      return;
    }

    // Check if the callback value contains a user
    if (std::holds_alternative<dpp::user_identified>(callback.value)) {
      dpp::user_identified target_user = std::get<dpp::user_identified>(callback.value);
      std::string user_pfp = target_user.get_avatar_url();
      std::string moderator_pfp = event.command.usr.get_avatar_url();

      // Create the embeds
      dpp::embed mod_embed = EmbedUtils::create_moderator_embed("User Banned", reason, user_pfp);
      mod_embed.add_field("Moderator", event.command.usr.username, true);
      mod_embed.add_field("User ID", std::to_string(user_id), false);

      dpp::embed public_embed = EmbedUtils::create_moderator_embed("User Banned", "dumb dumb got the ban treatment", user_pfp);
      public_embed.add_field("User", target_user.username);

      // Ban the user
      bot->guild_ban_add(event.command.guild_id, user_id, 0, [event, bot, mod_embed, public_embed](const dpp::confirmation_callback_t& ban_callback) {
        if (ban_callback.is_error()) {
          event.reply("Failed to ban the user.");
        } else {
          // Send the embed to the moderator channel
          dpp::message mod_message;
          mod_message.set_channel_id(1070176260891889725); // Private channel ID
          mod_message.add_embed(mod_embed);
          bot->message_create(mod_message);

          // Send the embed to the public channel
          dpp::message public_message;
          public_message.set_channel_id(1070188919192305744); // Public channel ID
          public_message.add_embed(public_embed);
          bot->message_create(public_message);

          event.reply("User has been banned.");
        }
      });
    } else {
      event.reply("Failed to retrieve user details.");
    }
  });
}
