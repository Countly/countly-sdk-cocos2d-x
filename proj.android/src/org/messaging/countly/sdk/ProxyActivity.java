package org.messaging.countly.sdk;

import org.countly.sdk.CountlyHelper;

import android.R.integer;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

public class ProxyActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
//		CountlyHelper.loadNativeLibs(this);
	}

	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
	}

	@Override
	protected void onStart() {
		super.onStart();

		Bundle extras = getIntent().getExtras();
		final Message msg = extras
				.getParcelable(CountlyMessaging.EXTRA_MESSAGE);

		if (msg != null) {

			if (extras.containsKey(CountlyMessaging.NOTIFICATION_SHOW_DIALOG)) {


				if(CountlyMessaging.isMainActivityStart) {
					CountlyMessaging.recordMessageOpen(msg.getId());
				}
				AlertDialog.Builder builder = new AlertDialog.Builder(this);
				builder.setTitle(msg.getNotificationTitle(this)).setMessage(
						msg.getNotificationMessage());

				if (msg.hasLink()) {
					builder.setCancelable(true)
							.setPositiveButton(CountlyMessaging.buttonNames[0],
									new DialogInterface.OnClickListener() {
										@Override
										public void onClick(
												DialogInterface dialog,
												int which) {
											CountlyMessaging
													.recordMessageAction(msg
															.getId());
											finish();
											startActivity(msg
													.getIntent(
															ProxyActivity.this,
															CountlyMessaging
																	.getActivityClass()));
										}
									})
							.setOnCancelListener(
									new DialogInterface.OnCancelListener() {
										@Override
										public void onCancel(
												DialogInterface dialog) {
											finish();
										}
									});
				} else if (msg.hasReview() || msg.hasUpdate()) {
					int index = 1;
					if(msg.hasUpdate()) {
						index = 2;
					}
					builder.setCancelable(true)
							.setPositiveButton(CountlyMessaging.buttonNames[index],
									new DialogInterface.OnClickListener() {
										@Override
										public void onClick(
												DialogInterface dialog,
												int which) {
											CountlyMessaging
													.recordMessageAction(msg
															.getId());
											finish();
											startActivity(msg
													.getIntent(
															ProxyActivity.this,
															CountlyMessaging
																	.getActivityClass()));
										}
									})
							.setOnCancelListener(
									new DialogInterface.OnCancelListener() {
										@Override
										public void onCancel(
												DialogInterface dialog) {
											finish();
										}
									});
				} else if (msg.hasMessage()) {
					if(CountlyMessaging.isMainActivityStart) {
						CountlyMessaging.recordMessageAction(msg.getId());
					}
					builder.setCancelable(true);
					builder.setOnCancelListener(new DialogInterface.OnCancelListener() {
						@Override
						public void onCancel(DialogInterface dialog) {
							finish();
						}
					});
				} else {
					throw new IllegalStateException(
							"Countly Message with UNKNOWN type in ProxyActivity");
				}

				builder.create().show();
			} else {

				if(CountlyMessaging.isMainActivityStart) {
					CountlyMessaging.recordMessageAction(msg.getId());
				}
				startActivity(msg.getIntent(this,
						CountlyMessaging.getActivityClass()));
			}
		}
	}

	@Override
	protected void onStop() {
		super.onStop();
	}
}
