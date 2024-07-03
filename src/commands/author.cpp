#include "author.h"

void handle_author_command(const dpp::slashcommand_t& event) {
  dpp::embed embed = dpp::embed()
    .set_title("Bot Developer")
    .set_description("Serenity, a discord bot made in C++ using the dpp library, was made by this godlike being that transcended humanity called Auth")
    .set_color(0x3498db);


  event.reply(dpp::message().add_embed(embed));
}
