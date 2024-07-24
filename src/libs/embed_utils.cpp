#include "../headers/libraries/embed_utils.hpp"

dpp::embed EmbedUtils::create_basic_embed(const std::string& title, const std::string& description, uint32_t color, const std::string& user_avatar_url) 
{
    return dpp::embed()
        .set_title(title)
        .set_description(description)
        .set_color(color)
        .set_thumbnail(user_avatar_url)
        .set_footer(dpp::embed_footer().set_text("Serenity © - 2024"));
}

dpp::embed EmbedUtils::create_moderator_embed(const std::string& title, const std::string& description, const std::string& mod_avatar_url) 
{
    return dpp::embed()
        .set_title(title)
        .set_description(description)
        .set_color(0xff0000)  // Red color for moderator actions
        .set_thumbnail(mod_avatar_url)
        .set_footer(dpp::embed_footer().set_text("Serenity © - 2024"));
}

dpp::embed EmbedUtils::create_public_embed(const std::string& title, const std::string& description, const std::string& user_avatar_url) 
{
    return dpp::embed()
        .set_title(title)
        .set_description(description)
        .set_color(0x00ff00)  // Green color for public actions
        .set_thumbnail(user_avatar_url)
        .set_footer(dpp::embed_footer().set_text("Serenity © - 2024"));
}

dpp::embed EmbedUtils::create_error_embed(const std::string& description)
{
    return dpp::embed()
        .set_title("An error occured")
        .set_description(description)
        .set_color(0xff0000)
        .set_footer(dpp::embed_footer().set_text("Serenity © - 2024"));
}