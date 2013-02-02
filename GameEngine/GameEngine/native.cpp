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
#include <string.h>
#include <math.h>
#include <jni.h>
#include <android/log.h>
#include <GLES/gl.h>
#include <vector>
#include <fstream>
#include <zlib.h>
#include <png.h>
#include <Android/log.h>
#include <dirent.h>
#include <sys/stat.h>
using namespace std;

GameWorld* GetGameAPI(const wchar_t* title, int winwidth, int winheight, bool fullscreen);

extern "C" 
{
	int dwidth = 800, dheight = 480;
	string m_savepath;
	bool menabled = true;
	bool senabled = true;
	JavaVM* m_VM = 0;

	jclass m_MainClass = 0;
	jmethodID MID_ShowDialog = 0;
	jmethodID MID_GoToWebsite = 0;
	jmethodID MID_Purchase = 0;
	jmethodID MID_CheckBillingSupport = 0;
	jmethodID MID_PlayMusic = 0;
	jmethodID MID_StopMusic = 0;
	jmethodID MID_SetMusicVolume = 0;
	jmethodID MID_ShowKeys = 0;
	jmethodID MID_HideKeys = 0;


bool DirectoryExists(const char* path)
{
    if(path==0) return false;

    DIR *pDir = 0;

    pDir = opendir (path);
    if (pDir!=0)
    {
       closedir(pDir);
	   return true;
	}
    return false;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) 
	{
        return -1;
    }
	m_VM = vm;

	m_MainClass = env->FindClass("com/omkengine/engine/GameEngineActivity");
	if(m_MainClass==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "class not found");
		return JNI_VERSION_1_6;
	}

	MID_ShowDialog = env->GetStaticMethodID(m_MainClass, "ShowDialog", "(Ljava/lang/String;Ljava/lang/String;)V");
	if(MID_ShowDialog==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "ShowDialog Method not found");
	}

	MID_GoToWebsite = env->GetStaticMethodID(m_MainClass, "GoToWebsite", "(Ljava/lang/String;)V");
	if(MID_GoToWebsite==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "GoToWebsite Method not found");
	}

	MID_Purchase = env->GetStaticMethodID(m_MainClass, "InitiatePurchase", "(Ljava/lang/String;)V");
	if(MID_Purchase==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "InitiatePurchase Method not found");
	}

	MID_CheckBillingSupport = env->GetStaticMethodID(m_MainClass, "CheckBillingSupport", "()Z");
	if(MID_CheckBillingSupport==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "CheckBillingSupport Method not found");
	}

	//keyboard
	MID_ShowKeys = env->GetStaticMethodID(m_MainClass, "ShowKeys", "()V");
	if(MID_ShowKeys==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "ShowKeys Method not found");
	}

	MID_HideKeys = env->GetStaticMethodID(m_MainClass, "HideKeys", "()V");
	if(MID_HideKeys==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "HideKeys Method not found");
	}

	//music

	MID_PlayMusic = env->GetStaticMethodID(m_MainClass, "PlayMusic", "(Ljava/lang/String;)V");
	if(MID_PlayMusic==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "MID_PlayMusic Method not found");
	}

	MID_StopMusic = env->GetStaticMethodID(m_MainClass, "StopMusic", "()V");
	if(MID_StopMusic==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "MID_StopMusic Method not found");
	}

	MID_SetMusicVolume = env->GetStaticMethodID(m_MainClass, "SetMusicVolume", "(F)V");
	if(MID_SetMusicVolume==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "MID_SetMusicVolume Method not found");
	}

    return JNI_VERSION_1_6;
}

JNIEnv* JNI_GetEnv() 
{
	if(m_VM==0) return 0;
    JNIEnv *env;
    int ret = m_VM->AttachCurrentThread(&env, NULL);
    if(ret < 0) return 0;
    return env;
}

void Android_ShowKeys()
{
	if(MID_ShowKeys==0) return;
	JNIEnv* env = JNI_GetEnv();
	if(env==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "JNI Env was null");
		return;
	}
	
	env->CallStaticVoidMethod(m_MainClass, MID_ShowKeys);
}

void Android_HideKeys()
{
	if(MID_HideKeys==0) return;
	JNIEnv* env = JNI_GetEnv();
	if(env==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "JNI Env was null");
		return;
	}
	
	env->CallStaticVoidMethod(m_MainClass, MID_HideKeys);
}

bool Android_CheckBillingSupport()
{
	if(MID_CheckBillingSupport==0) return false;
	JNIEnv* env = JNI_GetEnv();
	if(env==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "JNI Env was null");
		return false;
	}
	
	jboolean ret = env->CallStaticBooleanMethod(m_MainClass, MID_CheckBillingSupport);
	return ret;
}

void Android_SetMusicVolume(float val)
{
	if(MID_SetMusicVolume==0) return;
	JNIEnv* env = JNI_GetEnv();
	if(env==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "JNI Env was null");
		return;
	}
	
	env->CallStaticVoidMethod(m_MainClass, MID_SetMusicVolume, val);
}

void Android_PlayMusic(const char* v_path)
{
	if(MID_PlayMusic==0) return;
	JNIEnv* env = JNI_GetEnv();
	if(env==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "JNI Env was null");
		return;
	}

	jstring path = env->NewStringUTF(v_path);
	
	env->CallStaticVoidMethod(m_MainClass, MID_PlayMusic, path);

	env->DeleteLocalRef(path);
}

void Android_StopMusic()
{
	if(MID_StopMusic==0) return;
	JNIEnv* env = JNI_GetEnv();
	if(env==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "JNI Env was null");
		return;
	}
	
	env->CallStaticVoidMethod(m_MainClass, MID_StopMusic);
}

void Android_Purchase(const char* v_pid)
{
	if(MID_Purchase==0) return;
	JNIEnv* env = JNI_GetEnv();
	if(env==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "JNI Env was null");
		return;
	}

	jstring pid = env->NewStringUTF(v_pid);
	
	env->CallStaticVoidMethod(m_MainClass, MID_Purchase, pid);

	env->DeleteLocalRef(pid);
}

void Android_GoToWebsite(const char* v_url)
{
	if(MID_GoToWebsite==0) return;
	JNIEnv* env = JNI_GetEnv();
	if(env==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "JNI Env was null");
		return;
	}

	jstring weburl = env->NewStringUTF(v_url);
	
	env->CallStaticVoidMethod(m_MainClass, MID_GoToWebsite, weburl);

	env->DeleteLocalRef(weburl);
}

void Android_ShowDialog(const char* Title, const char* Message)
{
	if(MID_ShowDialog==0) return;
	JNIEnv* env = JNI_GetEnv();
	if(env==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "JNI Env was null");
		return;
	}

	jstring title = env->NewStringUTF(Title);
	jstring msg = env->NewStringUTF(Message);
	
	env->CallStaticVoidMethod(m_MainClass, MID_ShowDialog, title, msg);

	env->DeleteLocalRef(title);
	env->DeleteLocalRef(msg);
}

void Java_com_omkengine_engine_GameEngineActivity_onbillingchecked( JNIEnv* env, jobject obj, jboolean val )
{
	if(val)
	{
		//__android_log_print(ANDROID_LOG_VERBOSE, "native", ">>>>>>>>>>>>>>>>. >>>>>>>>>>>>>>>>>>>>>>>>>>>>> Billing supported . . . . .");
	}
	else
	{
		//__android_log_print(ANDROID_LOG_VERBOSE, "native", ">>>>>>>>>>>>>>>>. >>>>>>>>>>>>>>>>>>>>>>>>>>>>> Billing not supported . . . . .");
	}
}

jstring Java_com_omkengine_engine_GameEngineActivity_checkpurchased( JNIEnv* env, jobject obj )
{
	return env->NewStringUTF("");
}

void Java_com_omkengine_engine_GameEngineActivity_purchased( JNIEnv* env, jobject obj, jboolean val, jstring prod )
{
	
}

void Java_com_omkengine_engine_GameEngineActivity_notpurchased( JNIEnv* env, jobject obj, jstring prod )
{
	
}

jboolean Java_com_omkengine_engine_GameEngineActivity_isconsumable( JNIEnv* env, jobject obj, jstring prod )
{
	return false;
}

void Java_com_omkengine_engine_GameEngineActivity_consumed( JNIEnv* env, jobject obj, jstring prod )
{
	
}

void Java_com_omkengine_engine_EGLSurfaceView_mevent( JNIEnv* env, jobject obj, jint action, jfloat x, jfloat y )
{
	if(Game!=0)
	{
		if(action == 1)
		{
			Game->GetMainContainer()->MouseDown(KEY_LBUTTON, 0, PointI((int)x, (int)y));
		}
		else if(action == 2)
		{
			Game->GetMainContainer()->MouseMove(PointI((int)x, (int)y));
		}
		else if(action == 3)
		{
			Game->GetMainContainer()->MouseUp(KEY_LBUTTON, 0, PointI((int)x, (int)y));
		}
	}
}

jboolean Java_com_omkengine_engine_EGLSurfaceView_keydown( JNIEnv* env, jobject obj, jint key )
{
	if(Game!=0)
	{
		Game->GetMainContainer()->KeyDown(key);
	}
	return true;
}

jboolean Java_com_omkengine_engine_EGLSurfaceView_keyup( JNIEnv* env, jobject obj, jint key )
{
	if(Game!=0)
	{
		Game->GetMainContainer()->KeyUp(key);
	}
	return true;
}

void Java_com_omkengine_engine_EGLRenderer_ninit( JNIEnv* env, jobject obj )
{
	__android_log_print(ANDROID_LOG_VERBOSE, "native", "Surface init . . . . .");
	if(Game!=0)
	{
		Game->GetDevice()->ResetDevice();
		Game->ResetTimer();
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "Device Reset . . . . .");
	}

	if(Game==0)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "Device starting . . . . .");
		Game = GetGameAPI(L"Editor", dwidth, dheight, false);
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "Device started . . . . .");
		wchar_t tstr[256];
		MultiByteToWideChar(tstr, m_savepath.c_str(), 256);
		Game->m_savedir = tstr;
		Game->GetMainContainer()->SetEditable(false);
		Game->ShowFPS = false;
		Game->m_DebugDraw = false;
		Game->DrawBoxes = false;
	}
}

jint Java_com_omkengine_engine_EGLRenderer_ndrawframe( JNIEnv* env, jobject obj )
{
	if(Game!=0)
	{
		if(Game->m_closing)
		{
			return 1;
		}
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(Game!=0) Game->Loop();
	/*if(Game!=0 && Game->m_lopped == true)
	{
		Game->Init();
		Game->ShowFPS = true;
	}*/
	if(Game!=0)
	Game->m_lopped = true;
	return 0;
}

void Java_com_omkengine_engine_EGLRenderer_nresize( JNIEnv* env, jobject obj, jint w, jint h )
{
	__android_log_print(ANDROID_LOG_VERBOSE, "native", "Resizing  . . .  %d, %d", w, h);

	if(Game!=0)
	{
		Game->GetDevice()->SetBackBufferSize(Size(w, h));
		Game->GetMainContainer()->AdjustSize(*Game->GetDevice());
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "Resized %d, %d", w, h);
	}
}

void Java_com_omkengine_engine_GameEngineActivity_onstop( JNIEnv* env, jobject obj )
{
	
}

void Java_com_omkengine_engine_GameEngineActivity_onpause( JNIEnv* env, jobject obj )
{
	if(Game!=0)
	{
		Game->GetDevice()->OnDeviceLost();
		Game->Pause();
		menabled = Game->settings.GetMusic();
		senabled = Game->settings.m_ensound;
		Game->EnableMusic(false);
		Game->EnableSound(false);
	}
}

void Java_com_omkengine_engine_GameEngineActivity_onresume( JNIEnv* env, jobject obj )
{
	if(Game!=0)
	{
		Game->EnableMusic(menabled);
		Game->EnableSound(senabled);
		Game->splayer.Play();
	}
}

void Java_com_omkengine_engine_GameEngineActivity_ondestroy( JNIEnv* env, jobject obj )
{
	if(Game!=0) 
	{
		Game->QuitApp();
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "Releasing . . . . .");
		//Game->ReleaseAll();
		delete Game;
		__android_log_print(ANDROID_LOG_VERBOSE, "native", "Released . . . . .");
		Game = 0;
	}
}

void Java_com_omkengine_engine_GameEngineActivity_oninit( JNIEnv* env, jobject obj, jstring str, jstring str2 )
{
	__android_log_print(ANDROID_LOG_VERBOSE, "native", "Init . . . . .");
	jboolean copys; 
	const char* buff = env->GetStringUTFChars(str, &copys);
	const char* buff2 = env->GetStringUTFChars(str2, &copys);
	if(ZipFile::OpenZipFile(buff))
	__android_log_print(ANDROID_LOG_VERBOSE, "native", ">>>>>>>>>>>>>>>>>>>>>>>%s", buff);
	ZipFile2::OpenZipFile(buff);
	if(!DirectoryExists(buff2))
	{
		mkdir(buff2, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
	m_savepath = buff2;
	env->ReleaseStringUTFChars(str, buff);
	env->ReleaseStringUTFChars(str, buff2);
}

}

void OpenURL(std::string str)
{
	Android_GoToWebsite(str.c_str());
}