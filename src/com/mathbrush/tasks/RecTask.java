package com.mathbrush.tasks;

import java.util.ArrayList;

import android.os.AsyncTask;
import android.graphics.Point;

import com.mathbrush.*;
import com.mathbrush.tools.*;

public class RecTask extends AsyncTask<ArrayList<Point>, Void, String> {
	private MathBrush mb;

	public RecTask(MathBrush mb) {
		this.mb = mb;
	}

	@Override
	protected String doInBackground(ArrayList<Point>... params) {
		ArrayList<Point> points = params[0];
		int count = points.size();
		long[] xs = new long[count];
		long[] ys = new long[count];
		for (int i = 0; i < count; i++) {
			xs[i] = (long)points.get(i).x;
			ys[i] = (long)points.get(i).y;
		}
		mb.recognizer.addStroke(xs, ys, count);
		mb.recognizer.recognize();
		return mb.recognizer.getMathML();
	}

	@Override
	protected void onPostExecute(String mathML) {
		mb.update(mathML);
	}

/*
	@Override
	protected String doInBackground(ArrayList<Point>... params) {
		ArrayList<Point> points = params[0];
		int count = points.size();
		long[] xs = new long[count];
		long[] ys = new long[count];
		for (int i = 0; i < count; i++) {
			xs[i] = (long)points.get(i).x;
			ys[i] = (long)points.get(i).y;
		}
		mb.recognizer.addStroke(xs, ys, count);
		return mb.recognizer.getMathML();
	}

	@Override
	protected void onPostExecute(String mathML) {
		mb.update(mathML);
	}
*/
}