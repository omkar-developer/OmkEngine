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
#include <algorithm>

C_Renderer::C_Renderer(C_Device* dev)
{
	m_dev = dev;

	m_dev->OnDeviceCreated.RegisterSingleHandler<C_Renderer, &C_Renderer::OnDeviceCreated>(this);
	m_dev->OnDeviceLost.RegisterSingleHandler<C_Renderer, &C_Renderer::OnDeviceLost>(this);
	m_dev->OnDeviceRelease.RegisterSingleHandler<C_Renderer, &C_Renderer::OnDeviceRelease>(this);
	m_dev->OnDeviceReset.RegisterSingleHandler<C_Renderer, &C_Renderer::OnDeviceReset>(this);
}

C_Renderer::~C_Renderer()
{
	DisconnectEvents();
}

bool C_Renderer::StartDrawing()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	m_info.clear();
	return true;
}

void DrawBuffer(vertex3* vec, Color* col, vertex2* tex, unsigned int size)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(vertex3), vec);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Color), col);
	glTexCoordPointer(2, GL_FLOAT, sizeof(vertex2), tex);

	glDrawArrays(GL_TRIANGLES, 0, size);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void MultMatVertex(float* mat, vertex3* vec, float angle)
{
	float x = vec->x * mat[0] + vec->y * mat[4] + vec->z * mat[8] + 1 * mat[12];
	float y = vec->x * mat[1] + vec->y * mat[5] + vec->z * mat[9] + 1 * mat[13];
	float z = vec->x * mat[2] + vec->y * mat[6] + vec->z * mat[10] + 1 * mat[14];
	vec->x = x;
	vec->y = y;
	vec->z = z;
}

void VertexTransform(vertex3* vec, unsigned int size, float tx, float ty, float angle, float scalex, float scaley, float tx2 = 0.0f, float ty2 = 0.0f)
{
	float sin0 = sinf(angle);
	float cos0 = cosf(angle);
	float x = 0, y = 0;

	for(unsigned int i = 0; i<size; i++)
	{	
		vec[i].x *= scalex;
		vec[i].y *= scaley;

		vec[i].x += tx2;
		vec[i].y += ty2;

		x = (vec[i].x * cos0 - vec[i].y * sin0) + tx;
		y = (vec[i].x * sin0 + vec[i].y * cos0) + ty; 

		vec[i].x = x;
		vec[i].y = y;
	}
}

void C_Renderer::EndDrawing()
{
	GLuint ctex = 0;
	unsigned int cpos = 0;

	if(MAXBATCHSIZE < 6) return;
	sort(m_info.begin(), m_info.end(), greater<DrawingInfo>());
	for(unsigned int f = 0; f<m_info.size(); f++)
	{
		DrawingInfo* i = &m_info[f];
		if(ctex==0)
		{
			ctex = *((GLuint*)i->image->GetResourcePtr());
			glBindTexture(GL_TEXTURE_2D, ctex);
		}
		else if(cpos + 6 > MAXBATCHSIZE || ctex != *((GLuint*)i->image->GetResourcePtr()))
		{
			if(cpos!=0) DrawBuffer(m_v, m_c, m_t, cpos);
			ctex = *((GLuint*)i->image->GetResourcePtr());
			glBindTexture(GL_TEXTURE_2D, ctex);
			cpos = 0;
		}

		Size sz = i->image->GetImageSize();

		float srcl = i->sl/(float)sz.width;
		float srct = 1 - i->st/(float)sz.height;
		float srcr = i->sr/(float)sz.width;
		float srcb = 1 - i->sb/(float)sz.height;

		ColorARGB32 col(i->color);
		unsigned char r = col.GetR();
		unsigned char g = col.GetG();
		unsigned char b = col.GetB();
		unsigned char a = col.GetA();

		float dxx = -(i->sr - i->sl)/2.0f;
		float dyy = -(i->sb - i->st)/2.0f;
		float drr = dxx + (i->sr - i->sl);
		float dbb = dyy + (i->sb - i->st);

		m_v[cpos] = vertex3(dxx, dyy, 0);
		m_v[cpos+1] = vertex3(drr, dyy, 0);
		m_v[cpos+2] = vertex3(dxx, dbb, 0);

		m_v[cpos+3] = vertex3(drr, dyy, 0);
		m_v[cpos+4] = vertex3(drr, dbb, 0);
		m_v[cpos+5] = vertex3(dxx, dbb, 0);

		VertexTransform(&m_v[cpos], 6, i->dx + (i->dr-i->dx)/2.0f, i->dy+(i->db-i->dy)/2.0f, i->angle, (i->dr-i->dx)/(i->sr-i->sl), (i->db-i->dy)/(i->sb-i->st), i->tx, i->ty);

		m_c[cpos] = Color(r, g, b, a);
		m_c[cpos+1] = Color(r, g, b, a);
		m_c[cpos+2] = Color(r, g, b, a);

		m_c[cpos+3] = Color(r, g, b, a);
		m_c[cpos+4] = Color(r, g, b, a);
		m_c[cpos+5] = Color(r, g, b, a);

		m_t[cpos] = vertex2(srcl, srct);
		m_t[cpos+1] = vertex2(srcr, srct);
		m_t[cpos+2] = vertex2(srcl, srcb);

		m_t[cpos+3] = vertex2(srcr, srct);
		m_t[cpos+4] = vertex2(srcr, srcb);
		m_t[cpos+5] = vertex2(srcl, srcb);

		cpos += 6;
	}

	if(cpos!=0)
	{
		DrawBuffer(m_v, m_c, m_t, cpos);
		ctex = 0;
		glBindTexture(GL_TEXTURE_2D, ctex);
		cpos = 0;
	}

	m_info.clear();

	glFlush();
}

void C_Renderer::DrawImage(IResource* image, const RectangleI& source, const RectangleF& destination, unsigned int color, float angle, float layer)
{
	DrawImage(image, source.left, source.top, source.right, source.bottom, destination.left, destination.top, destination.right, destination.bottom, color, angle, layer);
}

void C_Renderer::DrawImageTranslated(IResource* image, const PointF& translation, const RectangleI& source, const RectangleF& destination, unsigned int color, float angle, float layer)
{
	DrawImageTranslated(image, translation, source.left, source.top, source.right, source.bottom, destination.left, destination.top, destination.right, destination.bottom, color, angle, layer);
}

void C_Renderer::DrawImageTranslated(IResource* image, const PointF& translation, int sl, int st, int sr, int sb, float dx, float dy, float dr, float db, unsigned int color, float angle, float layer)
{
	if(image==0 || image->GetResourcePtr()==0) return;
	m_info.push_back(DrawingInfo(layer, (ImageResource*)image, sl, st, sr, sb, dx, dy, dr, db, color, angle, translation.left, translation.top));
}

void C_Renderer::DrawImage(IResource* image, int sl, int st, int sr, int sb, float dx, float dy, float dr, float db, unsigned int color, float angle, float layer)
{
	if(image==0 || image->GetResourcePtr()==0) return;
	GLuint* tex = (GLuint*)image->GetResourcePtr();
	if(*tex==0) return;
	m_info.push_back(DrawingInfo(layer, (ImageResource*)image, sl, st, sr, sb, dx, dy, dr, db, color, angle));
}

void C_Renderer::DrawText(IResource* font, const wchar_t* text, RectangleI* destination, unsigned int color, unsigned int style, float scale, int Drawonly)
{
	DrawText(font, text, destination->left, destination->top, destination->right, destination->bottom, color, style, scale, Drawonly);
}

void C_Renderer::DrawText(IResource* font, const wchar_t* text, int dx, int dy, int dr, int db, unsigned int color, unsigned int style, float scale, int Drawonly)
{
	if(font==0) return;
#ifdef USE_BITMAPFONT
	Sprite* Font = CAST_RES(Sprite*, font);
	FontResource* rc = (FontResource*)font;
#else
	LPD3DXFONT Font = CAST_RES(LPD3DXFONT, font);
#endif

	if(Font==0) return;

#ifdef USE_BITMAPFONT
#else
	D3DXMatrixScaling(&mat, scale, scale, 1.0f);
	m_sprite->GetTransform(&m2);
	m_sprite->SetTransform(&(mat * m2));

	rect.left = (LONG)(dx/scale);
	rect.top = (LONG)(dy/scale);
	rect.right = (LONG)(dr/scale);
	rect.bottom = (LONG)(db/scale);
#endif


#ifdef USE_BITMAPFONT
	float sw = 0.0f;
	RectangleI* rct = 0;
	RectangleF rect;
	rect.left = (float)dx;
	rect.top = (float)dy;
	rect.right = (float)dr;
	rect.bottom = (float)db;

	unsigned int txtsize = wcslen(text);

	float fsize = (float)rc->GetSize();

	float fwidth = 0.0f;
	float fheight = 0.0f;
	for(unsigned int i=0; i<txtsize; i++)
	{
		rct = Font->GetSpriteRect((unsigned int)text[i]);

		if(rct!=0)
		{
			float scale2 = fsize / (rct->bottom - rct->top);
			fwidth += ((rct->right - rct->left - BFONTSPACEW) * scale2) * scale;
			fheight = ((rct->bottom - rct->top) * scale2) * scale;
		}
	}

	if(style&FONT_STYLE_MULTILINE)
	{
		rect.left -= BFONTSPACE;
		rect.right -= BFONTSPACE;
	}
	else
	{
		if(style&FONT_STYLE_CENTERALIGN)
		{
			float hfwidth = fwidth / 2.0f;
			float hwidth = (rect.right - rect.left) / 2.0f;
			rect.left = (rect.left + hwidth) - hfwidth - BFONTSPACEW;
			rect.right = rect.left + fwidth - BFONTSPACEW;
		}
		else if(style&FONT_STYLE_RIGHTALIGN)
		{
			rect.left = rect.right - fwidth - BFONTSPACE;
			rect.right = rect.left + fwidth - BFONTSPACE;
		}
		else
		{
			rect.left -= BFONTSPACE;
			rect.right = rect.left + fwidth - BFONTSPACE;
		}

	
		if(style&FONT_STYLE_VCENTERALIGN)
		{
			float hfheight = fheight / 2.0f;
			float hheight = (rect.bottom - rect.top) / 2.0f;
			rect.top = (rect.top + hheight) - hfheight;
			rect.bottom = rect.top + fheight;
		}
		else if(style&FONT_STYLE_BOTTOMALIGN)
		{
			rect.top = rect.bottom - fheight;
			rect.right = rect.top + fheight;
		}
		else
		{
			rect.bottom = rect.top + fheight;
		}
	}


	float ty = 0.0f;
	float tx = 0;

	for(unsigned int i=0; i<txtsize; i++)
	{
		rct = Font->GetSpriteRect((unsigned int)text[i]);

		if(rct!=0)
		{
			float scale2 = fsize / (rct->bottom - rct->top);
			if(style&FONT_STYLE_MULTILINE)
			{
				RectangleF rect2 = rect;
				rect2.left = rect.left + tx;
				rect2.right = (rect2.left + (rct->right - rct->left) * scale2  * scale);
				if(rect2.right>rect.right || text[i] == L'\n' || text[i] == L'\r')
				{
					ty += fsize * scale + BFONTVSPACE;
					tx = 0;

					rect2.left = rect.left + tx;
					rect2.right = (rect2.left + (rct->right - rct->left) * scale2  * scale);
				}

				rect2.top = rect.top + ty;
				rect2.bottom = (rect2.top + fsize * scale);

				if(!(text[i] == L'\n' || text[i] == L'\r'))
				{
					Font->SetIndex((unsigned int)text[i]);
					if(Drawonly==-1 || Drawonly==i) Font->Draw(this, rect2, color, 0.0f, 0.0f);
					tx += ((rct->right - rct->left - BFONTSPACEW) * scale2) * scale;
				}
			}
			else
			{
				rect.right = (rect.left + (rct->right - rct->left) * scale2  * scale);
				rect.bottom = (rect.top + fsize  * scale);
				Font->SetIndex((unsigned int)text[i]);
				if(Drawonly==-1 || Drawonly==i) Font->Draw(this, rect, color, 0.0f, 0.0f);
				rect.left += ((rct->right - rct->left - BFONTSPACEW) * scale2) * scale;
			}
		}
	}	
#else
	Font->DrawTextW(m_sprite, text, -1, (LPRECT)&rect, style, color);
	m_sprite->SetTransform(&m2);
#endif
}

void C_Renderer::CalculateRect(IResource* font, RectangleI* pt, const wchar_t* str, unsigned int start, unsigned int end, unsigned int style, float scale)
{
	if(font==0) return;
#ifdef USE_BITMAPFONT
	Sprite* Font = CAST_RES(Sprite*, font);
	FontResource* rc = (FontResource*)font;
#else
	LPD3DXFONT Font = CAST_RES(LPD3DXFONT, font);
#endif
	if(Font==0) return;

#ifdef USE_BITMAPFONT
#else
	D3DXMatrixScaling(&mat, scale, scale, 1.0f);
	m_sprite->GetTransform(&m2);
	m_sprite->SetTransform(&(mat * m2));
	pt->left = (LONG)(pt->left/scale);
	pt->top = (LONG)(pt->top/scale);
	pt->right = (LONG)(pt->right/scale);
	pt->bottom = (LONG)(pt->bottom/scale);
#endif	

#ifdef USE_BITMAPFONT
	float sw = 0.0f;
	RectangleI* rct = 0;
	RectangleF rect;
	rect.left = (float)pt->left;
	rect.top = (float)pt->top;
	rect.right = (float)pt->right;
	rect.bottom = (float)pt->bottom;

	unsigned int txtsize = wcslen(str);

	float fsize = (float)rc->GetSize();

	if(start>=txtsize || end>txtsize) return;

	float fwidth = 0.0f;
	float fheight = 0.0f;
	for(unsigned int i=start; i< start + end; i++)
	{
		rct = Font->GetSpriteRect((unsigned int)str[i]);

		if(rct!=0)
		{
			float scale2 = fsize / (rct->bottom - rct->top);
			fwidth += ((rct->right - rct->left - BFONTSPACEW) * scale2) * scale;
			fheight = ((rct->bottom - rct->top) * scale2) * scale;
		}
	}

	if(style&FONT_STYLE_MULTILINE)
	{
		rect.left -= BFONTSPACE;
		rect.right -= BFONTSPACE;
	}
	else
	{
		rect.left -= BFONTSPACE;
		rect.right = rect.left + fwidth - BFONTSPACE;
		rect.bottom = rect.top + fheight;

		pt->left = (int)(rect.left/scale);
		pt->top = (int)(rect.top/scale);
		pt->right = (int)(rect.right/scale);
		pt->bottom = (int)(rect.bottom/scale);
	}

#else
	Font->DrawTextW(m_sprite, str + start, end, (LPRECT)pt, style | DT_CALCRECT, D3DXCOLOR(0,0,0,1));
	m_sprite->SetTransform(&m2);
#endif
}

void C_Renderer::SetDevice(C_Device* dev)
{
	DisconnectEvents();
	m_dev = dev;

	m_dev->OnDeviceCreated.RegisterSingleHandler<C_Renderer, &C_Renderer::OnDeviceCreated>(this);
	m_dev->OnDeviceLost.RegisterSingleHandler<C_Renderer, &C_Renderer::OnDeviceLost>(this);
	m_dev->OnDeviceRelease.RegisterSingleHandler<C_Renderer, &C_Renderer::OnDeviceRelease>(this);
	m_dev->OnDeviceReset.RegisterSingleHandler<C_Renderer, &C_Renderer::OnDeviceReset>(this);
}

C_Device* C_Renderer::GetDevice()
{
	return m_dev;
}

void C_Renderer::OnDeviceLost()
{
	m_info.clear();
}

void C_Renderer::OnDeviceReset()
{
	
}

void C_Renderer::OnDeviceCreated()
{

}

void C_Renderer::OnDeviceRelease()
{
	m_info.clear();
	DisconnectEvents();
}

void C_Renderer::DisconnectEvents()
{
	if(m_dev==0) return;
	m_dev->OnDeviceCreated.DisconnectHandler<C_Renderer, &C_Renderer::OnDeviceCreated>(this);
	m_dev->OnDeviceLost.DisconnectHandler<C_Renderer, &C_Renderer::OnDeviceLost>(this);
	m_dev->OnDeviceRelease.DisconnectHandler<C_Renderer, &C_Renderer::OnDeviceRelease>(this);
	m_dev->OnDeviceReset.DisconnectHandler<C_Renderer, &C_Renderer::OnDeviceReset>(this);
}

void C_Renderer::DrawPolygon(PointF* verts, int vertexCount, unsigned int color)
{
	/*if(vb==0 || m_dev==0 || vertexCount < 2) return;
	Vertex* v;
	vb->Lock(0,0, (void**)&v, D3DLOCK_DISCARD);
	for(int i=0; i<vertexCount, i<VBMAX-1; i++)
	{
		v[i].z = 0;
		v[i].x = verts[i].left;
		v[i].y = verts[i].top;
		v[i].rhw = 1;
		v[i].color = color;
	}

	v[vertexCount].z = 0;
	v[vertexCount].x = verts[0].left;
	v[vertexCount].y = verts[0].top;
	v[vertexCount].rhw =  1;
	v[vertexCount].color = color;

	vb->Unlock();
	m_dev->GetDevice()->SetFVF(Vertex_FVF);
	m_dev->GetDevice()->SetStreamSource(0, vb, 0, sizeof(Vertex));
	m_dev->GetDevice()->DrawPrimitive(D3DPT_LINESTRIP, 0, vertexCount);*/
}

void C_Renderer::DrawSolidPolygon(PointF* verts, int vertexCount, unsigned int color)
{
	/*if(vb==0 || m_dev==0 || vertexCount < 2) return;
	Vertex* v;
	vb->Lock(0,0, (void**)&v, D3DLOCK_DISCARD);
	for(int i=0; i<vertexCount, i<VBMAX; i++)
	{
		v[i].z = 0;
		v[i].x = verts[i].left;
		v[i].y = verts[i].top;
		v[i].rhw = 1;
		v[i].color = color;
	}

	vb->Unlock();
	m_dev->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_dev->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_dev->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_dev->GetDevice()->SetFVF(Vertex_FVF);
	m_dev->GetDevice()->SetStreamSource(0, vb, 0, sizeof(Vertex));
	m_dev->GetDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, vertexCount - 2);
	m_dev->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);*/
}

void C_Renderer::DrawCircle(const PointF& center, float radius, unsigned int color)
{
	/*if(vb==0 || m_dev==0) return;
	Vertex* v;
	vb->Lock(0,0, (void**)&v, D3DLOCK_DISCARD);
	int f = 0;
	for(float i=0; i<=D3DX_PI * 2.0f, f<VBMAX; i+=D3DX_PI * 2/(VBMAX-1), f++)
	{
		v[f].z = 0;
		v[f].x = center.left + cos(i) * radius;
		v[f].y = center.top + sin(i) * radius;
		v[f].rhw = 1;
		v[f].color = color;
	}

	vb->Unlock();
	m_dev->GetDevice()->SetFVF(Vertex_FVF);
	m_dev->GetDevice()->SetStreamSource(0, vb, 0, sizeof(Vertex));
	m_dev->GetDevice()->DrawPrimitive(D3DPT_LINESTRIP, 0, VBMAX-1);*/
}

void C_Renderer::DrawSolidCircle(const PointF& center, float radius, unsigned int color)
{
	/*if(vb==0 || m_dev==0) return;
	Vertex* v;
	vb->Lock(0,0, (void**)&v, D3DLOCK_DISCARD);

	v[0].z = 0;
	v[0].x = center.left;
	v[0].y = center.top;
	v[0].rhw = 1;
	v[0].color = color;

	int f = 1;
	for(float i=0; i<=D3DX_PI * 2.0f, f<VBMAX; i+=D3DX_PI * 2/(VBMAX-2), f++)
	{
		v[f].z = 0;
		v[f].x = center.left + cos(i) * radius;
		v[f].y = center.top + sin(i) * radius;
		v[f].rhw = 1;
		v[f].color = color;
	}

	vb->Unlock();
	m_dev->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_dev->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_dev->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_dev->GetDevice()->SetFVF(Vertex_FVF);
	m_dev->GetDevice()->SetStreamSource(0, vb, 0, sizeof(Vertex));
	m_dev->GetDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, VBMAX-2);
	m_dev->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);*/
}

void C_Renderer::DrawLine(const PointF& p1, const PointF& p2, unsigned int color)
{
	/*if(vb==0 || m_dev==0) return;
	Vertex* v;
	vb->Lock(0,0, (void**)&v, D3DLOCK_DISCARD);

	v[0].z = 0;
	v[0].x = p1.left;
	v[0].y = p1.top;
	v[0].rhw =  1;
	v[0].color = color;

	v[1].z = 0;
	v[1].x = p2.left;
	v[1].y = p2.top;
	v[1].rhw =  1;
	v[1].color = color;

	vb->Unlock();
	m_dev->GetDevice()->SetFVF(Vertex_FVF);
	m_dev->GetDevice()->SetStreamSource(0, vb, 0, sizeof(Vertex));
	m_dev->GetDevice()->DrawPrimitive(D3DPT_LINESTRIP, 0, 1);*/
}