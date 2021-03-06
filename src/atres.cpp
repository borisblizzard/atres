/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <stdio.h>

#include <april/RenderSystem.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>
#include <hltypes/hversion.h>

#include "atres.h"
#include "Renderer.h"

namespace atres
{
	hstr logTag = "atres";

	static hversion version(5, 0, 0);

	static int textureSize = 1024;
	static bool allowAlphaTextures = true;

	void init()
	{
		hlog::write(logTag, "Initializing Atres: " + version.toString());
		atres::renderer = new Renderer();
		// automatically disable alpha textures if render system reports that they aren't supported
		if (!april::rendersys->getCaps().textureFormats.has(april::Image::Format::Alpha))
		{
			allowAlphaTextures = false;
		}
	}
	
	void destroy()
	{
		hlog::write(logTag, "Destroying Atres.");
		if (atres::renderer != NULL)
		{
			delete atres::renderer;
			atres::renderer = NULL;
		}
	}

	int getTextureSize()
	{
		return textureSize;
	}

	void setTextureSize(const int& value)
	{
		textureSize = value;
	}

	bool isAllowAlphaTextures()
	{
		return allowAlphaTextures;
	}

	void setAllowAlphaTextures(const bool& value)
	{
		allowAlphaTextures = value;
	}

}
