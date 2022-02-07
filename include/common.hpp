#pragma once

#include <my_math.hpp>

// To pass this struct to ImGui, we use a pointer to r.
// This works because the 4 values are aligned in memory.
// So the pointer can work as an array.
typedef struct
{
    float r, g, b, a;
} Color;