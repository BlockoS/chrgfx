#include "bmpchunk.hpp"

using namespace png;
using namespace chrgfx;

// class to chunk an input png into 8x8 chrs
// psuedo:
// - get dimensions, divide by 8
// - width in chrs, height in chrs
// - chrw * chrh = total tile count, provision vector<chr>
// - src image pixel row counter = 0
// - for each chr row
// -- provision temp array of chr for number of columns
// -- for each pixel row (8 pixel rows)
// --- curr pixel row = get_row(src image pixel row counter ++)
// --- for each chr column
// ---- read curr pixel row 8 pixels, store into temp array for each tile
// -- dump temp array into total chr vector

bank* bmpchunk(image<index_pixel>* bitmap, chr_traits* chrtraits)
{
	// store the chr dimensions locally
	u8 chrw = chrtraits->width, chrh = chrtraits->height;

	// get tile dimensions
	u32 chr_cols = bitmap->get_width() / chrw;
	u32 chr_rows = bitmap->get_height() / chrh;
	u32 chr_count = chr_cols * chr_rows;

	// set new bitmap dimensions (pixels)
	u32 bmph = chr_cols * chrh;
	u32 bmpw = chr_rows * chrw;
	bitmap->resize(bmpw, bmph);

	bank* outbank = new bank();

	outbank->reserve(chr_count);

	// temp array to hold the chrs for the current row
	auto this_chrrow = new chr[chr_cols];
	// temp vector holding the pixels from the current bmp row
	auto this_bmprow = new std::vector<index_pixel>();
	// iters and counters
	size_t bmprow_idx = 0,	// tracks the current pixel row in the source bitmap
			chrrow_iter, pxlrow_iter, chrcolpxl_iter, chrcol_iter;

	// for each chr row
	for(chrrow_iter = 0; chrrow_iter < chr_rows; chrrow_iter++)
	{
		// for each pxl row in the current chr row
		for(pxlrow_iter = 0; pxlrow_iter < chrh; pxlrow_iter++)
		{
			this_bmprow = &bitmap->get_row(bmprow_idx++);
			// for every (chr width) pixels in the pixel row
			chrcol_iter = 0;
			for(chrcolpxl_iter = 0; chrcolpxl_iter < chr_cols; chrcolpxl_iter += chrw)
			{
				std::copy(chrcolpxl_iter, chrcolpxl_iter + chrw,
									std::back_inserter(this_chrrow[chrcol_iter]));
				chrcol_iter++;
			}
		}
		// dump temp array into outbank
		std::copy(this_chrrow[0], this_chrrow[chr_cols - 1],
							std::back_inserter(outbank));
	}
	delete[] this_chrrow;

	return outbank;
}