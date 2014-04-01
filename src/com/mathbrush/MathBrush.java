package com.mathbrush;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.content.Context;
import android.util.Log;
import android.graphics.Point;

import com.mathbrush.tools.*;
import com.mathbrush.views.*;
import com.mathbrush.tasks.*;


public class MathBrush extends Activity {
	//For access in its created views
	public Recognizer recognizer;
	
	public MathBrush() {
	}

	public void ready(Recognizer r) {
		this.recognizer = r;
		setContentView(R.layout.main);
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.loader);
		LoadTask t = new LoadTask(this);
		t.execute();
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
			CanvasView canvasView = (CanvasView)this.findViewById(R.id.canvas_view);
			FormulaView formulaView = (FormulaView)this.findViewById(R.id.formula_view);
		switch (item.getItemId()) {
			case R.id.action_recognize:
				//When recognize button is clicked
				String ml = canvasView.recognize();
				Debugger.log(ml);
				formulaView.setText(ml);
				return true;
			case R.id.action_discard:
				canvasView.clean();
				formulaView.clean(); 
			default:
				return super.onOptionsItemSelected(item);
		}
	}
}