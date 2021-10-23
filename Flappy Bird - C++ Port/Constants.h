#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <cstdint>

enum class Gamemodes{
EXIT,
CLASSIC,
VERSUS,
SKINS,
BACKDROPS,
INVALID
};

/*
The skin convention is the following :
- Every skin shall be the same size
    - A compliant skin shall be 17 in width by 12 in height
- The lower nibble shall be used to indicate the number of rows until the skin's row
- The higher nibble shall be used to indicate the number of columns until the skin's column
- As each nibble can store sixteen different values, this allows for 16 * 16 = 256 different skins. Should be enough.
*/


/*
This line right here caused one of the weirdest bugs I've ever witnessed.
Basically, this used to be an int8 : which worked fine, except on one case. The skins on the rightmost column would be saved as
1111 .... : however, because this was a signed data type, negative values would get written to the save file, causing pretty terrible
problems.
*/

typedef uint8_t SkinPositions;

typedef uint8_t ScreenSide;

namespace ScreenSides
{
    enum{
    LEFT,
    RIGHT,
    ALL
    };
}

#endif // CONSTANTS_H_INCLUDED
