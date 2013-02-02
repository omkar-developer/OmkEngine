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

struct vertex3
{
	float x, y, z;

	vertex3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	vertex3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct vertex2
{
	float u, v;

	vertex2()
	{
		u = 0;
		v = 0;
	}

	vertex2(float u, float v)
	{
		this->u = u;
		this->v = v;
	}
};

struct Color
{
	unsigned char r, g, b, a;
	Color()
	{
		r=0;
		g=0;
		b=0;
		a=1.0f;
	}

	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};

void DrawBuffer(vertex3* vec, Color* col, vertex2* tex, unsigned int size);

struct DrawingInfo
{
	float layer;
	float tx, ty;
	float sl, st, sr, sb;
	float dx, dy, dr, db;
	unsigned int color;
	float angle;
	ImageResource* image;

	DrawingInfo()
	{
		layer = 0;
		sl = st = sr = sb = 0;
		dx = dy = dr = db = 0;
		color = 0xFFFFFFFF;
		angle = 0;
		image = 0;
		tx = ty = 0;
	}

	DrawingInfo(float layer, ImageResource* image, float sl, float st, float sr, float sb, float dx, float dy, float dr, float db, unsigned int color, float angle, float tx = 0, float ty = 0)
	{
		this->layer = layer;
		this->sl = sl;
		this->st = st;
		this->sr = sr;
		this->sb = sb;
		this->dx = dx;
		this->dy = dy;
		this->dr = dr;
		this->db = db;
		this->color = color;
		this->angle = angle;
		this->image = image;
		this->tx = tx;
		this->ty = ty;
	}

	bool operator<(const DrawingInfo& str) const
	{
		return layer < str.layer;
	}

	bool operator>(const DrawingInfo& str) const
	{
		return layer > str.layer;
	}
};

#define VBMAX 32
#define MAXBATCHSIZE 500

class C_Renderer
{
private:
	// tmp variables
	C_Device* m_dev;
	vertex3 m_v[MAXBATCHSIZE];
	Color m_c[MAXBATCHSIZE];
	vertex2 m_t[MAXBATCHSIZE];

	vector<DrawingInfo> m_info;

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