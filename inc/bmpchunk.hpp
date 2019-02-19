#ifndef BMPCHUNK_H
#define BMPCHUNK_H

#include "utils.hpp"

using namespace chrgfx;
using namespace png;

// class to chunk an input png into 8x8 chrs
// psuedo:
// - get dimensions, divide by 8
// - width in tiles, height in tiles
// - tilew * tileh = total tile count, provision vector<chr>
// - src image pixel row counter = 0
// - for each tile row
// -- provision temp array of chr for number of columns
// -- for each pixel row (8 pixel rows)
// --- curr pixel row = get_row(src image pixel row counter ++)
// --- for each tile column
// ---- read curr pixel row 8 pixels, store into temp array for each tile
// -- dump temp array into total chr vector

#endif