#pragma once
#include <string>

namespace users {
    // Prompts the user to select an existing profile or create a new one.
    // Returns the chosen username (never empty).
    std::string select_or_create();
}

