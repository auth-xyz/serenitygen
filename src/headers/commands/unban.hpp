#ifndef UNBAN_HPP
#define UNBAN_HPP

#include <dpp/dpp.h>

dpp::task<void> handle_unban_command(const dpp::slashcommand_t& event);

#endif // UNBAN_H
