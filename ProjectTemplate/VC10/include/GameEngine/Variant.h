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

#define RESERVEDNAME "name=~:reserved"

union Var
{
	short Short;
	unsigned UShort;
	int Integer;
	bool Boolean;
	void* Pointer;
	double Double;
	float Float;
	unsigned int UInteger;
	long long LongInteger;
	unsigned long long ULongInteger;
	char Character;
	wchar_t WideCharacter;
};

class Variant
{
protected:
	Var val;

public:
	enum PType
	{
		Undefined = 0,
		UInt,
		Int,
		Bool,
		Ptr,
		Dbl,
		Float,
		LInt,
		ULInt,
		Char,
		WChar,
		String,
		WString,
		Short,
		UShort,
		TSerializer
	};

	const PType GetType() const {return type;}
	const Var   GetValue() const {return val;}

	operator short()
	{
		return val.Short;
	}

	operator unsigned short()
	{
		return val.UShort;
	}

	operator int()
	{
		return val.Integer;
	}

	operator unsigned int()
	{
		return val.UInteger;
	}

	operator bool()
	{
		return val.Boolean;
	}

	operator void*()
	{
		return val.Pointer;
	}

	operator double()
	{
		return val.Double;
	}

	operator float()
	{
		return val.Float;
	}

	operator long long()
	{
		return val.LongInteger;
	}

	operator unsigned long long()
	{
		return val.ULongInteger;
	}

	operator char()
	{
		return val.Character;
	}

	operator wchar_t()
	{
		return val.WideCharacter;
	}

	operator const char*()
	{
		if(type==String)
		return static_cast<string*>(val.Pointer)->data();
		return 0;
	}

	operator const wchar_t*()
	{
		if(type==WString)
		return static_cast<wstring*>(val.Pointer)->data();
		return 0;
	}

	operator string()
	{
		if(type==String)
		return *static_cast<string*>(val.Pointer);
		return "NULL";
	}

	operator wstring()
	{
		if(type==WString)
		return *static_cast<wstring*>(val.Pointer);
		return L"NULL";
	}

	void ClearString()
	{
		if(type==String)
		{
			delete static_cast<string*>(val.Pointer);
			val.LongInteger = 0;
			type = Undefined;
		}
		else if(type==WString)
		{
			delete static_cast<wstring*>(val.Pointer);
			val.LongInteger = 0;
			type = Undefined;
		}
	}

	Variant()
	{
		ClearString();
		val.LongInteger = 0;
		type = Undefined;
	}

	void CopyContruct(const Variant& v)
	{
		ClearString();
		val.LongInteger = 0;
		type = Undefined;
		if(v.GetType()==String)
		{
			val.Pointer = new string(*static_cast<string*>(v.GetValue().Pointer));
			type = String;			
		}
		else if(v.GetType()==WString)
		{
			val.Pointer = new wstring(*static_cast<wstring*>(v.GetValue().Pointer));
			type = WString;			
		}
		else
		{
			val = v.GetValue();
			type = v.GetType();
		}
	}

	Variant(const Variant& v)
	{
		CopyContruct(v);
	}

	Variant& operator=(const Variant& v)
	{
		CopyContruct(v);
		return *this;
	}

	Variant& operator=(const int i)
	{
		ClearString();
		val.Integer = i;
		type = Int;
		return *this;
	}

	Variant& operator=(unsigned short i) 
	{
		ClearString();
		val.UShort = i;
		type = UShort;
		return *this;
	}

	Variant& operator=(short i)
	{
		ClearString();
		val.Short = i;
		type = Short;
		return *this;
	}

	Variant& operator=(unsigned int i) 
	{
		ClearString();
		val.UInteger = i;
		type = UInt;
		return *this;
	}

	Variant& operator=(bool i)
	{
		ClearString();
		val.Boolean = i;
		type = Bool;
		return *this;
	}

	Variant& operator=(void* i)
	{
		ClearString();
		val.Pointer = i;
		type = Ptr;
		return *this;
	}

	Variant& operator=(double i)
	{
		ClearString();
		val.Double = i;
		type = Dbl;
		return *this;
	}

	Variant& operator=(float i)
	{
		ClearString();
		val.Float = i;
		type = Float;
		return *this;
	}

	Variant& operator=(long long i)
	{
		ClearString();
		val.LongInteger = i;
		type = LInt;
		return *this;
	}

	Variant& operator=(unsigned long long i)
	{
		ClearString();
		val.ULongInteger = i;
		type = ULInt;
		return *this;
	}

	Variant& operator=(char i)
	{
		ClearString();
		val.Character = i;
		type = Char;
		return *this;
	}

	Variant& operator=(wchar_t i)
	{
		ClearString();
		val.WideCharacter = i;
		type = WChar;
		return *this;
	}

	Variant& operator=(string val1)
	{
		ClearString();
		val.Pointer = new string(val1);
		type = String;
		return *this;
	}

	Variant& operator=(const char* val1)
	{
		ClearString();
		val.Pointer = new string(val1);
		type = String;
		return *this;
	}

	Variant& operator=(const wchar_t* val1)
	{
		ClearString();
		val.Pointer = new wstring(val1);
		type = WString;
		return *this;
	}

	Variant& operator=(wstring val1)
	{
		ClearString();
		val.Pointer = new wstring(val1);
		type = WString;
		return *this;
	}

	//
	Variant(unsigned short i) 
	{
		ClearString();
		val.UShort = i;
		type = UShort;
	}

	Variant(short i)
	{
		ClearString();
		val.Short = i;
		type = Short;
	}

	Variant(unsigned int i) 
	{
		ClearString();
		val.UInteger = i;
		type = UInt;
	}

	Variant(int i)
	{
		ClearString();
		val.Integer = i;
		type = Int;
	}

	Variant(bool i)
	{
		ClearString();
		val.Boolean = i;
		type = Bool;
	}

	Variant(void* i)
	{
		ClearString();
		val.Pointer = i;
		type = Ptr;
	}

	Variant(double i)
	{
		ClearString();
		val.Double = i;
		type = Dbl;
	}

	Variant(float i)
	{
		ClearString();
		val.Float = i;
		type = Float;
	}

	Variant(long long i)
	{
		ClearString();
		val.LongInteger = i;
		type = LInt;
	}

	Variant(unsigned long long i)
	{
		ClearString();
		val.ULongInteger = i;
		type = ULInt;
	}

	Variant(char i)
	{
		ClearString();
		val.Character = i;
		type = Char;
	}

	Variant(wchar_t i)
	{
		ClearString();
		val.WideCharacter = i;
		type = WChar;
	}

	Variant(string val1)
	{
		ClearString();
		val.Pointer = new string(val1);
		type = String;
	}

	Variant(const char* val1)
	{
		ClearString();
		val.Pointer = new string(val1);
		type = String;
	}

	Variant(const wchar_t* val1)
	{
		ClearString();
		val.Pointer = new wstring(val1);
		type = WString;
	}

	Variant(wstring val1)
	{
		ClearString();
		val.Pointer = new wstring(val1);
		type = WString;
	}

	bool operator>>(BaseIO& str)
	{
		return Write(&str);
	}

	bool Write(BaseIO* str, bool hidden = false)
	{
		int Integer;
		bool Boolean;
		double Double;
		float Float;
		unsigned int UInteger;
		long long LongInteger;
		unsigned long long ULongInteger;
		char Character;
		wchar_t WideCharacter;
		string s;
		wstring ws;
		short Short;
		unsigned short UShort;
		unsigned int sz = 0;
		bool good = false;
		unsigned short* tmchar = 0;

		switch(GetType())
		{
		case Variant::UShort:
			Character = GetType();
			good = str->Write(Character, &sz, RESERVEDNAME, hidden);
			UShort = *this;
			good = str->Write(UShort, &sz, RESERVEDNAME, hidden);
			break;
		case Variant::Short:
			Character = GetType();
			good = str->Write((Character), &sz, RESERVEDNAME, hidden);
			Short = *this;
			good = str->Write((Short), &sz, RESERVEDNAME, hidden);
			break;
		case Variant::UInt:
			Character = GetType();
			good = str->Write((Character), &sz, RESERVEDNAME, hidden);
			UInteger = *this;
			good = str->Write((UInteger), &sz, RESERVEDNAME, hidden);
			break;
		case Variant::Int:
			Character = GetType();
			good = str->Write((Character), &sz, RESERVEDNAME, hidden);
			Integer = *this;
			good = str->Write((Integer), &sz, RESERVEDNAME, hidden);
			break;
		case Variant::Bool:
			Character = GetType();
			good = str->Write((Character), &sz, RESERVEDNAME, hidden);
			Boolean = *this;
			good = str->Write((Boolean), &sz, RESERVEDNAME, hidden);
			break;
		case Variant::Dbl:
			Character = GetType();
			good = str->Write((Character), &sz, RESERVEDNAME, hidden);
			Double = *this;
			good = str->Write((Double), &sz, RESERVEDNAME, hidden);
			break;
		case Variant::Float:
			Character = GetType();
			good = str->Write((Character), &sz, RESERVEDNAME, hidden);
			Float = *this;
			good = str->Write((Float), &sz, RESERVEDNAME, hidden);
			break;
		case Variant::LInt:
			Character = GetType();
			good = str->Write((Character), &sz, RESERVEDNAME, hidden);
			LongInteger = *this;
			good = str->Write((LongInteger), &sz, RESERVEDNAME, hidden);
			break;
		case Variant::ULInt:
			Character = GetType();
			good = str->Write((Character), &sz, RESERVEDNAME, hidden);
			ULongInteger = *this;
			good = str->Write((ULongInteger), &sz, RESERVEDNAME, hidden);
			break;
		case Variant::Char:
			Character = GetType();
			good = str->Write((Character), &sz, RESERVEDNAME, hidden);
			Character = *this;
			good = str->Write((Character), &sz, RESERVEDNAME, hidden);
			break;
		case Variant::WChar:
			Character = GetType();
			good = str->Write((Character), &sz, RESERVEDNAME, hidden);
			WideCharacter = *this;
			good = str->Write((WideCharacter), &sz, RESERVEDNAME, hidden);
			break;
		case Variant::String:
			Character = GetType();
			good = str->Write(Character, &sz, RESERVEDNAME, hidden);
			s = static_cast<const char*>(*this);
			UInteger = s.size();
			good = str->Write(UInteger, &sz, RESERVEDNAME, hidden);
			sz = UInteger;
			good = str->Write(reinterpret_cast<const void*>(s.c_str()), &sz, RESERVEDNAME);
			break;
		case Variant::WString:
			Character = GetType();
			good = str->Write(Character, &sz, RESERVEDNAME, hidden);
			ws = static_cast<const wchar_t*>(*this);
			UInteger = ws.size() * sizeof(unsigned short);
			good = str->Write(UInteger, &sz, RESERVEDNAME, hidden);
			sz = UInteger;
			tmchar = new unsigned short[ws.size()];
			ConvertWcharTo16Bit(tmchar, ws.c_str(), ws.size());
			good = str->Write(reinterpret_cast<const void*>(tmchar), &sz, RESERVEDNAME);
			delete [] tmchar;
			break;
		}
		return good;
	}

	bool operator<<(BaseIO& str)
	{
		return Read(&str);
	}

	bool Read(BaseIO* str, bool hidden = false)
	{
		int Integer;
		bool Boolean;
		double Double;
		float Float;
		unsigned int UInteger;
		long long LongInteger;
		unsigned long long ULongInteger;
		char Character;
		wchar_t WideCharacter;
		string s;
		wstring ws;
		char type;
		char* chr;
		wchar_t* wchr;
		short Short;
		unsigned short UShort;
		unsigned int sz = 0;
		string chr2;
		wstring wchr2;
		bool good = false;
		unsigned short* twchar = 0;

		good = str->Read(type, &sz, hidden);
		if(!good) return false;
		switch(type)
		{
		case Variant::UShort:
			good = str->Read(UShort, &sz, hidden);
			*this = UShort;
			break;
		case Variant::Short:
			good = str->Read(Short, &sz, hidden);
			*this = Short;
			break;
		case Variant::UInt:
			good = str->Read(UInteger, &sz, hidden);
			*this = UInteger;
			break;
		case Variant::Int:
			good = str->Read(Integer, &sz, hidden);
			*this = Integer;
			break;
		case Variant::Bool:
			good = str->Read(Boolean, &sz, hidden);
			*this = Boolean;
			break;
		case Variant::Dbl:
			good = str->Read(Double, &sz, hidden);
			*this = Double;
			break;
		case Variant::Float:
			good = str->Read(Float, &sz, hidden);
			*this = Float;
			break;
		case Variant::LInt:
			good = str->Read(LongInteger, &sz, hidden);
			*this = LongInteger;
			break;
		case Variant::ULInt:				
			good = str->Read(ULongInteger, &sz, hidden);
			*this = ULongInteger;
			break;
		case Variant::Char:				
			good = str->Read(Character, &sz, hidden);
			*this = Character;
			break;
		case Variant::WChar:				
			good = str->Read(WideCharacter, &sz, hidden);
			*this = WideCharacter;
			break;
		case Variant::String:			
			good = str->Read(UInteger, &sz, hidden);
			chr = new char[UInteger+1];
			sz = UInteger;
			good = str->Read((void*)chr, &sz);
			chr[UInteger] = '\0';
			*this = chr;
			delete [] chr;
			break;
		case Variant::WString:
			good = str->Read(UInteger, &sz, hidden);
			twchar = new unsigned short[(UInteger)/sizeof(unsigned short) + sizeof(unsigned short)];
			wchr = new wchar_t[(UInteger)/sizeof(unsigned short) + sizeof(unsigned short)];
			sz = UInteger;
			good = str->Read(reinterpret_cast<void*>(twchar), &sz);
			twchar[UInteger/sizeof(unsigned short)] = L'\0';
			Convert16BitWchar(wchr, twchar, (UInteger)/sizeof(unsigned short) + sizeof(unsigned short));
			delete [] twchar;
			*this = wchr;
			delete [] wchr;
			break;
		}
		return good;
	}

	~Variant()
	{
		ClearString();
	}
protected:
	PType type;
};