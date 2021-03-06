/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/Color.h>
#include <april/RenderSystem.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "Utility.h"

namespace atres
{
	static april::ColoredTexturedVertex _ctVertices[6];
	static april::PlainVertex _pVertices[6];
	static float _top = 0.0f;
	static float _bottom = 0.0f;

	HL_ENUM_CLASS_DEFINE(Horizontal,
	(
		HL_ENUM_DEFINE(Horizontal, Left);
		HL_ENUM_DEFINE(Horizontal, Center);
		HL_ENUM_DEFINE(Horizontal, Right);
		HL_ENUM_DEFINE(Horizontal, LeftWrapped);
		HL_ENUM_DEFINE(Horizontal, LeftWrappedUntrimmed);
		HL_ENUM_DEFINE(Horizontal, RightWrapped);
		HL_ENUM_DEFINE(Horizontal, RightWrappedUntrimmed);
		HL_ENUM_DEFINE(Horizontal, CenterWrapped);
		HL_ENUM_DEFINE(Horizontal, CenterWrappedUntrimmed);
		HL_ENUM_DEFINE(Horizontal, Justified);
	));

	bool Horizontal::isLeft() const
	{
		return (*this == Left || *this == LeftWrapped || *this == LeftWrappedUntrimmed);
	}

	bool Horizontal::isCenter() const
	{
		return (*this == Center || *this == CenterWrapped || *this == CenterWrappedUntrimmed);
	}

	bool Horizontal::isRight() const
	{
		return (*this == Right || *this == RightWrapped || *this == RightWrappedUntrimmed);
	}

	bool Horizontal::isWrapped() const
	{
		return (*this == LeftWrapped || *this == LeftWrappedUntrimmed || *this == CenterWrapped || *this == CenterWrappedUntrimmed ||
			*this == RightWrapped || *this == RightWrappedUntrimmed || *this == Justified);
	}

	bool Horizontal::isUntrimmed() const
	{
		return (*this == LeftWrappedUntrimmed || *this == CenterWrappedUntrimmed || *this == RightWrappedUntrimmed);
	}

	HL_ENUM_CLASS_DEFINE(Vertical,
	(
		HL_ENUM_DEFINE(Vertical, Top);
		HL_ENUM_DEFINE(Vertical, Center);
		HL_ENUM_DEFINE(Vertical, Bottom);
	));

	HL_ENUM_CLASS_DEFINE(TextEffect,
	(
		HL_ENUM_DEFINE(TextEffect, None);
		HL_ENUM_DEFINE(TextEffect, Shadow);
		HL_ENUM_DEFINE(TextEffect, Border);
	));

	ColorData::ColorData() :
		colorTopLeft(april::Color::White),
		colorTopRight(april::Color::White),
		colorBottomLeft(april::Color::White),
		colorBottomRight(april::Color::White),
		horizontalColorFit(false),
		verticalColorFit(false)
	{
	}

	ColorData::ColorData(const april::Color& colorTopLeft, const april::Color& colorTopRight, const april::Color& colorBottomLeft,
		const april::Color& colorBottomRight, bool horizontalColorFit, bool verticalColorFit)
	{
		this->colorTopLeft = colorTopLeft;
		this->colorTopRight = colorTopRight;
		this->colorBottomLeft = colorBottomLeft;
		this->colorBottomRight = colorBottomRight;
		this->horizontalColorFit = horizontalColorFit;
		this->verticalColorFit = verticalColorFit;
	}

	ColorData::~ColorData()
	{
	}

	RectDefinition::RectDefinition()
	{
	}

	RectDefinition::~RectDefinition()
	{
	}

	SymbolDefinition::SymbolDefinition() :
		RectDefinition(),
		advance(0.0f)
	{
	}

	CharacterDefinition::CharacterDefinition() :
		SymbolDefinition(),
		offsetY(0.0f)
	{
	}

	BorderCharacterDefinition::BorderCharacterDefinition(float borderThickness) :
		RectDefinition()
	{
		this->borderThickness = borderThickness;
	}

	RenderRectangle::RenderRectangle()
	{
	}

	RenderSequence::RenderSequence() :
		texture(NULL),
		lastAlpha(0),
		multiplyAlpha(false)
	{
	}

	void RenderSequence::addRenderRectangle(const RenderRectangle& rect, const april::Color& color, float italicSkewOffset)
	{
		_ctVertices[0].x = _ctVertices[2].x = _ctVertices[4].x = rect.dest.left();
		_ctVertices[1].x = _ctVertices[3].x = _ctVertices[5].x = rect.dest.right();
		_ctVertices[0].y = _ctVertices[1].y = _ctVertices[3].y = rect.dest.top();
		_ctVertices[2].y = _ctVertices[4].y = _ctVertices[5].y = rect.dest.bottom();
		_ctVertices[0].u = _ctVertices[2].u = _ctVertices[4].u = rect.src.left();
		_ctVertices[1].u = _ctVertices[3].u = _ctVertices[5].u = rect.src.right();
		_ctVertices[0].v = _ctVertices[1].v = _ctVertices[3].v = rect.src.top();
		_ctVertices[2].v = _ctVertices[4].v = _ctVertices[5].v = rect.src.bottom();
		if (italicSkewOffset > 0.0f)
		{
			_ctVertices[0].x += italicSkewOffset;
			_ctVertices[1].x += italicSkewOffset;
			_ctVertices[3].x += italicSkewOffset;
		}
		this->vertices.add(_ctVertices, 6);
		this->colors.add(color, 6);
	}

	void RenderSequence::addRenderRectangle(const RenderRectangle& rect, const april::Color& colorTopLeft, const april::Color& colorTopRight,
		const april::Color& colorBottomLeft, const april::Color& colorBottomRight, float italicSkewOffset)
	{
		_ctVertices[0].x = _ctVertices[2].x = _ctVertices[4].x = rect.dest.left();
		_ctVertices[1].x = _ctVertices[3].x = _ctVertices[5].x = rect.dest.right();
		_ctVertices[0].y = _ctVertices[1].y = _ctVertices[3].y = rect.dest.top();
		_ctVertices[2].y = _ctVertices[4].y = _ctVertices[5].y = rect.dest.bottom();
		_ctVertices[0].u = _ctVertices[2].u = _ctVertices[4].u = rect.src.left();
		_ctVertices[1].u = _ctVertices[3].u = _ctVertices[5].u = rect.src.right();
		_ctVertices[0].v = _ctVertices[1].v = _ctVertices[3].v = rect.src.top();
		_ctVertices[2].v = _ctVertices[4].v = _ctVertices[5].v = rect.src.bottom();
		if (italicSkewOffset > 0.0f)
		{
			_ctVertices[0].x += italicSkewOffset;
			_ctVertices[1].x += italicSkewOffset;
			_ctVertices[3].x += italicSkewOffset;
		}
		this->vertices.add(_ctVertices, 6);
		this->colors += colorTopLeft;
		this->colors += colorTopRight;
		this->colors += colorBottomLeft;
		this->colors += colorTopRight;
		this->colors += colorBottomLeft;
		this->colors += colorBottomRight;
	}

	void RenderSequence::mergeFrom(const RenderSequence& other)
	{
		this->vertices += other.vertices;
		this->colors += other.colors;
	}
	
	void RenderSequence::clear()
	{
		this->vertices.clear();
		this->colors.clear();
	}

	RenderLiningSequence::RenderLiningSequence()
	{
	}

	void RenderLiningSequence::addRectangle(cgrectf rect)
	{
		_top = rect.top();
		_bottom = rect.bottom();
		if (this->vertices.size() > 0 && this->vertices[this->vertices.size() - 1].y == _bottom && this->vertices[this->vertices.size() - 3].y == _top)
		{
			this->vertices[this->vertices.size() - 1].x = this->vertices[this->vertices.size() - 3].x = this->vertices[this->vertices.size() - 5].x = rect.right();
		}
		else
		{
			_pVertices[0].x = _pVertices[2].x = _pVertices[4].x = rect.left();
			_pVertices[1].x = _pVertices[3].x = _pVertices[5].x = rect.right();
			_pVertices[0].y = _pVertices[1].y = _pVertices[3].y = _top;
			_pVertices[2].y = _pVertices[4].y = _pVertices[5].y = _bottom;
			this->vertices.add(_pVertices, 6);
		}
	}

	void RenderLiningSequence::mergeFrom(const RenderLiningSequence& other)
	{
		this->vertices += other.vertices;
	}

	void RenderLiningSequence::clear()
	{
		this->vertices.clear();
	}

	RenderWord::RenderWord() :
		start(0),
		count(0),
		spaces(0),
		icon(false),
		advanceX(0.0f),
		bearingX(0.0f)
	{
	}

	RenderLine::RenderLine() :
		start(0),
		count(0),
		spaces(0),
		advanceX(0.0f),
		terminated(false)
	{
	}
	
	RenderText::RenderText()
	{
	}

	HL_ENUM_CLASS_DEFINE(FormatTag::Type,
	(
		HL_ENUM_DEFINE(FormatTag::Type, Escape);
		HL_ENUM_DEFINE(FormatTag::Type, Font);
		HL_ENUM_DEFINE(FormatTag::Type, Icon);
		HL_ENUM_DEFINE(FormatTag::Type, Color);
		HL_ENUM_DEFINE(FormatTag::Type, Scale);
		HL_ENUM_DEFINE(FormatTag::Type, NoEffect);
		HL_ENUM_DEFINE(FormatTag::Type, Shadow);
		HL_ENUM_DEFINE(FormatTag::Type, Border);
		HL_ENUM_DEFINE(FormatTag::Type, StrikeThrough);
		HL_ENUM_DEFINE(FormatTag::Type, Underline);
		HL_ENUM_DEFINE(FormatTag::Type, Italic);
		HL_ENUM_DEFINE(FormatTag::Type, Hide);
		HL_ENUM_DEFINE(FormatTag::Type, IgnoreFormatting);
		HL_ENUM_DEFINE(FormatTag::Type, Close);
		HL_ENUM_DEFINE(FormatTag::Type, CloseConsume);
	));

	FormatTag::FormatTag() :
		type(Type::Escape),
		start(0),
		count(0)
	{
	}

	TextureContainer::TextureContainer() :
		texture(NULL),
		penX(0),
		penY(0),
		rowHeight(0)
	{
	}

	TextureContainer::~TextureContainer()
	{
		if (this->texture != NULL)
		{
			april::rendersys->destroyTexture(this->texture);
		}
	}

	TextureContainer* TextureContainer::createNew() const
	{
		return new TextureContainer();
	}

	BorderTextureContainer::BorderTextureContainer(float borderThickness) :
		TextureContainer()
	{
		this->borderThickness = borderThickness;
	}

	TextureContainer* BorderTextureContainer::createNew() const
	{
		return new BorderTextureContainer(this->borderThickness);
	}

	CacheEntryBasicText::CacheEntryBasicText() :
		horizontal(Horizontal::CenterWrapped),
		vertical(Vertical::Center),
		useMoreColors(false),
		horizontalColorFit(false),
		verticalColorFit(false)
	{
	}
	
	CacheEntryBasicText::~CacheEntryBasicText()
	{
	}

	void CacheEntryBasicText::set(chstr text, chstr fontName, cgrectf rect, Horizontal horizontal, Vertical vertical, const april::Color& color, cgvec2f offset)
	{
		this->text = text;
		this->fontName = fontName;
		this->rect = rect;
		this->horizontal = horizontal;
		this->vertical = vertical;
		// overrides alpha to 255 because of a conflict within AprilUI that would cause alpha-animated
		// text to be cached for every possible alpha value (0-255) and rendered very slowly
		this->color = april::Color(color, 255);
		this->useMoreColors = false;
		this->colorTopRight = april::Color::White;
		this->colorBottomLeft = april::Color::White;
		this->colorBottomRight = april::Color::White;
		this->horizontalColorFit = false;
		this->verticalColorFit = false;
		this->offset = offset;
	}

	void CacheEntryBasicText::set(chstr text, chstr fontName, cgrectf rect, Horizontal horizontal, Vertical vertical, const april::Color& color, bool useMoreColors,
		const april::Color& colorTopRight, const april::Color& colorBottomLeft, const april::Color& colorBottomRight, bool horizontalColorFit, bool verticalColorFit, cgvec2f offset)
	{
		this->text = text;
		this->fontName = fontName;
		this->rect = rect;
		this->horizontal = horizontal;
		this->vertical = vertical;
		// overrides alpha to 255 because of a conflict within AprilUI that would cause alpha-animated
		// text to be cached for every possible alpha value (0-255) and rendered very slowly
		this->color = april::Color(color, 255);
		this->useMoreColors = useMoreColors;
		this->colorTopRight = colorTopRight;
		this->colorBottomLeft = colorBottomLeft;
		this->colorBottomRight = colorBottomRight;
		this->horizontalColorFit = horizontalColorFit;
		this->verticalColorFit = verticalColorFit;
		this->offset = offset;
	}

	bool CacheEntryBasicText::isEqual(const CacheEntryBasicText& other) const
	{
		if (this->text == other.text &&
			this->fontName == other.fontName &&
			this->rect == other.rect &&
			this->horizontal == other.horizontal &&
			this->vertical == other.vertical &&
			this->color.r == other.color.r &&
			this->color.g == other.color.g &&
			this->color.b == other.color.b &&
			this->offset == other.offset)
		{
			if (this->useMoreColors == other.useMoreColors)
			{
				if (!this->useMoreColors ||
					(this->colorTopRight.r == other.colorTopRight.r &&
					 this->colorTopRight.g == other.colorTopRight.g &&
					 this->colorTopRight.b == other.colorTopRight.b &&
					 this->colorBottomLeft.r == other.colorBottomLeft.r &&
					 this->colorBottomLeft.g == other.colorBottomLeft.g &&
					 this->colorBottomLeft.b == other.colorBottomLeft.b &&
					 this->colorBottomRight.r == other.colorBottomRight.r &&
					 this->colorBottomRight.g == other.colorBottomRight.g &&
					 this->colorBottomRight.b == other.colorBottomRight.b &&
					 this->horizontalColorFit == other.horizontalColorFit &&
					 this->verticalColorFit == other.verticalColorFit))
				{
					return true;
				}
			}
		}
		return false;
	}

	unsigned int CacheEntryBasicText::hash() const
	{
		unsigned int result = 0;
		for_iter (i, 0, this->text.size())
		{
			result ^= this->text[i] << ((i % 4) * 8);
		}
		for_iter (i, 0, this->fontName.size())
		{
			result ^= this->fontName[i] << ((i % 4) * 8);
		}
		// this code with fvar is used to avoid strict aliasing violations
		const float* fvar = NULL;
		const bool* bvar = NULL;
		fvar = &this->rect.x;	result ^= *((unsigned int*)fvar);
		fvar = &this->rect.y;	result ^= *((unsigned int*)fvar) << 8;
		fvar = &this->rect.w;	result ^= *((unsigned int*)fvar) << 16;
		fvar = &this->rect.h;	result ^= *((unsigned int*)fvar) << 24;
		result ^= (((unsigned int)(this->vertical.value)) & 0xFFFF);
		result ^= (((unsigned int)(this->horizontal.value) << 16) & 0xFFFF0000);
		result ^= (this->color.r << 8);
		result ^= (this->color.g << 16);
		result ^= (this->color.b << 24);
		fvar = &this->offset.x;	result ^= *((unsigned int*)fvar) & 0xFFFF;
		fvar = &this->offset.y;	result ^= (*((unsigned int*)fvar) << 16) & 0xFFFF0000;
		bvar = &this->useMoreColors;	result ^= *((unsigned int*)bvar);
		if (this->useMoreColors)
		{
			result ^= (this->colorTopRight.r << 8);
			result ^= (this->colorTopRight.g << 16);
			result ^= (this->colorTopRight.b << 24);
			result ^= (this->colorBottomLeft.r << 8);
			result ^= (this->colorBottomLeft.g << 16);
			result ^= (this->colorBottomLeft.b << 24);
			result ^= (this->colorBottomRight.r << 8);
			result ^= (this->colorBottomRight.g << 16);
			result ^= (this->colorBottomRight.b << 24);
			bvar = &this->horizontalColorFit;	result ^= *((unsigned int*)bvar) & 0xFFFF;
			bvar = &this->verticalColorFit;		result ^= (*((unsigned int*)bvar) << 16) & 0xFFFF0000;
		}
		return result;
	}

	CacheEntryText::CacheEntryText() :
		CacheEntryBasicText()
	{
	}

	CacheEntryLines::CacheEntryLines() :
		CacheEntryBasicText()
	{
	}

	CacheEntryLine::CacheEntryLine()
	{
	}

	void CacheEntryLine::set(chstr text, chstr fontName, cgvec2f size)
	{
		this->text = text;
		this->fontName = fontName;
		this->size = size;
	}

	bool CacheEntryLine::isEqual(const CacheEntryLine& other) const
	{
		return (this->text == other.text && this->fontName == other.fontName && this->size == other.size);
	}

	unsigned int CacheEntryLine::hash() const
	{
		unsigned int result = 0;
		for_iter (i, 0, this->text.size())
		{
			result ^= this->text[i] << ((i % 4) * 8);
		}
		for_iter (i, 0, this->fontName.size())
		{
			result ^= this->fontName[i] << ((i % 4) * 8);
		}
		// this code with fvar is used to avoid strict aliasing violations
		const float* fvar = NULL;
		fvar = &this->size.x;	result ^= *((unsigned int*)fvar);
		fvar = &this->size.y;	result ^= (*((unsigned int*)fvar) << 16) & 0xFFFF0000;
		return result;
	}
	
}
