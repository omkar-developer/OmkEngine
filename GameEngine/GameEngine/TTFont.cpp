/*
 * Copyright (c) 2013 Omkar Kanase
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "stdafx.h"

#ifdef TTFONT

FT_Library TTFont::library = 0;

TTFont::TTFont()
{
	
}

TTFont::~TTFont()
{
	if(face!=0)
	{
		FT_Done_Face(face);
		face = 0;
	}
}

bool TTFont::Init()
{
	FT_Error error = FT_Init_FreeType( &library );
	if ( error )
	{
		return false;
	}
	return true;
}

bool TTFont::LoadFromFile(const char* filename)
{
	if(library == 0) return false;
	FT_Error error = FT_New_Face( library, filename, 0, &face );
	if ( error )
	{
		return false;
	}
	SetSize(16);
	return true;
}

void TTFont::SetSize(int size)
{
	if(library == 0 || face == 0) return;
	FT_Set_Char_Size(face, 0, size * 64, 96, 96);
	FT_Set_Pixel_Sizes(face, 0, size);
}

FT_GlyphSlot TTFont::GetGlyph(wchar_t ch)
{
	FT_UInt ind = FT_Get_Char_Index(face, (FT_ULong) ch);
	FT_Error err = FT_Load_Glyph(face, ind, FT_LOAD_DEFAULT);
	if(err)	return 0;

	err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
	if(err)	return 0;
	return face->glyph;
}

void TTFont::Release()
{
	if(face!=0)
	{
		FT_Done_Face(face);
		face = 0;
	}
	if(library!=0)
	{
		FT_Done_FreeType(library);
		library = 0;
	}
}

#endif