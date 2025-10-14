#pragma once

inline constexpr float RECTANGLE_VERT[8] = {
     1.0,  1.0,
     1.0, -1.0,
    -1.0, -1.0,
    -1.0,  1.0
};

inline constexpr unsigned int RECTANGLE_INDECES[6] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};