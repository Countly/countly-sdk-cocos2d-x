package org.count.ly.sdk;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.json.JSONObject;


import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.FeatureInfo;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.media.AudioManager;
import android.net.ConnectivityManager;
import android.os.BatteryManager;
import android.os.Build;
import android.os.Environment;
import android.os.StatFs;
import android.util.Log;

public class CrashDetail {
    private static boolean inBackground = true;
    private static long totalMemory = 0;

	private static long getTotalRAM() {
        if(totalMemory == 0) {
            RandomAccessFile reader = null;
            String load = null;
            try {
                reader = new RandomAccessFile("/proc/meminfo", "r");
                load = reader.readLine();

                // Get the Number value from the string
                Pattern p = Pattern.compile("(\\d+)");
                Matcher m = p.matcher(load);
                String value = "";
                while (m.find()) {
                    value = m.group(1);
                }
                reader.close();

                totalMemory = (long) (Long.parseLong(value) / 1024.0f);
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
        return totalMemory;
    }

    /**
     * Notify when app is in foreground
     */
    static void inForeground() {
        inBackground = false;
    }

    /**
     * Notify when app is in background
     */
    static void inBackground() {
        inBackground = true;
    }

    /**
     * Returns app background state
     */
    static boolean isInBackground() {
        return inBackground;
    }


    /**
     * Returns the current device manufacturer.
     */
    static String getManufacturer() {
        return android.os.Build.MANUFACTURER;
    }

    /**
     * Returns the current device cpu.
     */
    static String getCpu() {
        if(android.os.Build.VERSION.SDK_INT < 21 )
            return android.os.Build.CPU_ABI;
        else
            return Build.SUPPORTED_ABIS[0];
    }

    /**
     * Returns the current device openGL version.
     */
    static float getOpenGL() {
        PackageManager packageManager = CountlyHelper.me.getPackageManager();
        FeatureInfo[] featureInfos = packageManager.getSystemAvailableFeatures();
        if (featureInfos != null && featureInfos.length > 0) {
            for (FeatureInfo featureInfo : featureInfos) {
                // Null feature name means this feature is the open gl es version feature.
                if (featureInfo.name == null) {
                    if (featureInfo.reqGlEsVersion != FeatureInfo.GL_ES_VERSION_UNDEFINED) {
                        return (float)((featureInfo.reqGlEsVersion & 0xffff0000) >> 16);
                    } else {
                        return 1.0f; // Lack of property means OpenGL ES version 1
                    }
                }
            }
        }
        return 1.0f;
    }

    /**
     * Returns the current device RAM amount.
     */
    static long getRamCurrent() {
        ActivityManager.MemoryInfo mi = new ActivityManager.MemoryInfo();
        ActivityManager activityManager = (ActivityManager) CountlyHelper.me.getSystemService(Context.ACTIVITY_SERVICE);
        activityManager.getMemoryInfo(mi);

        long currentRam = getTotalRAM() - (mi.availMem / 1048576L);
        return currentRam;
    }

    /**
     * Returns the total device RAM amount.
     */
    static long getRamTotal() {
        return getTotalRAM();
    }

    /**
     * Returns the current device disk space.
     */
    static long getDiskCurrent() {
        if(android.os.Build.VERSION.SDK_INT < 18 ) {
            StatFs statFs = new StatFs(Environment.getRootDirectory().getAbsolutePath());
            long   total  = (statFs.getBlockCountLong() * statFs.getBlockSizeLong());
            long   free   = (statFs.getAvailableBlocksLong() * statFs.getBlockSizeLong());
            return (total - free)/ 1048576L;
        }
        else{
            StatFs statFs = new StatFs(Environment.getRootDirectory().getAbsolutePath());
            long   total  = (statFs.getBlockCountLong() * statFs.getBlockSizeLong());
            long   free   = (statFs.getAvailableBlocksLong() * statFs.getBlockSizeLong());
            return (total - free) / 1048576L;
        }
    }

    /**
     * Returns the current device disk space.
     */
    static long getDiskTotal() {
        if(android.os.Build.VERSION.SDK_INT < 18 ) {
            StatFs statFs = new StatFs(Environment.getRootDirectory().getAbsolutePath());
            long   total  = (statFs.getBlockCountLong() * statFs.getBlockSizeLong());
            return total/ 1048576L;
        }
        else{
            StatFs statFs = new StatFs(Environment.getRootDirectory().getAbsolutePath());
            long   total  = (statFs.getBlockCountLong() * statFs.getBlockSizeLong());
            return total/ 1048576L;
        }
    }

    /**
     * Returns the current device battery level.
     */
    static int getBatteryLevel() {
        try {

            Intent batteryIntent = CountlyHelper.me.registerReceiver(null, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
            int level = batteryIntent.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
            int scale = batteryIntent.getIntExtra(BatteryManager.EXTRA_SCALE, -1);

            // Error checking that probably isn't needed but I added just in case.
            if (level > -1 && scale > 0) {
            	int batLevel = (level * 100) /  scale;
                return batLevel;
            }
        }
        catch(Exception e){
//            if (Countly.sharedInstance().isLoggingEnabled()) {
                Log.i("Countly", "Can't get battery level");
//            }
        }

        return 0;
    }

     /**
     * Returns the current device orientation.
     */
    static String getOrientation() {
        int orientation = CountlyHelper.me.getResources().getConfiguration().orientation;
        switch(orientation)
        {
            case  Configuration.ORIENTATION_LANDSCAPE:
                return "Landscape";
            case Configuration.ORIENTATION_PORTRAIT:
                return "Portrait";
            case Configuration.ORIENTATION_SQUARE:
                return "Square";
            case Configuration.ORIENTATION_UNDEFINED:
                return "Unknown";
            default:
                return null;
        }
    }

    /**
     * Checks if device is rooted.
     */
    static boolean isRooted() {
        String[] paths = { "/sbin/su", "/system/bin/su", "/system/xbin/su", "/data/local/xbin/su", "/data/local/bin/su", "/system/sd/xbin/su",
                "/system/bin/failsafe/su", "/data/local/su" };
        for (String path : paths) {
            if (new File(path).exists()) return true;
        }
        return false;
    }

    /**
     * Checks if device is online.
     */
    static long isOnline() {
        try {
            ConnectivityManager conMgr = (ConnectivityManager) CountlyHelper.me.getSystemService(Context.CONNECTIVITY_SERVICE);
            if (conMgr != null && conMgr.getActiveNetworkInfo() != null
                    && conMgr.getActiveNetworkInfo().isAvailable()
                    && conMgr.getActiveNetworkInfo().isConnected()) {

                return 1;
            }
            return 0;
        }
        catch(Exception e){
//            if (Countly.sharedInstance().isLoggingEnabled()) {
//                Log.w(Countly.TAG, "Got exception determining connectivity", e);
//            }
        }
        return 1;
    }

    /**
     * Checks if device is muted.
     */
    static boolean isMuted() {
        AudioManager audio = (AudioManager) CountlyHelper.me.getSystemService(Context.AUDIO_SERVICE);
        switch( audio.getRingerMode() ){
            case AudioManager.RINGER_MODE_SILENT:
                return true;
            case AudioManager.RINGER_MODE_VIBRATE:
                return true;
            default:
                return false;
        }
    }
}
