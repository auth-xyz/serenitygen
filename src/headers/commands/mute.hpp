#pragma once
#include <dpp/dpp.h>
#include <chrono>

dpp::task<void> handle_mute_command(const dpp::slashcommand_t& event);