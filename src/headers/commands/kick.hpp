#ifndef KICK_HPP
#define KICK_HPP

#include <dpp/dpp.h>

dpp::task<void> handle_kick_command(const dpp::slashcommand_t& event);

#endif 
