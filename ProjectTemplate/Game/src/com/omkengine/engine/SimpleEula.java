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

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.preference.PreferenceManager;

public class SimpleEula 
{
	private String m_appname = "";
	private String m_eulatext = "";
	private String EULA_PREFIX = "eula_";
	private Activity mActivity; 
	
	public SimpleEula(Activity context, String appname, String eulatext) 
	{
		m_appname = appname;
		m_eulatext = eulatext;
		mActivity = context; 
	}
	
	private PackageInfo getPackageInfo() 
	{
        PackageInfo pi = null;
        try 
        {
             pi = mActivity.getPackageManager().getPackageInfo(mActivity.getPackageName(), PackageManager.GET_ACTIVITIES);
        } catch (PackageManager.NameNotFoundException e) 
        {
            e.printStackTrace();
        }
        return pi; 
    }

     public void show() 
     {
        PackageInfo versionInfo = getPackageInfo();

		final String eulaKey = EULA_PREFIX + versionInfo.versionCode;
        final SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(mActivity);
        boolean hasBeenShown = prefs.getBoolean(eulaKey, false);
        if(hasBeenShown == false)
        {

            String title = m_appname + " v" + versionInfo.versionName;
            String message = m_eulatext;

            AlertDialog.Builder builder = new AlertDialog.Builder(mActivity)
                    .setTitle(title)
                    .setMessage(message)
                    .setOnCancelListener(new DialogInterface.OnCancelListener() 
                    {
                        public void onCancel(DialogInterface dialog) 
                        {
                            mActivity.finish();
                        }
                    })
                    .setPositiveButton("Accept", new Dialog.OnClickListener() 
                    {
                      
                        public void onClick(DialogInterface dialogInterface, int i) 
                        {
                            SharedPreferences.Editor editor = prefs.edit();
                            editor.putBoolean(eulaKey, true);
                            editor.commit();
                            dialogInterface.dismiss();
                        }
                    })
                    .setNegativeButton(android.R.string.cancel, new Dialog.OnClickListener() 
                    {
						
						public void onClick(DialogInterface dialog, int which) 
						{
							mActivity.finish(); 
						}
                    	
                    });
            builder.create().show();
        }
    }
	
}
