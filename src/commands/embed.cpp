#include "../headers/embed.h"
#include <iostream>

void handle_embed_command(const dpp::slashcommand_t& event) {
    dpp::embed embed = dpp::embed()
        .set_title("pppp")
        .set_author("Auth", "https://auth-xyz.github.io/", "https://auth-xyz.github.io/favicon.ico")
        .set_description("pppp")
        .set_color(0x00FF00);

    std::cout << event.from << std::endl;

    event.reply(dpp::message().add_embed(embed));
}

