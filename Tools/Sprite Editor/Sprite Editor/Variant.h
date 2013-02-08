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

typedef class Serializer
{
public:
	virtual void Save(ostream& str)=0;
	virtual void Load(istream& str)=0;
}* LPSerializer;

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

	LPSerializer GetSerializer()
	{
		return static_cast<LPSerializer>(val.Pointer);
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

	Variant& operator=(LPSerializer val1)
	{
		ClearString();
		val.Pointer = val1;
		type = TSerializer;
		return *this;
	}
	//

	Variant(LPSerializer val1)
	{
		ClearString();
		val.Pointer = val1;
		type = TSerializer;
	}

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

	bool operator>>(ostream& str)
	{
		return Write(&str);
	}

	bool Write(ostream* str)
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

		switch(GetType())
		{
		case Variant::UShort:
			Character = GetType();
			str->write(reinterpret_cast<char*>(&Character), sizeof(char));
			UShort = *this;
			str->write(reinterpret_cast<char*>(&UShort), sizeof(unsigned short));
			break;
		case Variant::Short:
			Character = GetType();
			str->write(reinterpret_cast<char*>(&Character), sizeof(char));
			Short = *this;
			str->write(reinterpret_cast<char*>(&Short), sizeof(short));
			break;
		case Variant::UInt:
			Character = GetType();
			str->write(reinterpret_cast<char*>(&Character), sizeof(char));
			UInteger = *this;
			str->write(reinterpret_cast<char*>(&UInteger), sizeof(unsigned int));
			break;
		case Variant::Int:
			Character = GetType();
			str->write(reinterpret_cast<char*>(&Character), sizeof(char));
			Integer = *this;
			str->write(reinterpret_cast<char*>(&Integer), sizeof(int));
			break;
		case Variant::Bool:
			Character = GetType();
			str->write(reinterpret_cast<char*>(&Character), sizeof(char));
			Boolean = *this;
			str->write(reinterpret_cast<char*>(&Boolean), sizeof(bool));
			break;
		case Variant::Dbl:
			Character = GetType();
			str->write(reinterpret_cast<char*>(&Character), sizeof(char));
			Double = *this;
			str->write(reinterpret_cast<char*>(&Double), sizeof(double));
			break;
		case Variant::Float:
			Character = GetType();
			str->write(reinterpret_cast<char*>(&Character), sizeof(char));
			Float = *this;
			str->write(reinterpret_cast<char*>(&Float), sizeof(float));
			break;
		case Variant::LInt:
			Character = GetType();
			str->write(reinterpret_cast<char*>(&Character), sizeof(char));
			LongInteger = *this;
			str->write(reinterpret_cast<char*>(&LongInteger), sizeof(long long));
			break;
		case Variant::ULInt:
			Character = GetType();
			str->write(reinterpret_cast<char*>(&Character), sizeof(char));
			ULongInteger = *this;
			str->write(reinterpret_cast<char*>(&ULongInteger), sizeof(unsigned long long));
			break;
		case Variant::Char:
			Character = GetType();
			str->write(reinterpret_cast<char*>(&Character), sizeof(char));
			Character = *this;
			str->write(reinterpret_cast<char*>(&Character), sizeof(char));
			break;
		case Variant::WChar:
			Character = GetType();
			str->write(reinterpret_cast<char*>(&Character), sizeof(char));
			WideCharacter = *this;
			str->write(reinterpret_cast<char*>(&WideCharacter), sizeof(wchar_t));
			break;
		case Variant::String:
			Character = GetType();
			str->write(reinterpret_cast<char*>(&Character), sizeof(char));
			s = static_cast<const char*>(*this);
			UInteger = s.size();
			str->write(reinterpret_cast<char*>(&UInteger), sizeof(unsigned int));
			str->write(static_cast<const char*>(s.c_str()), UInteger);
			break;
		case Variant::WString:
			Character = GetType();
			str->write(reinterpret_cast<char*>(&Character), sizeof(char));
			ws = static_cast<const wchar_t*>(*this);
			UInteger = ws.size() * sizeof(wchar_t);
			str->write(reinterpret_cast<char*>(&UInteger), sizeof(unsigned int));
			str->write(reinterpret_cast<char*>(const_cast<wchar_t*>(ws.c_str())), UInteger);
			break;
		case Variant::TSerializer:
			Character = GetType();
			str->write(reinterpret_cast<char*>(&Character), sizeof(char));
			GetSerializer()->Save(*str);
			break;
		}
		return str->good();
	}

	bool operator<<(istream& str)
	{
		return Read(&str);
	}

	bool Read(istream* str)
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

		str->read(reinterpret_cast<char*>(&type), sizeof(char));
		switch(type)
		{
		case Variant::UShort:
			str->read(reinterpret_cast<char*>(&UShort), sizeof(unsigned short));
			*this = UShort;
			break;
		case Variant::Short:
			str->read(reinterpret_cast<char*>(&Short), sizeof(short));
			*this = Short;
			break;
		case Variant::UInt:
			str->read(reinterpret_cast<char*>(&UInteger), sizeof(unsigned int));
			*this = UInteger;
			break;
		case Variant::Int:
			str->read(reinterpret_cast<char*>(&Integer), sizeof(int));
			*this = Integer;
			break;
		case Variant::Bool:
			str->read(reinterpret_cast<char*>(&Boolean), sizeof(bool));
			*this = Boolean;
			break;
		case Variant::Dbl:
			str->read(reinterpret_cast<char*>(&Double), sizeof(double));
			*this = Double;
			break;
		case Variant::Float:
			str->read(reinterpret_cast<char*>(&Float), sizeof(float));
			*this = Float;
			break;
		case Variant::LInt:
			str->read(reinterpret_cast<char*>(&LongInteger), sizeof(long long));
			*this = LongInteger;
			break;
		case Variant::ULInt:				
			str->read(reinterpret_cast<char*>(&ULongInteger), sizeof(unsigned long long));
			*this = ULongInteger;
			break;
		case Variant::Char:				
			str->read(reinterpret_cast<char*>(&Character), sizeof(char));
			*this = Character;
			break;
		case Variant::WChar:				
			str->read(reinterpret_cast<char*>(&WideCharacter), sizeof(wchar_t));
			*this = WideCharacter;
			break;
		case Variant::String:
			str->read(reinterpret_cast<char*>(&UInteger), sizeof(unsigned int));
			chr = new char[UInteger+1];
			str->read(chr, UInteger);
			chr[UInteger] = '\0';
			*this = chr;
			delete chr;
			break;
		case Variant::WString:
			str->read(reinterpret_cast<char*>(&UInteger), sizeof(unsigned int));
			wchr = new wchar_t[(UInteger)/sizeof(wchar_t) + sizeof(wchar_t)];
			str->read(reinterpret_cast<char*>(wchr), UInteger);
			wchr[UInteger/sizeof(wchar_t)] = L'\0';
			*this = wchr;
			delete wchr;
			break;
		case Variant::TSerializer:
			if(GetType()==Variant::TSerializer)GetSerializer()->Load(*str);
			break;
		}
		return str->good();
	}

	~Variant()
	{
		ClearString();
	}
protected:
	PType type;
};