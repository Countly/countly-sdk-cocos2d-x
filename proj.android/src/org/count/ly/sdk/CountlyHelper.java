package org.count.ly.sdk;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.Locale;
import java.util.UUID;

import org.cocos2dx.lib.Cocos2dxHelper;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Build;
import android.os.Bundle;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.Log;

public class CountlyHelper {

	public static native String reportCrash(String error, String reason,
			boolean nonfatal);

	public static native String onRegistrationId(String registrationId);

	public static native String recordPushEvent(String key, String messageId);

	// static Activity me;
	public static void init(Context context) {
		// me = (Activity) context;

	}

	public static void loadNativeLibs(Context context) {
		try {
			ApplicationInfo ai = context.getPackageManager()
					.getApplicationInfo(context.getPackageName(),
							PackageManager.GET_META_DATA);
			Bundle bundle = ai.metaData;
			String libName = bundle.getString("android.app.lib_name");
			System.loadLibrary(libName);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static String getDeviceId() {
		final TelephonyManager tm = (TelephonyManager) Cocos2dxHelper
				.getActivity().getBaseContext()
				.getSystemService(Context.TELEPHONY_SERVICE);

		final String tmDevice, tmSerial, androidId;
		tmDevice = "" + tm.getDeviceId();
		tmSerial = "" + tm.getSimSerialNumber();
		androidId = ""
				+ android.provider.Settings.Secure.getString(Cocos2dxHelper
						.getActivity().getContentResolver(),
						android.provider.Settings.Secure.ANDROID_ID);

		UUID deviceUuid = new UUID(androidId.hashCode(),
				((long) tmDevice.hashCode() << 32) | tmSerial.hashCode());
		String deviceId = deviceUuid.toString();
		return deviceId + "CountlyX";
	}

	public static String getCarrierName() {
		TelephonyManager telephonyManager = ((TelephonyManager) Cocos2dxHelper
				.getActivity().getSystemService(Context.TELEPHONY_SERVICE));
		String operatorName = telephonyManager.getNetworkOperatorName();
		return operatorName;
	}

	public static synchronized void enableCrashReporting() {
		// get default handler
		final Thread.UncaughtExceptionHandler oldHandler = Thread
				.getDefaultUncaughtExceptionHandler();

		Thread.UncaughtExceptionHandler handler = new Thread.UncaughtExceptionHandler() {

			@Override
			public void uncaughtException(Thread t, Throwable e) {
				StringWriter sw = new StringWriter();
				PrintWriter pw = new PrintWriter(sw);
				e.printStackTrace(pw);
				reportCrash(sw.toString(), "Uncaught Exception", false);

				// if there was another handler before
				if (oldHandler != null) {
					// notify it also
					oldHandler.uncaughtException(t, e);
				}
			}
		};

		Thread.setDefaultUncaughtExceptionHandler(handler);

		// return this;
	}

	public static void testCrash() {
		String[] strings = { "a", "b" };
		String str = strings[strings.length];
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
		return (aInfo != null ? (String) pm.getApplicationLabel(aInfo) : null);
	}

	public static String getPackageVersionName() {
		PackageManager pm = Cocos2dxHelper.getActivity().getPackageManager();
		PackageInfo pInfo;
		try {
			pInfo = pm.getPackageInfo(Cocos2dxHelper.getCocos2dxPackageName(),
					0);
		} catch (final NameNotFoundException e) {
			pInfo = null;
		}
		return (pInfo != null ? (String) pInfo.versionName : null);
	}

	public static String getPackageVersionCode() {
		PackageManager pm = Cocos2dxHelper.getActivity().getPackageManager();
		PackageInfo pInfo;
		try {
			pInfo = pm.getPackageInfo(Cocos2dxHelper.getCocos2dxPackageName(),
					0);
		} catch (final NameNotFoundException e) {
			pInfo = null;
		}
		return (pInfo != null ? Integer.toString(pInfo.versionCode) : null);
	}

	public static String getDeviceModel() {
		return Build.MODEL;
	}

	public static String getDeviceSystemName() {
		return "Android";
	}

	public static String getDeviceSystemVersion() {
		return Build.VERSION.RELEASE;
	}

	public static String getUserAgent() {
		return System.getProperty("http.agent");
	}

	static String getLocale() {
		final Locale locale = Locale.getDefault();
		return locale.getLanguage() + "_" + locale.getCountry();
	}

	static String getAppVersion() {
		String result = "1.0";
		try {
			result = Cocos2dxHelper
					.getActivity()
					.getPackageManager()
					.getPackageInfo(
							Cocos2dxHelper.getActivity().getPackageName(), 0).versionName;
		} catch (PackageManager.NameNotFoundException e) {
			Log.i("Countly", "No app version found");
		}
		return result;
	}

	static String getStore() {
		String result = "";
		if (android.os.Build.VERSION.SDK_INT >= 3) {
			try {
				result = Cocos2dxHelper
						.getActivity()
						.getPackageManager()
						.getInstallerPackageName(
								Cocos2dxHelper.getActivity().getPackageName());
			} catch (Exception e) {
				Log.i("Countly", "Can't get Installer package");
			}
			if (result == null || result.length() == 0) {
				result = "";
				Log.i("Countly", "No store found");
			}
		}
		return result;
	}

	static String getDensity() {
		String densityStr = "";
		final int density = Cocos2dxHelper.getActivity().getResources()
				.getDisplayMetrics().densityDpi;
		switch (density) {
		case DisplayMetrics.DENSITY_LOW:
			densityStr = "LDPI";
			break;
		case DisplayMetrics.DENSITY_MEDIUM:
			densityStr = "MDPI";
			break;
		// case DisplayMetrics.DENSITY_TV:
		// densityStr = "TVDPI";
		// break;
		case DisplayMetrics.DENSITY_HIGH:
			densityStr = "HDPI";
			break;
		case DisplayMetrics.DENSITY_XHIGH:
			densityStr = "XHDPI";
			break;
		// case DisplayMetrics.DENSITY_400:
		// densityStr = "XMHDPI";
		// break;
		// case DisplayMetrics.DENSITY_XXHIGH:
		// densityStr = "XXHDPI";
		// break;
		// case DisplayMetrics.DENSITY_XXXHIGH:
		// densityStr = "XXXHDPI";
		// break;
		}
		return densityStr;
	}

}
