// commands/embed.cpp
#include "embed.h"

void handle_embed_command(const dpp::slashcommand_t& event) {
    dpp::embed embed = dpp::embed()
        .set_title("Embed Title")
        .set_description("Embed Description")
        .set_color(0x00FF00);
    event.reply(dpp::message().add_embed(embed));
}

