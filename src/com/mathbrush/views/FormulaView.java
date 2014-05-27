package com.mathbrush.views;

import android.content.Context;
import android.util.AttributeSet;
import android.graphics.*;
import android.view.*;

import com.mathbrush.tools.ADevice;

public class FormulaView extends View {

	private ADevice device;
	private Renderer renderer;

	public FormulaView(Context c, AttributeSet attrs) {
		super(c, attrs);
		//initialization
		renderer = new Renderer();
	}

	@Override
	protected void onDraw(Canvas canvas) {
		if (device == null) {
			Paint paint = new Paint();
			paint.setAntiAlias(true);
			paint.setDither(true);
			paint.setStyle(Paint.Style.STROKE);
        	paint.setStrokeJoin(Paint.Join.ROUND);
        	paint.setStrokeCap(Paint.Cap.ROUND);
        	paint.setStrokeWidth(12);
			device = new ADevice(this, canvas, paint);
			renderer.init(device, 60);
		}			
	}

	public void display(String mathML) {
		renderer.display(mathML, 50, 50);
	}
}
