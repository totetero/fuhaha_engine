package com.totetero.fuhaha;

import android.os.Bundle;
import android.app.Fragment;
import android.view.View;
import android.view.ViewGroup;
import android.view.LayoutInflater;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// openGL描画クラス
public class FuhahaGLFragment extends Fragment{
	private FuhahaGLView glView;

	// ----------------------------------------------------------------

	public FuhahaGLFragment(){
		super();
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){
		this.glView = new FuhahaGLView(this.getActivity());
		return this.glView;
	}

	@Override
	public void onPause(){
		this.glView.onPause();
		super.onPause();
	}

	@Override
	public void onResume(){
		super.onResume();
		this.glView.onResume();
	}

	@Override
	public void onDestroyView(){
		this.glView.onDestroy();
		this.glView = null;
		super.onDestroy();
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

