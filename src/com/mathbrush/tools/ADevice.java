package com.mathbrush.tools;

import android.graphics.*;
import android.view.*;

import com.mathbrush.tools.Debugger;

public class ADevice {

	private View view;
	private Canvas canvas;
	private Paint paint;

	public ADevice(View view, Canvas canvas, Paint paint) {
		this.view = view;
		this.canvas = canvas;
		this.paint = paint;
	}
}