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

package com.omkengine.engine;

import java.io.IOException;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.omkengine.engine.SimpleEula;

import com.omkengine.util.IabHelper;
import com.omkengine.util.IabResult;
import com.omkengine.util.Inventory;
import com.omkengine.util.Purchase;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;

import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.media.MediaPlayer;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.content.pm.PackageInfo;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;

import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;

public class GameEngineActivity extends Activity
{
	public static Activity m_avtivity;
	private static EGLSurfaceView view;
	private static Context mContext;
	public static Boolean billingsupport = false;
	private static Boolean Restored = true;
	public static MediaPlayer musicplayer;
	
	public static IabHelper billhelper;
	static final int RC_REQUEST = 10001;
	
	public String m_elua = "";
	
    @Override
    public void onCreate(Bundle savedInstanceState) 
	{
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        Log.v("activity", " >>>>>>>>>>>>>>>>>>>>>>>>>>>. On create");
        view = new EGLSurfaceView(this);
        view.renderer.app = this;
        view.setFocusableInTouchMode(true);
        setContentView(view);
        
        if(m_elua != "")
        new SimpleEula(this, this.getTitle().toString(), m_elua).show();

		String apkFilePath = null;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = this.getPackageManager();

		try 
		{
			appInfo = packMgmr.getApplicationInfo(getApplication().getPackageName(), 0);
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
			throw new RuntimeException("Unable to locate assets, aborting...");
		}
		
		mContext = this;
		m_avtivity = this;
		
		musicplayer = new MediaPlayer();
		
		billhelper = new IabHelper(this, GetPublicKey());
		onbillingchecked(false);
		billhelper.startSetup(new IabHelper.OnIabSetupFinishedListener() 
		{
            public void onIabSetupFinished(IabResult result) 
            {	
                if(result.isSuccess()) 
                {
                	if(view != null)
            		{
            			view.queueEvent(new Runnable()
            			{
            				public void run()
            				{
            					onbillingchecked(true);
            				}
            			});
            		}
                	billingsupport = true;
                	billhelper.queryInventoryAsync(InventoryListener);
                }
            }
        });

		apkFilePath = appInfo.sourceDir;
		oninit(apkFilePath, getFilesDir().getAbsolutePath());
	}
    
    public static String GetPublicKey()
    {
    	return "pub key empty";
    }
    
    IabHelper.QueryInventoryFinishedListener InventoryListener = new IabHelper.QueryInventoryFinishedListener() 
    {
        public void onQueryInventoryFinished(IabResult result, Inventory inventory) 
        {
            if(result.isFailure()) 
            {
                ShowDialog("Error", "Failed to query purchased items.");
                return;
            }
            
            String pname = checkpurchased();
            
            while(pname!="")
            {
            	if(inventory.hasPurchase(pname))
            	{
            		final String pdname = pname;
            		if(isconsumable(pdname))
            		{
            			billhelper.consumeAsync(inventory.getPurchase(pname), ConsumeFinished);	            		      
            		}
            		else
            		{
            			if(view != null)
	            		{
	            			view.queueEvent(new Runnable()
	            			{
	            				public void run()
	            				{
	            					purchased(pdname);
	            				}
	            			});
	            		} 
            		}
            	}
            	else
            	{
            		final String pdname = pname;
            		if(view != null)
            		{
            			view.queueEvent(new Runnable()
            			{
            				public void run()
            				{
            					notpurchased(pdname);
            				}
            			});
            		}    
            	}
            	pname = checkpurchased();
            }
        }
    };
    
    static IabHelper.OnConsumeFinishedListener ConsumeFinished = new IabHelper.OnConsumeFinishedListener() 
    {
        public void onConsumeFinished(Purchase purchase, IabResult result) 
        {
            if (result.isSuccess()) 
            {
            	final String pdname = purchase.getSku();
        		if(view != null)
        		{
        			view.queueEvent(new Runnable()
        			{
        				public void run()
        				{
        					consumed(pdname);
        				}
        			});
        		}  
            }
            else 
            {
                ShowDialog("Error", "Purchase Failed");
            }
        }
    };
    
    public static void ShowKeys()
    {
    	InputMethodManager imm = (InputMethodManager) m_avtivity.getSystemService(Context.INPUT_METHOD_SERVICE);
    	imm.toggleSoftInput(InputMethodManager.SHOW_FORCED,0);
    }
    
    public static void HideKeys()
    {
    	InputMethodManager imm = (InputMethodManager) m_avtivity.getSystemService(Context.INPUT_METHOD_SERVICE);
    	imm.hideSoftInputFromWindow(view.getWindowToken(),0); 
    }
    
    public static void PlayMusic(String path)
    {
    	AssetFileDescriptor descriptor;
    	AssetManager mngr = mContext.getAssets();
    	try
		{
			descriptor = mngr.openFd( path );
			if(musicplayer.isPlaying()) musicplayer.stop();
			musicplayer.reset();
			musicplayer.setDataSource(descriptor.getFileDescriptor(), descriptor.getStartOffset(),  descriptor.getLength());
			descriptor.close();
			musicplayer.setLooping(true);
			musicplayer.prepare();
			musicplayer.start();
		} 
    	catch (IOException e)
		{
			Log.v("activity", " error opening music file.");
		}
    }
    
    public static void StopMusic()
    {
    	if(musicplayer.isPlaying()) musicplayer.stop();
    }
    
    public static void SetMusicVolume(float vol)
    {
    	musicplayer.setVolume(vol, vol);
    }
    
    public static void GoToWebsite(String m_url)
    {
    	Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(m_url));
		m_avtivity.startActivity(browserIntent);
    }
    
    public static void InitiatePurchase(String pid)
    {
    	billhelper.launchPurchaseFlow(m_avtivity, pid, RC_REQUEST, PurchasedListener);
    }
    
    static IabHelper.OnIabPurchaseFinishedListener PurchasedListener = new IabHelper.OnIabPurchaseFinishedListener() 
    {
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) 
        {
            if(result.isFailure()) 
            {
                ShowDialog("Error", "Purchasing error : " + result);
                return;
            }
            
            if(isconsumable(purchase.getSku()))
            {
            	billhelper.consumeAsync(purchase, ConsumeFinished);
            }
            else
            {
	            final String pdname = purchase.getSku();
	    		if(view != null)
	    		{
	    			view.queueEvent(new Runnable()
	    			{
	    				public void run()
	    				{
	    					purchased(pdname);
	    				}
	    			});
	    		}
            }
        }
    };
    
    private static PackageInfo getPackageInfo() 
	{
        PackageInfo pi = null;
        try 
        {
             pi = m_avtivity.getPackageManager().getPackageInfo(m_avtivity.getPackageName(), PackageManager.GET_ACTIVITIES);
        } 
        catch (PackageManager.NameNotFoundException e) 
        {
            e.printStackTrace();
        }
        return pi; 
    }
    
    public static boolean CheckBillingSupport()
    {
    	return billingsupport;
    }
    
    public static void ShowDialog(final String Title, final String Message)
    {
    	m_avtivity.runOnUiThread(new Runnable()
		{
			public void run()
			{
				String title = Title;
		        String message = Message;

		        AlertDialog.Builder builder = new AlertDialog.Builder(m_avtivity);
		        builder.setTitle(title);
		        builder.setMessage(message);
		        builder.setPositiveButton("OK", new Dialog.OnClickListener() 
		        {                  
		        	public void onClick(DialogInterface dialogInterface, int i) 
		        	{
		        		dialogInterface.dismiss();
		            }
		        });
		        
		        builder.create().show();		
			}
		});    	
    }
    
    @Override
    public void onPause()
    {
    	super.onPause();
    	Log.v("activity", " >>>>>>>>>>>>>>>>>>>>>>>>>>>. On Pause");
    	view.onPause();
    	onpause();
    }
    
    @Override
    public void onResume()
    {
    	super.onResume();
    	Log.v("activity", " >>>>>>>>>>>>>>>>>>>>>>>>>>>. On Resume");
    	view.onResume();
    	onresume();
    }

	@Override
    public void onStop()
    {
		super.onStop();
		Log.v("activity", " >>>>>>>>>>>>>>>>>>>>>>>>>>>. On Stop");
    	onstop();
    }
	
	@Override
	public void onDestroy()
	{
		super.onDestroy();
		ondestroy();
		if (billhelper != null) billhelper.dispose();
		billhelper = null;
	}
	
	private static native String checkpurchased();
	private static native void purchased(String pname);
	private static native void notpurchased(String pname);
	private static native boolean isconsumable(String pname);
	private static native void consumed(String pname);
	private static native void onbillingchecked(boolean val);
	
	private static native void onstop();
	private static native void oninit(String str, String str2);
	private static native void onpause();
	private static native void onresume();
	private static native void ondestroy();	

	static
	{
		System.loadLibrary("openal");
		System.loadLibrary("GameEngine");
    }
}

class EGLSurfaceView extends GLSurfaceView
{
	public EGLRenderer renderer;
	public Context mContext;
	
	public EGLSurfaceView(Context context)
	{
		super(context);
		renderer = new EGLRenderer();
		//setEGLConfigChooser(8, 8, 8, 8, 16, 0);
		setRenderer(renderer);
		mContext = context;
	}
	
	public boolean onTouchEvent(final MotionEvent event)
	{
		if(event.getAction()==MotionEvent.ACTION_DOWN)
		{
			if(renderer!=null)
			{				
				queueEvent(new Runnable()
                {
                    public void run()
                    {
                        mevent(1, event.getX(), event.getY());
                    }
                });
			}
		}
		else if(event.getAction()==MotionEvent.ACTION_MOVE)
		{
			if(renderer!=null)
			{
				queueEvent(new Runnable()
                {
                    public void run()
                    {
                        mevent(2, event.getX(), event.getY());
                    }
                });
			}
		}
		else if(event.getAction()==MotionEvent.ACTION_UP)
		{
			if(renderer!=null)
			{
				queueEvent(new Runnable()
                {
                    public void run()
                    {
                        mevent(3, event.getX(), event.getY());
                    }
                });
			}
		}
		//Log.v("activity", " X : " + event.getX() + " Y : " + event.getY());
		return true;
	}
	
	@Override
	public boolean onKeyDown(final int keyCode, KeyEvent event)  
	{
  
	    if(renderer!=null)
		{
			queueEvent(new Runnable()
            {
                public void run()
                {
                	keydown(keyCode);
                }
            });
			return true;
		}

	    return super.onKeyDown(keyCode, event);
	}
	
	@Override
	public boolean onKeyUp(final int keyCode, KeyEvent event)  
	{
	    if(renderer!=null)
		{
			queueEvent(new Runnable()
            {
                public void run()
                {
                	keyup(keyCode);
                }
            });
			return true;
		}

	    return super.onKeyDown(keyCode, event);
	}
	
	private static native void mevent(int action, float x, float y);
	private static native boolean keydown(int key);
	private static native boolean keyup(int key);
}

class EGLRenderer implements GLSurfaceView.Renderer
{
	public GameEngineActivity app;
	
	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		Log.v("activity", " >>>>>>>>>>>>>>>>>>>>>>>>>>>. Surface create");
		ninit();
	}
	
	public void onSurfaceChanged(GL10 gl, int w, int h)
	{
		Log.v("activity", " >>>>>>>>>>>>>>>>>>>>>>>>>>>. Surface changed");
		gl.glViewport(0, 0, w, h);
		gl.glMatrixMode(GL10.GL_PROJECTION);
		gl.glLoadIdentity();
		gl.glOrthof(0, w, h, 0, 1, 100);
		gl.glMatrixMode(GL10.GL_MODELVIEW);
		nresize(w, h);
	}
	
	public void onDrawFrame(GL10 gl)
	{
		if(ndrawframe()==1)
		{
			ExitApp();
		}
	}
	
	public void ExitApp()
	{
		if(app!=null) app.finish();
	}
	
	private static native int ndrawframe();
	private static native void ninit();
	private static native void nresize(int w, int h);
}

