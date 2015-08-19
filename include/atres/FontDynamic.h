/// @file
/// @version 3.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a font with a dynamically created character map.

#ifndef ATRESTTF_FONT_DYNAMIC_H
#define ATRESTTF_FONT_DYNAMIC_H

#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "atresExport.h"
#include "Font.h"

namespace april
{
	class Texture;
}

namespace atres
{
	class atresExport FontDynamic : public Font
	{
	public:
		FontDynamic(chstr name);
		~FontDynamic();

		april::Texture* getTexture(unsigned int charCode);
		april::Texture* getTexture(chstr iconName);
		bool hasCharacter(unsigned int charCode);
		bool hasIcon(chstr iconName);

	protected:
		int penX;
		int penY;
		int rowHeight;

		bool _load();

		april::Texture* _createTexture();
		bool _addCharacterBitmap(unsigned int charCode, bool initial = false);
		bool _addIconBitmap(chstr iconName, bool initial = false);
		TextureContainer* _addBitmap(bool initial, april::Image* image, int bitmapWidth, int bitmapHeight, int offsetY, chstr name);

		virtual april::Image* _loadCharacterImage(unsigned int charCode, bool initial, int& leftOffset, int& topOffset, int& ascender, int& descender, int& bearingX, int& advance);
		virtual april::Image* _loadIconImage(chstr iconName, bool initial);

	};

}

#endif

