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

/** Global Constants \addtogroup GlobalConstants
* \{
*/
#define C_PI 3.1415926f ///< Constant PI value.
/* \} */

/** Structure of the rectangle holding integer vales for left,top,right,bottom properties. */
struct RectangleI
{
	int left; ///< X position of the rectable.
	int top; ///< Y position of the rectangle.
	int right; ///< Right side position of the rectangle.
	int bottom; ///< Buttom side position of the rectangle.

	
	/** Default constructor setting all values to 0. */
	RectangleI();

	/** Constructor with parameters.  
	\param left X position of the rectable.
	\param top Y position of the rectangle.
	\param right Right side position of the rectangle.
	\param bottom Buttom side position of the rectangle. */
	RectangleI(int left, int top, int right, int bottom);

	/** Adds values to the rectangle like left + rect.left for all properties.  
	\param rect Rectangle from which values to be added to this rectangle. */
	void operator+=(const RectangleI& rect);

	/** Adds values to the rectangle like left + rect.left for all properties but this function doesn't change current rectangle values.  
	\param rect Rectangle to be added to the returned rectangle.
	\return Rectangle with added values from the specified rectangle. */
	RectangleI operator+(const RectangleI& rect);
};

/** Structure of the rectangle holding float vales for left,top,right,bottom properties. */
struct RectangleF
{
	float left; ///< X position of the rectable.
	float top; ///< Y position of the rectangle.
	float right; ///< Right side position of the rectangle.
	float bottom; ///< Buttom side position of the rectangle.

	/** Default constructor setting all values to 0. */
	RectangleF();

	/** Constructor with parameters.  
	\param left X position of the rectable.
	\param top Y position of the rectangle.
	\param right Right side position of the rectangle.
	\param bottom Buttom side position of the rectangle. */
	RectangleF(float left, float top, float right, float bottom);

	/** Adds values to the rectangle like left + rect.left for all properties.  
	\param rect Rectangle from which values to be added to this rectangle. */
	void operator+=(RectangleF rect);

	/** Adds values to the rectangle like left + rect.left for all properties but this function doesn't change current rectangle values.  
	\param rect Rectangle to be added to the returned rectangle.
	\return Rectangle with added values from the specified rectangle. */
	RectangleF operator+(RectangleF rect);
};

/** Point structure with values X,Y. */
struct PointI
{
	int left; ///< X position of the point.
	int top; ///< Y position of the point.

	/** Default point constructor setting all values to 0. */
	PointI();


	/** Point constructor with parameters.  
	\param left X Position of the point.
	\param top Y Position of the point.	*/
	PointI(int left, int top);

	/** Adds values to the point like left + pt.left for all properties.  
	\param pt Point from which values to be added to this point. */
	void operator+=(PointI pt);

	/** Adds values to the point like left + pt.left for all properties but this function doesn't change current point values.  
	\param pt Point to be added to the returned point.
	\return Point with added values from the specified point. */
	PointI operator+(PointI pt);
	
	/** Checks if the specified point lies in the specified Rectangle.  
	\param point Point to be checked for intersection.
	\param rect Rectangle to be checked for intersection.
	\return true if point lies in rectangle, false otherwise. */
	static bool Intersect(PointI const& point, RectangleF const& rect);
};

/** Point structure with values X,Y. */
struct PointF
{
	float left; ///< X position of the point.
	float top; ///< Y position of the point.

	/** Default point constructor setting all values to 0. */
	PointF();


	/** Point constructor with parameters.  
	\param left X Position of the point.
	\param top Y Position of the point.	*/
	PointF(float left, float top);

	/** Adds values to the point like left + pt.left for all properties.  
	\param pt Point from which values to be added to this point. */
	void operator+=(PointF pt);

	/** Adds values to the point like left + pt.left for all properties but this function doesn't change current point values.  
	\param pt Point to be added to the returned point.
	\return Point with added values from the specified point. */
	PointF operator+(PointF pt);
	
	/** Checks if the specified point lies in the specified Rectangle.  
	\param point Point to be checked for intersection.
	\param rect Rectangle to be checked for intersection.
	\return true if point lies in rectangle, false otherwise. */
	static bool Intersect(PointF const& point, RectangleF const& rect);
};

/** Size structure with values width,height. */
struct Size
{
	unsigned int width; ///< Width value of the size.
	unsigned int height; ///< Height value of the size.

	/** Default size constructor setting width and height values to 0. */
	Size();


	/** size constructor with parameters.   
	\param width Width value of the size.
	\param height Height value of the size.*/
	Size(unsigned int width, unsigned int height);

	/** Adds values to the size like width + pt.width for all properties.  
	\param sz Size from which values to be added to this size. */
	void operator+=(PointI sz);

	/** Adds values to the size like width + sz.width for all properties but this function doesn't change current size values.  
	\param sz Size to be added to the new returned size.
	\return Size with added values from the specified size. */
	Size operator+(PointI sz);
};

/**
 * Fast inverse square root function.
 * \param number Number whose inverse square root is to be determined.
 * \return Inverse square root of the number.
 */
float fast_rsqrt( float number );

/** Color class with 32 bit ARGB values. */
class ColorARGB32
{
private:
	unsigned int m_colorB : 8; ///< Blue value.
	unsigned int m_colorG : 8; ///< Green value.
	unsigned int m_colorR : 8; ///< Red value.
	unsigned int m_colorA : 8; ///< Alpha value.

public:
	/** constructor with parameters.   
	\param a Alpha value.
	\param r Red value.
	\param g Green value.
	\param b Blue value.
	*/
	ColorARGB32(unsigned char a = 0xFF, unsigned char r = 0, unsigned char g=0, unsigned char b=0);

	/** constructor with parameters.   
	\param color integer value of the color.
	*/
	ColorARGB32(unsigned int color);

	/** Gets Alpha value of the color.
	\return Alpha value of the color. */
	unsigned char GetA();

	/** Gets Red value of the color.
	\return Red value of the color. */
	unsigned char GetR();

	/** Gets Green value of the color.
	\return Green value of the color. */
	unsigned char GetG();

	/** Gets Blue value of the color.
	\return Blue value of the color. */
	unsigned char GetB();

	/** Sets Alpha value of the color.
	\param val Alpha value of the color. */
	void SetA(unsigned char val);

	/** Sets Red value of the color.
	\param val Red value of the color. */
	void SetR(unsigned char val);

	/** Sets Green value of the color.
	\param val Green value of the color. */
	void SetG(unsigned char val);

	/** Sets Blue value of the color.
	\param val Blue value of the color. */
	void SetB(unsigned char val);

	/** Sets color from integer value.
	\param color integer value of the color. */
	void SetColor(unsigned int color);

	/** returns integer value of the color.
	\return integer value of the color. */
	operator unsigned int();
};

/** \addtogroup GlobalFunctions
* \{
*/

/** Gobal function populates map with name and value pairs from provided input.  
	\param lst map for output, if map is not empty it is not cleared.
	\param input Input string to be tokenized.
	\param delim1 First delimiter for seperating string into values, Example : Name=value;Name=value2 (here ';' is the first delimiter).
	\param delim2 Second delimiter to seperate string even further into name and value pairs, Example : Name=value (here '=' is the second delimiter).
*/
void GetStringMap(map<string, string>& lst, const char* input, char delim1 = ';', char delim2 = '=');

/** Gobal function populates vector with values from provided input.  
	\param lst vector for output, if vector is not empty it is not cleared.
	\param input Input string to be tokenized.
	\param delim1 First delimiter for seperate string into values, Example : value;value2 (here ';' is the first delimiter).
*/
void GetStringArray(vector<string>& lst, const char* input, char delim1 = ';');

/** Gobal function populates vector with values from provided input.  
	\param lst vector for output, if vector is not empty it is not cleared.
	\param input Input string to be tokenized.
	\param delim1 First delimiter for seperate string into values, Example : value;value2 (here ';' is the first delimiter).
*/
void GetStringArray(vector<int>& lst, const char* input, char delim1 = ';');

/** Gobal function populates vector with values from provided input.  
	\param lst vector for output, if vector is not empty it is not cleared.
	\param input Input string to be tokenized.
	\param delim1 First delimiter for seperate string into values, Example : value;value2 (here ';' is the first delimiter).
*/
void GetStringArray(vector<float>& lst, const char* input, char delim1 = ';');

/**
 * Converts wide character string to multibyte (wchar_t* to char*).
 * \param dest Destination - pointer to the character array to receive converted string.
 * \param src Source - pointer to the string (wchar_t array) to be converted.
 * \param max Size of the destination array.
 */
void WideCharToMultiByte(char* dest, const wchar_t* src, unsigned int max);

/**
 * Converts multibyte character string to wide character string (char* to wchar_t*).
 * \param dest Destination - pointer to the wide character array to receive converted string.
 * \param src Source - pointer to the multibyte character string array(const char*) to be converted.
 * \param max Size of the destination array.
 */
void MultiByteToWideChar(wchar_t* dest, const char* src, unsigned int max);

/**
 * Converts 16bit unsigned short array to wchar_t array.
 * \param dest Pointer to the wchar_t array.
 * \param src Pointer to the unsigned short array to be converted.
 * \param max Size of the destination array.
 */
void Convert16BitWchar(wchar_t* dest, const unsigned short* src, unsigned int max);

/**
 * Converts wide character array to unsigned short array (if wchar_t is of 4 bytes data might lost).
 * \param dest Destination - pointer to the unsigned short array.
 * \param src Pointer to the wchar_t* to be converted.
 * \param max Size of the destination array.
 */
void ConvertWcharTo16Bit(unsigned short* dest, const wchar_t* src, unsigned int max);

/**
 * Helper class for case insensitive comparison of the strings.
 */
struct icompstrs 
{
	/**
	 * Does case insensitive comparison of two specified strings.
	 * \param s1 First string to be compared.
	 * \param s2 Second string to be compared.
	 * \return true if both strings are same otherwise false.
	 */
	bool operator() (const string& s1, const string& s2) const;
};

/**
 * Wraps angle around max specified value.
 * Example : if angle is 91 and max is 90 then angle wraps to 1.
 * \param angle Angle to be wrapped.
 * \param max Max value of the angle.
 * \return Angle wrapped around max angle.
 */
float WrapAngle(float angle, float max);

/**
 * Converts to lower case string.
 * \param str Reference to the string to be converted.
 */
void StrToLower(string& str);

/**
 * Does case insensitive comparison of specified strings.
 * \param a s1 First string to be compared.
 * \param b s2 Second string to be compared.
 * \return true if both strings are same otherwise false.
 */
bool iequals(const string& a, const string& b);

/** \} */
