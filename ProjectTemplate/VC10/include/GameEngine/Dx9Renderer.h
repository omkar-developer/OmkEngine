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

struct Vertex
{
	float x, y, z;
	float rhw;
	unsigned int color;
};

#define Vertex_FVF D3DFVF_XYZRHW | D3DFVF_DIFFUSE
#define VBMAX 32

struct vertex
{
	float x, y, z;
	float tx, ty;
	unsigned int color;
};

//class VertexBuffer
//{
//private:
//	vector<vertex> data;
//	C_Device* m_dev;
//
//public:
//	ImageResource* image;;
//
//	VertexBuffer(C_Device* dev);
//	void AddData(float px, float py, float tx, float ty, unsigned int color);
//	void AddTile(float px, float py, float tx, float ty, unsigned int color);
//	void ModifyTile(int indx);
//	void DeleteBuffer(int idx);
//	void ClearBuffer();
//	void DrawBuffer();
//};

class C_Renderer
{
private:
	// tmp variables
	D3DXMATRIX mat, m2, mat2, mat3, mat4;
	RECT rect;

	LPD3DXSPRITE m_sprite;
	C_Device* m_dev;
	IDirect3DVertexBuffer9* vb;

public:
	C_Renderer(C_Device* dev);
	~C_Renderer();

	bool StartDrawing();
	void EndDrawing();

	void DrawImageTranslated(IResource* image, const PointF& translation, const RectangleI& source, const RectangleF& destination, unsigned int color = 0xFFFFFFFF, float angle=0.0f, float layer = 0.0f);
	void DrawImageTranslated(IResource* image, const PointF& translation, int sl, int st, int sr, int sb, float dx, float dy, float dr, float db, unsigned int color = 0xFFFFFFFF, float angle=0.0f, float layer = 0.0f);

	void DrawImage(IResource* image, const RectangleI& source, const RectangleF& destination, unsigned int color = 0xFFFFFFFF, float angle=0.0f, float layer = 0.0f);
	void DrawImage(IResource* image, int sx, int sy, int sw, int sh, float dx, float dy, float dr, float db, unsigned int color = 0xFFFFFFFF, float angle=0.0f, float layer = 0.0f);

	void DrawText(IResource* font, const wchar_t* text, RectangleI* destination, unsigned int color = 0xFFFFFFFF, unsigned int style = FONT_STYLE_CENTERALIGN | FONT_STYLE_VCENTERALIGN, float scale = 1.0f, int Drawonly = -1);
	void DrawText(IResource* font, const wchar_t* text, int dx, int dy, int dr, int db, unsigned int color = 0xFFFFFFFF, unsigned int style = FONT_STYLE_CENTERALIGN | FONT_STYLE_VCENTERALIGN, float scale = 1.0f, int Drawonly = -1);
	void CalculateRect(IResource* font, RectangleI* pt, const wchar_t* str, unsigned int start, unsigned int end, unsigned int style, float scale);

	void DrawPolygon(PointF* verts, int vertexCount, unsigned int color);
	void DrawSolidPolygon(PointF* verts, int vertexCount, unsigned int color);
	void DrawCircle(const PointF& center, float radius, unsigned int color);
	void DrawSolidCircle(const PointF& center, float radius, unsigned int color);
	void DrawLine(const PointF& p1, const PointF& p2, unsigned int color);

	//void DrawGrid(float size);

	void SetDevice(C_Device* dev);
	C_Device* GetDevice();

	void OnDeviceLost();
	void OnDeviceReset();
	void OnDeviceCreated();
	void OnDeviceRelease();

	void DisconnectEvents();
	//void SetRenderTarget();
};