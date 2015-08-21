/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hlog.h>
#include <hltypes/hrdir.h>
#include <hltypes/hresource.h>
#include <hltypes/hstring.h>

#include "atres.h"
#include "FontIconMap.h"

namespace atres
{
	static hstr _map_withoutExtension(hstr filename)
	{
		return hresource::withoutExtension(filename);
	}

	FontIconMap::FontIconMap(chstr fontDirectory, chstr name, float height, float scale, float lineHeight, float spacing) : FontDynamic(name)
	{
		this->fontDirectory = fontDirectory;
		this->name = name;
		this->height = height;
		this->baseScale = scale;
		this->scale = scale;
		this->lineHeight = lineHeight;
		this->spacing = spacing;
	}

	FontIconMap::~FontIconMap()
	{
	}

	bool FontIconMap::_isAllowAlphaTextures()
	{
		return false;
	}

	bool FontIconMap::_load()
	{
		if (!hrdir::exists(this->fontDirectory))
		{
			return false;
		}
		if (!FontDynamic::_load())
		{
			return false;
		}
		if (this->lineHeight == 0.0f)
		{
			this->lineHeight = this->height;
		}
		return true;
	}

	april::Image* FontIconMap::_loadIconImage(chstr iconName, bool initial, int& advance)
	{
		harray<hstr> files = hrdir::files(this->fontDirectory).mapped(&_map_withoutExtension);
		if (!files.has(iconName))
		{
			return NULL;
		}
		hstr filename = april::rendersys->findTextureResource(hrdir::joinPath(this->fontDirectory, iconName));
		if (filename == "")
		{
			return NULL;
		}
		april::Image* image = april::Image::createFromResource(filename);
		advance = image->w + (int)this->spacing;
		return image;
	}

}
