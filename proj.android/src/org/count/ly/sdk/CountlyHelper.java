package org.count.ly.sdk;

import java.util.UUID;

import org.cocos2dx.lib.Cocos2dxHelper;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Build;
import android.telephony.TelephonyManager;

public class CountlyHelper {
	
	static Activity me;
	public static void init(Context context) {
		me = (Activity) context;
		
	}
	
	public static String getDeviceId() {
		  final TelephonyManager tm = (TelephonyManager) me.getBaseContext().getSystemService(Context.TELEPHONY_SERVICE);

		    final String tmDevice, tmSerial, androidId;
		    tmDevice = "" + tm.getDeviceId();
		    tmSerial = "" + tm.getSimSerialNumber();
		    androidId = "" + android.provider.Settings.Secure.getString(me.getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);

		    UUID deviceUuid = new UUID(androidId.hashCode(), ((long)tmDevice.hashCode() << 32) | tmSerial.hashCode());
		    String deviceId = deviceUuid.toString();
		return deviceId;
	}
	
	public static String getCarrierName() {
		TelephonyManager telephonyManager = ((TelephonyManager) me.getSystemService(Context.TELEPHONY_SERVICE));
		String operatorName = telephonyManager.getNetworkOperatorName();
		return operatorName;
	}
	
	public static String getApplicationName() {
		PackageManager pm = Cocos2dxHelper.getActivity().getPackageManager();
		ApplicationInfo aInfo;
		try {
			aInfo = pm.getApplicationInfo(
					Cocos2dxHelper.getCocos2dxPackageName(), 0);
		} catch (final NameNotFoundException e) {
			aInfo = null;
		}
		return (aInfo != null ? (String)pm.getApplicationLabel(aInfo) : null);
	}
	
	public static String getPackageVersionName() {
		PackageManager pm = Cocos2dxHelper.getActivity().getPackageManager();
		PackageInfo pInfo;
		try {
			pInfo = pm.getPackageInfo( Cocos2dxHelper.getCocos2dxPackageName(), 0);
		} catch (final NameNotFoundException e) {
			pInfo = null;
		}
		return (pInfo != null ? (String)pInfo.versionName : null );
	}
	
	public static String getPackageVersionCode() {
		PackageManager pm = Cocos2dxHelper.getActivity().getPackageManager();
		PackageInfo pInfo;
		try {
			pInfo = pm.getPackageInfo( Cocos2dxHelper.getCocos2dxPackageName(), 0);
		} catch (final NameNotFoundException e) {
			pInfo = null;
		}
		return (pInfo != null ? Integer.toString(pInfo.versionCode) : null );
	}
	
	public static String getDeviceModel(){
		return Build.MODEL;
	}
	
	public static String getDeviceSystemName() {
		return "Android";
	}
	
	public static String getDeviceSystemVersion(){
		return Build.VERSION.RELEASE;
	}
	
	public static String getUserAgent(){
		return System.getProperty("http.agent");
	}

}
