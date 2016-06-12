package com.totetero.fuhaha;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.view.KeyEvent;
import android.widget.LinearLayout;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// アクティビティ プログラムはここから
public class FuhahaActivity extends Activity{
	private FuhahaGLView glView;

	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);

		LinearLayout layout = new LinearLayout(this);
		layout.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
		layout.setOrientation(LinearLayout.VERTICAL);
		this.setContentView(layout);

		// ゲーム本体
		this.glView = new FuhahaGLView(this);
		this.glView.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT, 1.0f));
		layout.addView(this.glView);
	}

	protected void onPause(){
		this.glView.onPause();
		super.onPause();
	}

	protected void onResume(){
		this.glView.onResume();
		super.onResume();
	}

	protected void onDestroy(){
		this.glView.onDestroy();
		super.onDestroy();
	}

	public boolean dispatchKeyEvent(KeyEvent event){
		return this.glView.dispatchKeyEvent(event) || super.dispatchKeyEvent(event);
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

