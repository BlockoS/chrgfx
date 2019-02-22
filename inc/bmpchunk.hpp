#ifndef BMPCHUNK_H
#define BMPCHUNK_H

#include "utils.hpp"

using namespace chrgfx;
using namespace png;

bank* bmpchunk(image<index_pixel>* image, chr_traits* chrtraits);

#endif