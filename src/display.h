#ifndef DISPLAY_H
#define DISPLAY_H

#include "position.h"
#include "searchstate.h"

#include <optional>

struct Display
{
    static void display_position(Position& pos, std::optional<SearchParameters> search_parameters);
};

#endif // !DISPLAY_H
