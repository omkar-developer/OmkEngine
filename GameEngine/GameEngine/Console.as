package com.adobe.flascc
{
	import flash.display.DisplayObjectContainer;
	import flash.display.Sprite;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.events.SampleDataEvent;
	import flash.net.LocalConnection;
	import flash.net.navigateToURL;
	import flash.net.URLRequest;
	import flash.utils.ByteArray;
	import flash.display.Stage3D;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.display3D.Context3D;
	import flash.display3D.Context3DProfile;
	import flash.display3D.Context3DRenderMode;
	import flash.media.Sound;
	import flash.media.SoundChannel;
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	import com.adobe.flascc.vfs.ISpecialFile;
	import com.adobe.flascc.vfs.LSOBackingStore;
	import com.adobe.flascc.vfs.RootFSBackingStore;
	
	import GLS3D.GLAPI;
	
	public class Console extends Sprite implements ISpecialFile
	{
		private var DispContainer:DisplayObjectContainer
	
		private var s3d:Stage3D;
		private var ctx3d:Context3D;
		
		private var audio:Sound;
		private var audioctrl:SoundChannel;
		
		private var mainloopTickPtr:int;
		
		private var mousedownfn:int;
		private var mouseupfn:int;
		private var mousemovefn:int;
		private var keydownfn:int;
		private var keyupfn:int;
		
		private var ackey:int;
		private var acx:int;
		private var acy:int;
		
		private var snddataptr:int;
		private var sndupdate:int;
		
		private var winactivated:int;
		private var windeactivated:int;
		
		private var winresize:int;
		private var winwidth:int;
		private var winheight:int;
		
		private var wexitapp:int;
		
		private var inited:Boolean = false;
		private const emptyVec:Vector.<int> = new Vector.<int>();
		
		public function Console(container:DisplayObjectContainer = null)
		{
			CModule.rootSprite = container ? container.root : this
			
			if (container)
			{
				container.addChild(this)
				init(null)
			}
			else
			{
				addEventListener(Event.ADDED_TO_STAGE, init)
			}
		}
		
		protected function context_created(e:Event):void
		{
			ctx3d = s3d.context3D
			ctx3d.configureBackBuffer(stage.stageWidth, stage.stageHeight, 2, false)
			ctx3d.enableErrorChecking = false;
			
			GLAPI.init(ctx3d, null, stage);
			
			CModule.write32(winwidth, stage.stageWidth);
			CModule.write32(winheight, stage.stageHeight);
				
			CModule.startAsync(this);
			
			mainloopTickPtr = CModule.getPublicSymbol("MainLoop");			
			keydownfn = CModule.getPublicSymbol("KeyDown");
			keyupfn = CModule.getPublicSymbol("KeyUp");
			mousedownfn = CModule.getPublicSymbol("MouseDown");
			mouseupfn = CModule.getPublicSymbol("MouseUp");
			mousemovefn = CModule.getPublicSymbol("MouseMove");
			
			ackey = CModule.getPublicSymbol("ackey");
			acx = CModule.getPublicSymbol("acx");
			acy = CModule.getPublicSymbol("acy");
			
			snddataptr = CModule.getPublicSymbol("sdataptr");
			sndupdate = CModule.getPublicSymbol("UpdateSData");
			
			winactivated = CModule.getPublicSymbol("WActivated");
			windeactivated = CModule.getPublicSymbol("WDeactivated");
			
			winresize = CModule.getPublicSymbol("WResized");
			winwidth = CModule.getPublicSymbol("wwidth");
			winheight = CModule.getPublicSymbol("wheight");
			wexitapp = CModule.getPublicSymbol("ExitApp");
			
			audioctrl = audio.play();
			inited = true;
		}
		
		protected function KeyDown(e:KeyboardEvent)
		{
			if (inited)
			{
				CModule.write32(ackey, e.keyCode);
				CModule.callI(keydownfn, emptyVec);
			}
		}
		
		protected function KeyUp(e:KeyboardEvent)
		{
			if (inited)
			{
				CModule.write32(ackey, e.keyCode);
				CModule.callI(keyupfn, emptyVec);
			}
		}
		
		protected function MouseDown(e:MouseEvent)
		{
			if (inited)
			{
				CModule.write32(acx, e.stageX);
				CModule.write32(acy, e.stageY);
				CModule.callI(mousedownfn, emptyVec);
			}
		}
		
		protected function MouseUp(e:MouseEvent)
		{
			if (inited)
			{
				CModule.write32(acx, e.stageX);
				CModule.write32(acy, e.stageY);
				CModule.callI(mouseupfn, emptyVec);
			}
		}
		
		protected function MouseMove(e:MouseEvent)
		{
			if (inited)
			{
				CModule.write32(acx, e.stageX);
				CModule.write32(acy, e.stageY);
				CModule.callI(mousemovefn, emptyVec);
			}
		}
		
		protected function SampleData(e:SampleDataEvent)
		{
			CModule.callI(sndupdate, emptyVec);
			
			e.data.endian = Endian.LITTLE_ENDIAN;
			e.data.length = 0;
			
			var pt:int = CModule.read32(snddataptr);
			
			for (var i:int = 0; i < 8192; i++)
			{
				CModule.ram.position = pt + i * 4;
				e.data.writeFloat(CModule.ram.readFloat());
			}			
		}
		
		protected function WDeactivated(e:Event)
		{
			if (audioctrl) audioctrl.stop();
			CModule.callI(windeactivated, emptyVec);
		}
		
		protected function WActivated(e:Event)
		{
			if (audioctrl) audioctrl.stop();
			audioctrl = audio.play();
			CModule.callI(winactivated, emptyVec);
		}
		
		protected function WResized(e:Event)
		{
			if (inited)
			{
				ctx3d.configureBackBuffer(stage.stageWidth, stage.stageHeight, 2, false);
				CModule.write32(winwidth, stage.stageWidth);
				CModule.write32(winheight, stage.stageHeight);
				CModule.callI(winresize, emptyVec);
			}
		}
		
		protected function OnClosing(e:Event)
		{
			trace("On Closing  . . .");
			CModule.callI(keydownfn, wexitapp);
		}
		
		protected function init(e:Event):void
		{
			DispContainer = new Sprite()
			addChild(DispContainer)
			
			audio = new Sound();
			audio.addEventListener(SampleDataEvent.SAMPLE_DATA, SampleData);
			
			addEventListener(Event.ENTER_FRAME, enterFrame)
			
			stage.frameRate = 60;
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			stage.addEventListener(Event.DEACTIVATE, WActivated);
			stage.addEventListener(Event.DEACTIVATE, WDeactivated);
			stage.addEventListener(Event.RESIZE, WResized);
			stage.addEventListener(Event.REMOVED_FROM_STAGE, OnClosing);
			
			stage.addEventListener(KeyboardEvent.KEY_DOWN, KeyDown);
			stage.addEventListener(KeyboardEvent.KEY_UP, KeyUp);
			stage.addEventListener(MouseEvent.MOUSE_MOVE, MouseMove);
			stage.addEventListener(MouseEvent.MOUSE_DOWN, MouseDown);
			stage.addEventListener(MouseEvent.MOUSE_UP, MouseUp);
			
			s3d = stage.stage3Ds[0];
			s3d.addEventListener(Event.CONTEXT3D_CREATE, context_created);
			s3d.requestContext3D(Context3DRenderMode.AUTO)
			
			CModule.vfs.console = this
			CModule.vfs.addBackingStore(new RootFSBackingStore(), null)
			CModule.vfs.addDirectory("/local")
			CModule.vfs.addBackingStore(new LSOBackingStore("localvfsLSO"), "/local")
		}
		
		public static function OpenUrl(urlname:String)
		{
			var url:URLRequest = new URLRequest(urlname);
			navigateToURL(url, "_blank");
		}
		
		public function exit(code:int):Boolean
		{
			trace("Exiting");
			return false;
		}
		
		public function write(fd:int, bufPtr:int, nbyte:int, errnoPtr:int):int
		{
			var str:String = CModule.readString(bufPtr, nbyte)
			return nbyte
		}
		
		public function read(fd:int, bufPtr:int, nbyte:int, errnoPtr:int):int
		{
			return 0
		}
		
		public function fcntl(fd:int, com:int, data:int, errnoPtr:int):int
		{
			return 0
		}
		
		public function ioctl(fd:int, com:int, data:int, errnoPtr:int):int
		{
			return 0
		}
		
		protected function enterFrame(e:Event):void
		{
			CModule.serviceUIRequests()
			
			if (inited)
			{
				CModule.callI(mainloopTickPtr, emptyVec);
				GLAPI.instance.context.present();
			}
		}
	}
}