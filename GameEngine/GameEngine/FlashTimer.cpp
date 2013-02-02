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
//
//C_Timer::C_Timer()
//{
//	frames = 0;
//}
//
//void C_Timer::Start()
//{
//	gettimeofday(&start, 0);
//}
//
//float C_Timer::Reset()
//{
//	while(etime>5000)
//	{
//		frames -= static_cast<unsigned int>(FPS());
//		etime = etime - 1000;
//	}
//
//	timeval end;
//	gettimeofday(&end, 0);
//
//	float seconds = end.tv_sec - start.tv_sec;
//	float useconds = end.tv_usec - start.tv_usec;
//
//	double diff = seconds * 1000.0 + useconds / 1000.0;
//
//	etime += (unsigned int) diff;
//	float delta = (float)(diff / 1000.0);
//	start = end;
//	++frames;
//	return delta;
//}
//
//unsigned int C_Timer::FPS()
//{
//	unsigned int fps = 0;
//	if(etime!=0) fps = static_cast<unsigned int>(frames / ( etime / 1000.0f)); 
//	return fps;
//}

#include "stdafx.h"

C_Timer::C_Timer()
{
	frames = 0;
}

void C_Timer::Start()
{
	start = clock();
}

float C_Timer::Reset()
{
	while(etime>5000)
	{
		frames -= static_cast<unsigned int>(FPS());
		etime = etime - 1000;
	}

	clock_t end = clock();

	float diff = end - start;

	etime += (unsigned int) (diff/(float)(CLOCKS_PER_SEC)*1000.0f);
	float delta = (float)(end - start)/CLOCKS_PER_SEC;
	start = end;
	++frames;
	return delta;
}

unsigned int C_Timer::FPS()
{
	unsigned int fps = 0;
	if(etime!=0) fps = static_cast<unsigned int>(frames / ( etime / 1000.0f)); 
	return fps;
}