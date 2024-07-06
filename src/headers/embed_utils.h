#ifndef EMBED_UTILS_H
#define EMBED_UTILS_H

#include <dpp/dpp.h>

class EmbedUtils {
public:
    static dpp::embed create_basic_embed(const std::string& title, const std::string& description, uint32_t color, const std::string& user_avatar_url);
    static dpp::embed create_moderator_embed(const std::string& title, const std::string& description, const std::string& mod_avatar_url);
    static dpp::embed create_public_embed(const std::string& title, const std::string& description, const std::string& user_avatar_url);
};

#endif // EMBED_UTILS_H

