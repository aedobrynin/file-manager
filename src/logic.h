#pragma once

#include "context.h"

bool update_menu(Context *ctx);
void walk(Context *ctx);
void toggle_hidden(Context *ctx);
void delete_file(Context *ctx);
void start_copy(Context *ctx, bool is_cut);
void end_copy(Context *ctx);