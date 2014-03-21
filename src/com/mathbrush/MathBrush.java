package com.mathbrush;


import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;

import com.mathbrush.tools.Recognizer;
import com.mathbrush.views.*;
import com.mathbrush.wrappers.*;

public class MathBrush extends Activity {
	
	private Recognizer recognizer;
	
	public MathBrush() {
		this.recognizer = new Recognizer();
	}
	
	//Load view
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//Create main CanvasView for drawing
		setContentView(R.layout.main);
	}
	
	//Attach button to action bar
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.actions, menu);
		return super.onCreateOptionsMenu(menu);
	}
	
	//Button click
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
			case R.id.action_recognize:
				this.recognizer.recognize();
				return true;
			default:
				return super.onOptionsItemSelected(item);
		}
	}
}