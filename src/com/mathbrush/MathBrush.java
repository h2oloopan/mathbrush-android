package com.mathbrush;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;

import java.io.*;

import com.mathbrush.tools.*;
import com.mathbrush.views.*;
import com.mathbrush.wrappers.*;


public class MathBrush extends Activity {
	
	private Recognizer recognizer;
	
	public MathBrush() {
		this.recognizer = new Recognizer();
	}
	
	//Helper functions to copy assets files to internal storage
	private void copyFile(InputStream in, OutputStream out) throws IOException {
		byte[] buffer = new byte[1024];
		int read;
		while((read = in.read(buffer)) != -1) {
			out.write(buffer, 0, read);
		}
	}

	private void copyAssets() {
		File folder = this.getDir("support", 0);
		AssetManager manager = this.getAssets();
		String[] files = null;
		try {
			files = manager.list("mathbrush");
		} catch (IOException e) {
			Log.e("pan", "Failed to get assets list", e);
		}
		for (String file : files) {
			InputStream in = null;
			OutputStream out = null;
			try {
				in = manager.open("mathbrush/" + file);
				File outFile = new File(folder, file);
				out = new FileOutputStream(outFile);
				copyFile(in, out);
				in.close();
				out.flush();
				out.close();
			} catch (IOException e) {
				Log.e("pan", "Failed to copy asset file: " + file, e);
			}
		}
	}

	//Load view
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//Create main CanvasView for drawing
		setContentView(R.layout.main);


		//Copy assets to storage
		copyAssets();

		//Call to initialize recognizer
		//Create folder if not exist
		File dir = this.getDir("support", 0);
		String trainingPath = dir.getAbsolutePath();
		String profilePath = dir.getAbsolutePath();
		String profileName = "";
		String verboseFile = "";
		Debugger.log(profilePath);
		this.recognizer.init(trainingPath, profilePath, profileName, verboseFile);
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
				//When recognize button is clicked
				return true;
			default:
				return super.onOptionsItemSelected(item);
		}
	}
}