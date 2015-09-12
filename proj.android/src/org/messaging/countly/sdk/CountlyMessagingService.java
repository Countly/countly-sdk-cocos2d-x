package org.messaging.countly.sdk;

import android.app.ActivityManager;
import android.app.IntentService;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import com.google.android.gms.gcm.GoogleCloudMessaging;

import java.util.List;

import org.countly.sdk.CountlyHelper;

public class CountlyMessagingService extends IntentService {
	public static final String TAG = "CountlyMessagingService";

	public CountlyMessagingService() {
		super(TAG);
	}

	@Override
	protected void onHandleIntent(Intent intent) {

//		CountlyHelper.loadNativeLibs(this);

		Log.i("mCountly", "onHandleIntent");
		Bundle extras = intent.getExtras();

		GoogleCloudMessaging gcm = GoogleCloudMessaging
				.getInstance(getApplicationContext());
		String messageType = gcm.getMessageType(intent);

		if (!extras.isEmpty()) {
			if (GoogleCloudMessaging.MESSAGE_TYPE_MESSAGE.equals(messageType)) {
				final Message msg = new Message(extras);

				if (msg.isValid()) {
					// if (Countly.sharedInstance().isLoggingEnabled()) {
					// Log.i(TAG, "Got a message from Countly Messaging: " +
					// msg);
					// }

					// Send broadcast
					Intent broadcast = new Intent(
							CountlyMessaging
									.getBroadcastAction(getApplicationContext()));
					broadcast.putExtra(
							CountlyMessaging.BROADCAST_RECEIVER_ACTION_MESSAGE,
							msg);
					sendBroadcast(broadcast);

					// Init Countly in case app is not running
					// if (!Countly.sharedInstance().isInitialized()) {
					// if
					// (!CountlyMessaging.initCountly(getApplicationContext()))
					// {
					// Log.e(TAG, "Cannot init Countly in background");
					// }
					// }

					Log.e("isSilent", msg.isSilent() + " - ");
					// Show message if not silent
					if (msg.isSilent() && CountlyMessaging.isMainActivityStart) {
						CountlyMessaging.recordMessageOpen(msg.getId());
					} else {
						// Go through proxy activity to be able to record
						// message open & action performed events

						Class<?> activityClass = ProxyActivity.class;
						if(!CountlyMessaging.isMainActivityStart) {
							activityClass = CountlyMessaging.getActivityClass();
						}
						Intent proxy = new Intent(getApplicationContext(),
								activityClass);
						proxy.putExtra(CountlyMessaging.EXTRA_MESSAGE, msg);
						notify(proxy);
					}

				}
			}
		}

		CountlyMessaging.completeWakefulIntent(intent);
	}

	protected void notify(Intent proxy) {
		Message msg = proxy.getParcelableExtra(CountlyMessaging.EXTRA_MESSAGE);
		proxy.putExtra(CountlyMessaging.NOTIFICATION_SHOW_DIALOG, true);

		if (isAppInForeground(this)) {
			// Go with dialog
			proxy.putExtra(CountlyMessaging.NOTIFICATION_SHOW_DIALOG, true);
			proxy.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			startActivity(proxy);
		} else {
			// Notification case
			if(CountlyMessaging.isMainActivityStart) {
				CountlyMessaging.recordMessageOpen(msg.getId());
			}

			int notifyId = (int) System.currentTimeMillis();

			NotificationManager manager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
			PendingIntent contentIntent = PendingIntent.getActivity(
					getApplicationContext(), notifyId, proxy,
					PendingIntent.FLAG_UPDATE_CURRENT);

			// Get icon from application or use default one
			int icon;
			try {
				icon = getPackageManager().getApplicationInfo(getPackageName(),
						0).icon;
			} catch (PackageManager.NameNotFoundException e) {
				icon = android.R.drawable.ic_dialog_email;
			}

			NotificationCompat.Builder builder = new NotificationCompat.Builder(
					getApplicationContext())
					.setAutoCancel(true)
					.setSmallIcon(icon)
					.setTicker(msg.getNotificationMessage())
					.setContentTitle(
							msg.getNotificationTitle(getApplicationContext()))
					.setContentText(msg.getNotificationMessage())
					.setContentIntent(contentIntent);

			if (msg.hasSoundDefault()) {
				builder.setDefaults(Notification.DEFAULT_SOUND);
			} else if (msg.hasSoundUri()) {
				builder.setSound(Uri.parse(msg.getSoundUri()));
			}

			manager.notify(notifyId, builder.build());
		}
	}

	private static boolean isAppInForeground(Context context) {
		ActivityManager activityManager = (ActivityManager) context
				.getSystemService(Context.ACTIVITY_SERVICE);
		List<ActivityManager.RunningAppProcessInfo> appProcesses = activityManager
				.getRunningAppProcesses();
		if (appProcesses == null) {
			return false;
		}
		final String packageName = context.getPackageName();
		for (ActivityManager.RunningAppProcessInfo appProcess : appProcesses) {
			if (appProcess.importance == ActivityManager.RunningAppProcessInfo.IMPORTANCE_FOREGROUND
					&& appProcess.processName.equals(packageName)) {
				return true;
			}
		}
		return false;
	}
}