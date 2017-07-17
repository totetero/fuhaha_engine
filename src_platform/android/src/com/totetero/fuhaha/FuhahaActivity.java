package com.totetero.fuhaha;

import android.os.Bundle;
import android.app.Activity;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.widget.LinearLayout;
import android.widget.FrameLayout;
import android.view.Window;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// アクティビティ プログラムはここから
public class FuhahaActivity extends Activity{
	private static final int FRAGMENT_FUHAHA_ID = 0x00000001;

	@Override
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);

		// ルートレイアウト作成
		LinearLayout layout = new LinearLayout(this);
		layout.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
		layout.setOrientation(LinearLayout.VERTICAL);
		this.setContentView(layout);

		// フラグメント用レイアウト作成
		FrameLayout frame = new FrameLayout(this);
		frame.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT, 1.0f));
		frame.setId(FuhahaActivity.FRAGMENT_FUHAHA_ID);
		layout.addView(frame);
		// フラグメント作成
		FragmentManager fragmentManager = getFragmentManager();
		FragmentTransaction transaction = fragmentManager.beginTransaction();
		transaction.add(frame.getId(), new FuhahaGLFragment());
		transaction.commit();
	}

	@Override
	protected void onPause(){
		super.onPause();
	}

	@Override
	protected void onResume(){
		super.onResume();
	}

	@Override
	protected void onDestroy(){
		super.onDestroy();
	}

	// バックボタンのイベントを無視する
	@Override
	public void onBackPressed(){
	}

}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

