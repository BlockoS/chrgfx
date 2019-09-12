#include "utils.hpp"

using namespace png;

namespace chrgfx
{
/**
 * Returns a value with set bits equivalent to the number of bits requested
 * e.g. 5 bits = 0x0000001F (0x1F = 00011111)
 */
u32 create_bitmask(u8 bitcount)
{
	// max 32 bits, anything higher would waste cycles
	if(bitcount > 31)
	{
		return 0xffffffff;
	}

	u32 bitmask = 0;
	for(s16 mask_iter = 0; mask_iter < bitcount; ++mask_iter)
	{
		bitmask |= (bitmask << 1) | 1;
	}
	return bitmask;
}

palette* make_pal(bool blank)
{
	auto outpal = new palette();
	outpal->reserve(256);

	if(blank)
	{
		outpal->insert(outpal->begin(), 256, color(0, 0, 0));
	}
	else
	{
		// basic 16 color palette based on Xterm colors
		// repeated 16x for 256 entry 8bpp palette
		for(uint8_t l = 0; l < 16; l++)
		{
			outpal->push_back(color(0, 0, 0));
			outpal->push_back(color(128, 0, 0));
			outpal->push_back(color(0, 128, 0));
			outpal->push_back(color(128, 128, 0));

			outpal->push_back(color(0, 0, 128));
			outpal->push_back(color(128, 0, 128));
			outpal->push_back(color(0, 128, 128));
			outpal->push_back(color(192, 192, 192));

			outpal->push_back(color(128, 128, 128));
			outpal->push_back(color(255, 0, 0));
			outpal->push_back(color(0, 255, 0));
			outpal->push_back(color(255, 255, 0));

			outpal->push_back(color(0, 0, 255));
			outpal->push_back(color(255, 0, 255));
			outpal->push_back(color(0, 255, 255));
			outpal->push_back(color(255, 255, 255));
		}
	}
	return outpal;
}

palette* get_pal(pal_xform* xform, const u8* data, s16 subpal)
{
	auto outpal = new palette();

	// if subpalette < 0, do not use subpalettes
	// - get count from palette traits (palette_length)
	// if subpalette > 0, use subpalettes
	// - start of paliter loop is: subpalette * subpalette_length
	// - paliter loop cound: subpalette_length

	u8 count{0};
	auto traits = xform->get_traits();

	// subpalettes are 0 indexed
	if(subpal > (traits->subpalette_count - 1))
	{
		std::cerr << "Warning: invalid subpalette specified; using full palette"
							<< std::endl;
		subpal = -1;
	}

	auto datasize = traits->color_size;

	if(subpal < 0)
	{
		outpal->reserve(traits->palette_length);
		count = traits->palette_length;
	}
	else
	{
		outpal->reserve(traits->subpalette_length);
		count = traits->subpalette_length;
		data += traits->subpalette_length * subpal * datasize;
#ifdef DEBUG
		std::cerr << "subpal: " << (int)subpal << std::endl;
		std::cerr << "count: " << (int)count << std::endl;
#endif
	}

	for(u8 paliter = 0; paliter < count; paliter++)
	{
		// palettes are only valid up to 256 colors
		// though some devices (such as NeoGeo) have system palettes that are much
		// larger stop adding if we've hit this limit
		if(outpal->size() >= 256) return outpal;
		outpal->push_back(*(xform->get_rgb(data)));
		data += datasize;
	}

	// fill_pal(outpal);

	return outpal;
}

// fill in any blank entries in a palette to bring it up to 256
void fill_pal(palette* pal)
{
	if(pal->size() >= 256) return;

	u8 toFill = 256 - pal->size();

	for(u8 fillIter = 0; fillIter < toFill; fillIter++)
		pal->push_back(color(0, 0, 0));

	return;
}

}	// namespace chrgfx