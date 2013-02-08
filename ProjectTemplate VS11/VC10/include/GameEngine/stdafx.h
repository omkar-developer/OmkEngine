
#pragma once
#define USEBOX2DPHYSICS
#define USESCRIPTING
#define USESTREAMING
#define USE_BITMAPFONT
#define USE_SINGLETHREADLOADING
#define USE_OGG

#if defined (ANDROID) || defined(__AVM2__)
#undef _WIN32
#endif

#define THREADSAFE
//#define TTFONT

#ifdef __AVM2__
#define USE_FSTREAM
#define MAXPHYINTERATIONS 2
#else
#ifdef ANDROID
#define USE_GLES
#define USE_FSTREAM
#define MAXPHYINTERATIONS 2
#else
#define USE_DIRECTSOUND8
#define USE_DIRECT3D9
#define MAXPHYINTERATIONS 60
#endif
#endif

#ifdef USESTREAMING
#define MAXSBUFF 40960
#else
#define MAXSBUFF 5242880
#endif
#define MAX_VOL 10000.0f

#define BFONTSPACE 0
#define BFONTSPACEW 4
#define BFONTVSPACE 0

#ifdef __AVM2__
//#include <Flash++.h>
#include <AS3/AS3.h>
#else
#ifdef ANDROID
#else
#include "targetver.h"
#include <tchar.h>
#endif
#endif

#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <ctype.h>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <list>
#include <sstream>
#include <string>
#include <time.h>

//#include <zlib.h>
#include "unzip.h"

#ifdef __AVM2__
void PrintAVMLog(const std::string& str);
void OpenUrl(const std::string& str);
#else
#ifdef ANDROID
#else
#include <Windows.h>
#include <WindowsX.h>
#include <Shlobj.h>
#endif
#endif

#ifdef USE_FSTREAM
//#include <fstream>
#endif

#ifdef USE_DIRECT3D9
#include <d3d9.h>
#include <d3dx9.h>
#else
#ifdef USE_GLES
#include <GLES/gl.h>
//#include <EGL/egl.h>
#else
#include <gl/gl.h>
#include <gl/glu.h>
#endif
#endif

#ifdef __AVM2__

#else
#ifdef USE_DIRECTSOUND8
#include <dsound.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#endif

#ifdef USE_OGG
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#endif
using namespace std;

#ifdef USEBOX2DPHYSICS
#include <Box2D/Box2D.h>
#include "Box2DExt/b2Polygon.h"
#endif

#ifdef USESCRIPTING
#include <angelscript.h>
#include "AngelscriptExt/aswrappedcall.h"
#include <scriptarray.h>
#include <scriptstdstring.h>
#include <scriptmath.h>
#endif

#ifdef TTFONT
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#endif

#include "Core.h"
#include "Signals.h"
#include "UIWString.h"

#ifdef __AVM2__
#include "FlashPlatformMutex.h"
#else
#ifdef ANDROID
#include <Android/log.h>
#include <pthread.h>
#include <unistd.h>
#include "AndPlatformMutex.h"
#else
#include "WinPlatformMutex.h"
#endif
#endif

#include "SmartPtr.h"

#ifdef __AVM2__
#include "FlashPlatformThread.h"
#include "FlashTimer.h"
#include "FlashPlatformWindow.h"
#else
#ifdef ANDROID
#include "AndPlatformThread.h"
#include "AndTimer.h"
#include "AndPlatformWindow.h"
#else
#include "WinPlatformThread.h"
#include "WinTimer.h"
#include "WinPlatformWindow.h"
#endif
#endif

#ifdef USE_DIRECT3D9
#include "Dx9Device.h"
#else
#include "OpenGLDevice.h"
#endif

#ifdef USE_DIRECTSOUND8
#include "Dx8Sound.h"
#else
#ifdef __AVM2__
#include "FlashNativeSound.h"
#else
#include "NativeSound.h"
#endif
#endif

#include "BaseResource.h"
#include "BaseIO.h"

#ifdef __AVM2__
#include "PlatformFile.h"
#else
#ifdef ANDROID
#include "PlatformFile.h"
#else
#include "WinPlatformFile.h"
#endif
#endif

#include "ZipFile.h"
#include "Variant.h"

#ifdef TTFONT
#include "TTFont.h"
#endif

#ifdef USE_DIRECT3D9
#include "Dx9PlatformResource.h"
#include "Dx9Renderer.h"
#else
#ifdef __AVM2__
#include <libpng15/png.h>
#else
#include <png.h>
#endif
#ifdef __AVM2__
#include "FLOpenGLPlatformResource.h"
#else
#include "OpenGLPlatformResource.h"
#endif
#include "OpenGLRenderer.h"
#endif

#ifdef __AVM2__
#include "FlashSoundPlayer.h"
#else
#ifdef ANDROID
#include "SoundPlayer.h"
#else
#include "DxSoundPlayer.h"
#endif
#endif

#include "Sprite.h"
#include "Object.h"
#include "Animation.h"

#ifdef USEBOX2DPHYSICS
#include "Physics.h"
#endif

#ifdef USESCRIPTING
#include "Scripting.h"
#endif

#include "Game.h"