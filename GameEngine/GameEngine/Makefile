GAME_FILES := jni/GameCode.cpp

GameEngine : check
	@echo && echo "Compiling virtual file system . . ."
	
	"$(FLASCC)/usr/bin/genfs" vfs enginevfs
	cp "$(FLASCC)/usr/share/LSOBackingStore.as" .

	$(AS3COMPILERARGS) -optimize \
				-import $(call nativepath,$(FLASCC)/usr/lib/builtin.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/playerglobal.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/BinaryData.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/ISpecialFile.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/IBackingStore.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/IVFS.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/InMemoryBackingStore.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/PlayerKernel.abc) \
				LSOBackingStore.as enginevfsRootFSBackingStore.as -outdir . -out VFS
				
	$(AS3COMPILERARGS) -optimize \
				-import $(call nativepath,$(FLASCC)/usr/lib/builtin.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/libGL.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/playerglobal.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/BinaryData.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/ISpecialFile.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/IBackingStore.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/IVFS.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/InMemoryBackingStore.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/PlayerKernel.abc) \
				-import $(call nativepath,$(FLASCC)/usr/lib/CModule.abc) \
				-import VFS.abc \
				Console.as -outdir . -out Console

	@echo && echo "Compiling C++ Code to swf . . ."
	"$(FLASCC)/usr/bin/g++" -jvmopt=-Xmx1G -fllvm-opt-opt=-strip -O1 -s -fno-strict-aliasing -I. -IDependencies/include FLmain.cpp ZipFile.cpp Animation.cpp BaseIO.cpp BaseResource.cpp Camera.cpp Core.cpp FlashNativeSound.cpp FlashPlatformThread.cpp FlashPlatformWindow.cpp FlashSoundPlayer.cpp FlashTimer.cpp Game.cpp Object.cpp OpenGLDevice.cpp FLOpenGLPlatformResource.cpp OpenGLRenderer.cpp Physics.cpp PlatformFile.cpp Scripting.cpp Signals.cpp Sprite.cpp stdafx.cpp UIWString.cpp Box2DExt/b2Polygon.cpp Box2DExt/b2Triangle.cpp $(GAME_FILES) VFS.abc -LDependencies/lib/Flascc/Release -lGL -lminizip -lz -lpng15 -langelscript -langeladdon -lBox2D -lvorbisfile -lvorbis -logg $(FLASCC)/usr/lib/AlcVFSZip.abc -symbol-abc=Console.abc -emit-swf -swf-size=800x480 -o Game.swf

include ./Makefile.common

clean:
	rm -f Game.swf main *.bc *.exe
