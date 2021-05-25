package com.ptolkachev.androidscanner;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

public class Scanner implements Runnable, ClipboardManager.OnPrimaryClipChangedListener {
    // in C/C++ code the function will have name
    // Java_com_ptolkachev_androidscanner_Scanner_OnBarcodeReceived
    static native void OnBarcodeReceived(long pObject, String barcode);

    Activity mActivity;
    long mCallObject;

    String mScanMode;
    String mExtraData;
    BroadcastReceiver mReceiver;

    public Scanner(Activity activity, long pObject) {
        mActivity = activity;
        mCallObject = pObject;
        mReceiver = null;
        mScanMode = "broadcast";
    }

    public void show()
    {
        mActivity.runOnUiThread(this);
    }

    @Override
    public void run() {
        System.loadLibrary("com_ptolkachev_AndroidScanner");
    }

    public void start(String scanMode, String actionName, String extraData) {
        if (mReceiver != null) {
            return;
        }

        mScanMode = scanMode;

        if (mScanMode.equals("clipboard")) {
            ClipboardManager clipboardManager = getClipboardManager();
            if (clipboardManager != null) {
                clipboardManager.addPrimaryClipChangedListener(this);
            }
        } else {
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
    }

    public void stop() {
        if (mReceiver != null) {
            mActivity.unregisterReceiver(mReceiver);
            mReceiver = null;
        }
        if (mScanMode.equals("clipboard")) {
            ClipboardManager clipboardManager = getClipboardManager();
            if (clipboardManager != null) {
                clipboardManager.removePrimaryClipChangedListener(this);
            }
        }
    }

    @Override
    public void onPrimaryClipChanged() {
        ClipboardManager clipboardManager = getClipboardManager();
        if (clipboardManager != null) {
            ClipData clipData = clipboardManager.getPrimaryClip();
            if (clipData != null) {
                ClipData.Item item = clipData.getItemAt(0);
                String barCode = item.getText().toString();
                if (!barCode.isEmpty()) {
                    OnBarcodeReceived(mCallObject, barCode);
                }
            }
        }
    }

    private ClipboardManager getClipboardManager() {
        return (ClipboardManager) mActivity.getSystemService(Context.CLIPBOARD_SERVICE);
    }
}