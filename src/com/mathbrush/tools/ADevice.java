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

	public void createFont(String name, float size) {
		Debugger.log("ADevice-createFont: " + name + " " + size);
		Typeface tf = Typeface.create(name, Typeface.NORMAL);
		paint.setTypeface(tf);
		paint.setTextSize(size);
	}

	public int[] getStringBox(String txt) {
		Debugger.log("ADevice-getStringBox: " + txt);
		Rect bounds = new Rect();
		paint.getTextBounds(txt, 0, txt.length(), bounds);
		int[] result = new int[2];
		result[0] = bounds.width();
		result[1] = bounds.height();
		Debugger.log("ADevice-getStringBox-return: " + result[0] + " " + result[1]);
		return result;
	}

	public void setTextColor(int a, int r, int g, int b) {
		Debugger.log("ADevice-setTextColor: " + a + " " + r + " " + g + " " + b);
		paint.setARGB(a, r, g, b);
	}

	public void setPenColor(int a, int r, int g, int b) {
		Debugger.log("ADevice-setPenColor: " + a + " " + r + " " + g + " " + b);
		paint.setARGB(a, r, g, b);
	}

	public void setBackColor(int a, int r, int g, int b) {
		Debugger.log("ADevice-setBackColor: " + a + " " + r + " " + g + " " + b);
		canvas.drawARGB(a, r, g, b);
	}

	public void drawText(String txt, float x, float y) {
		Debugger.log("ADevice-drawText: " + txt + " " + x + " " + y);
		canvas.drawText(txt, x, y, paint);
	}

	public void drawArc(float x1, float y1, float x2, float y2, float left, float top, float right, float bottom) {
		Debugger.log("ADevice-drawArc: " + x1 + " " + y1 + " " + x2 + " " + y2 + " " + left + " " + top + " " + right + " " + bottom);
		RectF bounds = new RectF(left, top, right, bottom);
		Path path = new Path();
		path.moveTo(x1, y1);
		path.arcTo(bounds, 60, 60); //TODO: of course this is wrong
		canvas.drawPath(path, paint);
	}

	public void fillRect(float left, float top, float right, float bottom) {
		Debugger.log("ADevice-fillRect: " + left + " " + top + " " + right + " " + bottom);
		Rect bounds = new Rect((int)Math.round(left), (int)Math.round(top), (int)Math.round(right), (int)Math.round(bottom));
		canvas.drawRect(bounds, paint);
	}

	public void drawLine(float x1, float y1, float x2, float y2) {
		Debugger.log("ADevice-drawLine: " + x1 + " " + y1 + " " + x2 + " " + y2);
		Path path = new Path();
		path.moveTo(x1, y1);
		path.lineTo(x2, y2);
		canvas.drawPath(path, paint);
	}

	public void drawLineTo(float x, float y) {
		Debugger.log("ADevice-drawLineTo: " + x + " " + y);
		Path path = new Path();
		path.lineTo(x, y);
		canvas.drawPath(path, paint);
	}







}