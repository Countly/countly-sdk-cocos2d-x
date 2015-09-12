package org.messaging.countly.sdk;

import org.cocos2dx.lib.Cocos2dxActivity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;

public class CountlyCocos2dxActivity extends Cocos2dxActivity {
	

	private BroadcastReceiver messageReceiver;
	
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	}

	@Override
	protected void onResume() {
		super.onResume();

		/**
		 * Register for broadcast action if you need to be notified when Countly
		 * message received
		 */
		messageReceiver = new BroadcastReceiver() {
			@Override
			public void onReceive(Context context, Intent intent) {
				Message message = intent
						.getParcelableExtra(CountlyMessaging.BROADCAST_RECEIVER_ACTION_MESSAGE);
				Log.i("CountlyActivity",
						"Got a message with data: " + message.getData());
			}
		};
		IntentFilter filter = new IntentFilter();
		filter.addAction(CountlyMessaging
				.getBroadcastAction(getApplicationContext()));
		registerReceiver(messageReceiver, filter);
	}

	@Override
	protected void onPause() {
		super.onPause();
		unregisterReceiver(messageReceiver);
	}

}
