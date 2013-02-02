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

C_Renderer::C_Renderer(C_Device* dev)
{
	m_sprite = 0;
	vb = 0;
	m_dev = dev;
	D3DXCreateSprite(m_dev->GetDevice(), &m_sprite);
	dev->GetDevice()->CreateVertexBuffer(sizeof(Vertex)*VBMAX, D3DUSAGE_DYNAMIC, Vertex_FVF, D3DPOOL_DEFAULT, &vb, 0);

	m_dev->OnDeviceCreated.RegisterSingleHandler<C_Renderer, &C_Renderer::OnDeviceCreated>(this);
	m_dev->OnDeviceLost.RegisterSingleHandler<C_Renderer, &C_Renderer::OnDeviceLost>(this);
	m_dev->OnDeviceRelease.RegisterSingleHandler<C_Renderer, &C_Renderer::OnDeviceRelease>(this);
	m_dev->OnDeviceReset.RegisterSingleHandler<C_Renderer, &C_Renderer::OnDeviceReset>(this);
}

C_Renderer::~C_Renderer()
{
	if(m_sprite!=0) m_sprite->Release();
	if(vb!=0) vb->Release();
	DisconnectEvents();
}

bool C_Renderer::StartDrawing()
{
	if(SUCCEEDED(m_sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK  /*| D3DXSPRITE_SORT_TEXTURE*/ /*| D3DXSPRITE_DONOTSAVESTATE*/))) return true;
	return false;
}

void C_Renderer::EndDrawing()
{
	m_sprite->End();
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
	D3DXMatrixTranslation(&mat4, translation.left, translation.top, 0.0f);
	D3DXMatrixTranslation(&mat3, dx + (dr-dx)/2.0f, dy + (db-dy)/2.0f, 0.0f);
	D3DXMatrixRotationZ(&mat2, angle);
	D3DXMatrixScaling(&mat, (dr - dx)/(sr-sl),(db - dy)/(sb-st), 1.0f);
	m_sprite->GetTransform(&m2);
	m_sprite->SetTransform(&(mat * mat4 * mat2 * m2 * mat3));

	rect.left = sl;
	rect.top = st;
	rect.right = sr;
	rect.bottom = sb;

	m_sprite->Draw(CAST_RES(LPDIRECT3DTEXTURE9, image), &rect, &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(-(rect.right - rect.left)/2.0f, -(rect.bottom - rect.top)/2.0f /** 1/mat._22*/, layer), color);
	m_sprite->SetTransform(&m2);
}

void C_Renderer::DrawImage(IResource* image, int sl, int st, int sr, int sb, float dx, float dy, float dr, float db, unsigned int color, float angle, float layer)
{
	if(image==0 || image->GetResourcePtr()==0) return;
	D3DXMatrixTranslation(&mat3, dx + (dr-dx)/2.0f, dy + (db-dy)/2.0f, 0);
	D3DXMatrixRotationZ(&mat2, angle);
	D3DXMatrixScaling(&mat, (dr - dx)/(sr-sl),(db - dy)/(sb-st), 1.0f);
	m_sprite->GetTransform(&m2);
	m_sprite->SetTransform(&(mat * mat2 * m2 * mat3));

	rect.left = sl;
	rect.top = st;
	rect.right = sr;
	rect.bottom = sb;
	m_dev->GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_sprite->Draw(CAST_RES(LPDIRECT3DTEXTURE9, image), &rect, &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(-(rect.right - rect.left)/2.0f, -(rect.bottom - rect.top)/2.0f /** 1/mat._22*/, layer), color);
	m_sprite->SetTransform(&m2);
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
	if(m_sprite!=0) m_sprite->OnLostDevice();
	if(vb!=0)
	{
		vb->Release();
		vb = 0;
	}
}

void C_Renderer::OnDeviceReset()
{
	if(m_sprite!=0) m_sprite->OnResetDevice();
	if(m_dev != 0) m_dev->GetDevice()->CreateVertexBuffer(sizeof(Vertex)*VBMAX, D3DUSAGE_DYNAMIC, Vertex_FVF, D3DPOOL_DEFAULT, &vb, 0);
}

void C_Renderer::OnDeviceCreated()
{

}

void C_Renderer::OnDeviceRelease()
{
	if(m_sprite!=0)
	{
		m_sprite->Release();
		m_sprite = 0;
	}
	if(vb!=0)
	{
		vb->Release();
		vb = 0;
	}
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

//void C_Renderer::DrawGrid(float size)
//{
//	if(vb==0 || m_dev==0) return;
//	Vertex* v;
//	for(int x1=0; x1<(2048.0f/size)/4; x1++)
//	{
//		for(int y1=0; y1<(2048.0f/size)/4; y1++)
//		{
//			vb->Lock(0,0, (void**)&v, D3DLOCK_DISCARD);
//
//			for(int x=0; x<4, x<VBMAX; x++)
//			{
//				for(int y=0; y<4, y<VBMAX; y++)
//				{
//					v[x*4+y].z = 0;
//					v[x*4+y].x = x*size*(x1*size*4);
//					v[x*4+y].y = y*size*(y1*size*4);
//					v[x*4+y].rhw = 1;
//					v[x*4+y].color = 0xFFFFFFFF;
//				}
//			}
//
//			vb->Unlock();
//			m_dev->GetDevice()->SetFVF(Vertex_FVF);
//			m_dev->GetDevice()->SetStreamSource(0, vb, 0, sizeof(Vertex));
//			m_dev->GetDevice()->DrawPrimitive(D3DPT_POINTLIST, 0, 16);
//		}
//	}
//}

void C_Renderer::DrawPolygon(PointF* verts, int vertexCount, unsigned int color)
{
	if(vb==0 || m_dev==0 || vertexCount < 2) return;
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
	m_dev->GetDevice()->DrawPrimitive(D3DPT_LINESTRIP, 0, vertexCount);
}

void C_Renderer::DrawSolidPolygon(PointF* verts, int vertexCount, unsigned int color)
{
	if(vb==0 || m_dev==0 || vertexCount < 2) return;
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
	//m_dev->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_dev->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_dev->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//m_dev->GetDevice()->SetFVF(Vertex_FVF);
	//m_dev->GetDevice()->SetStreamSource(0, vb, 0, sizeof(Vertex));
	////m_dev->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_dev->GetDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, vertexCount - 2);
	////m_dev->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_dev->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_dev->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_dev->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_dev->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_dev->GetDevice()->SetFVF(Vertex_FVF);
	m_dev->GetDevice()->SetStreamSource(0, vb, 0, sizeof(Vertex));
	m_dev->GetDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, vertexCount - 2);
	m_dev->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void C_Renderer::DrawCircle(const PointF& center, float radius, unsigned int color)
{
	if(vb==0 || m_dev==0) return;
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
	m_dev->GetDevice()->DrawPrimitive(D3DPT_LINESTRIP, 0, VBMAX-1);
}

void C_Renderer::DrawSolidCircle(const PointF& center, float radius, unsigned int color)
{
	if(vb==0 || m_dev==0) return;
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
	m_dev->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void C_Renderer::DrawLine(const PointF& p1, const PointF& p2, unsigned int color)
{
	if(vb==0 || m_dev==0) return;
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
	m_dev->GetDevice()->DrawPrimitive(D3DPT_LINESTRIP, 0, 1);
}

//VertexBuffer::VertexBuffer(C_Device* dev)
//{
//	m_dev = dev;
//}
//
//void VertexBuffer::AddData(float px, float py, float tx, float ty, unsigned int color)
//{
//	vertex v;
//	v.x = px;
//	v.y = py;
//	v.tx = tx;
//	v.ty = ty;
//	v.color = color;
//}
//
//void VertexBuffer::AddTile(float px, float py, float tx, float ty, unsigned int color)
//{
//	float srcl = i->sl/(float)sz.width;
//	float srct = 1 - i->st/(float)sz.height;
//	float srcr = i->sr/(float)sz.width;
//	float srcb = 1 - i->sb/(float)sz.height;
//}
//
//void VertexBuffer::ModifyTile(int indx)
//{
//}
//
//void VertexBuffer::DeleteBuffer(int idx)
//{
//}
//
//void VertexBuffer::ClearBuffer()
//{
//}
//
//void VertexBuffer::DrawBuffer()
//{
//}