package com.ptolkachev.androidscanner;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

public class Scanner implements Runnable {
    // in C/C++ code the function will have name
    // Java_com_ptolkachev_AndroidScanner_Scanner_OnBarcodeReceived
    static native void OnBarcodeReceived(long pObject, String barcode);

    Activity mActivity;
    long mCallObject;

    BroadcastReceiver mReceiver;
    String mExtraData;

    public Scanner(Activity activity, long pObject) {
        mActivity = activity;
        mCallObject = pObject;
        mReceiver = null;
    }

    public void show() {
        mActivity.runOnUiThread(this);
    }

    @Override
    public void run() {
        System.loadLibrary("com_ptolkachev_AndroidScanner");
    }

    public void start(String actionName, String extraData) {
        if (mReceiver != null) {
            return;
        }

        mExtraData = extraData;

        mReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                if (intent != null && intent.hasExtra(mExtraData)) {
                    String barCode = intent.getStringExtra(mExtraData);
                    if (barCode != null && !barCode.isEmpty()) {
                        OnBarcodeReceived(mCallObject, barCode);
                    }
                }
            }
        };

        IntentFilter filter = new IntentFilter(actionName);
        mActivity.registerReceiver(mReceiver, filter);
    }

    public void stop() {
        if (mReceiver != null) {
            mActivity.unregisterReceiver(mReceiver);
            mReceiver = null;
        }
    }
}