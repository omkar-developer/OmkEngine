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

class UIWString
{
private:
	vector<wchar_t> tmp;

public:
	UIWString(){tmp.push_back('\0');};
	UIWString(const char* str);
	UIWString(const wchar_t* str);
	UIWString(const wchar_t str);
	UIWString(const long str);
	UIWString(const unsigned long str);
	UIWString(const int str);
	UIWString(const double str);
	
	void AttachString(UIWString str);
	void AddString(UIWString str);
	void InsertString(UIWString str, unsigned int index);
	void DeleteString(unsigned int index=0, unsigned int length=1);
	void Clear();
	unsigned int Length();
	wchar_t* GetPtr();

	bool operator==(UIWString str);
	bool operator<(const UIWString& str) const;

	UIWString operator+=(UIWString str);

	operator wchar_t*();
	operator wchar_t*() const;
};