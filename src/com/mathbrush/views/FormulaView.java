package com.mathbrush.views;

import android.content.Context;
import android.util.AttributeSet;
import android.graphics.*;
import android.view.*;

import com.mathbrush.tools.Renderer;
import com.mathbrush.tools.Debugger;

public class FormulaView extends View {

	private Paint paint;
	private Canvas canvas;
	private Bitmap bitmap;
	private Path path;
	private Renderer renderer;

	private int PATH_WIDTH = 5;
	private int REGULAR_WIDTH = 1;

	public FormulaView(Context c, AttributeSet attrs) {
		super(c, attrs);

		paint = new Paint();
		paint.setColor(Color.BLACK);
		paint.setAntiAlias(true);
		paint.setDither(true);
		paint.setStyle(Paint.Style.FILL);
    	paint.setStrokeJoin(Paint.Join.ROUND);
    	paint.setStrokeCap(Paint.Cap.ROUND);
    	paint.setStrokeWidth(REGULAR_WIDTH);

    	path = new Path();

		//initialization
		renderer = new Renderer();
		renderer.init(this, 60);
	}

	@Override
	protected void onDraw(Canvas canvas) {
		canvas.drawBitmap(bitmap, 0, 0, paint);
	}

	@Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        if (canvas == null) {
        	bitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
        	canvas = new Canvas(bitmap);
        	clean();
        }
    }

    //helper
    private void printBytes(byte[] buf) {
    	Debugger.log("ADevice-printBytes: " + buf.length);
    	for (int i = 0; i < buf.length; i++) {
    		Debugger.log(String.format("%x", buf[i]));
    	}
    }


    public void clean() {
    	canvas.drawColor(0xFFFFFFFF);
    	invalidate();
    }

	/*
	public void display(String mathML) {
		Debugger.log(mathML);
		renderer.display(mathML, 50, 50);
	}
	*/

	public void display(long exprTree) {
		clean();
		renderer.display(exprTree, 50, 50);
	}

	public void createFont(String name, float size) {
		Debugger.log("ADevice-createFont: " + name + " " + size);
		Typeface tf = Typeface.create(name, Typeface.NORMAL);
		paint.setTypeface(tf);
		paint.setTextSize(size);
	}

	public float getFontHeight() {
		//Debugger.log("ADevice-getFontHeight: ");
		Paint.FontMetrics fm = paint.getFontMetrics();
		//Debugger.log("ADevice-getFontHeight-return: " + fm.ascent + " " + fm.descent);
		return fm.descent - fm.ascent;
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
		//paint.setARGB(a, r, g, b);
	}

	public void setPenColor(int a, int r, int g, int b) {
		Debugger.log("ADevice-setPenColor: " + a + " " + r + " " + g + " " + b);
		//paint.setARGB(a, r, g, b);
	}

	public void setBackColor(int a, int r, int g, int b) {
		Debugger.log("ADevice-setBackColor: " + a + " " + r + " " + g + " " + b);
		//canvas.drawARGB(a, r, g, b);
		//invalidate();
	}

	public void drawText(String txt, float x, float y) {
		Debugger.log("ADevice-drawText: " + txt + " " + x + " " + y);
		//byte[] buf = txt.getBytes();
		//printBytes(buf);
		canvas.drawText(txt, x, y, paint);
		invalidate();
	}

	public void drawArc(float x1, float y1, float x2, float y2, float left, float top, float right, float bottom) {
		Debugger.log("ADevice-drawArc: " + x1 + " " + y1 + " " + x2 + " " + y2 + " " + left + " " + top + " " + right + " " + bottom);
		RectF bounds = new RectF(left, top, right, bottom);
		path.reset();
		path.moveTo(x1, y1);
		path.arcTo(bounds, 60, 60); //TODO: of course this is wrong
		paint.setStyle(Paint.Style.STROKE);
		paint.setStrokeWidth(PATH_WIDTH);
		canvas.drawPath(path, paint);
		paint.setStyle(Paint.Style.FILL);
		paint.setStrokeWidth(REGULAR_WIDTH);
		invalidate();
	}

	public void fillRect(float left, float top, float right, float bottom) {
		Debugger.log("ADevice-fillRect: " + left + " " + top + " " + right + " " + bottom);
		Rect bounds = new Rect((int)Math.round(left), (int)Math.round(top), (int)Math.round(right), (int)Math.round(bottom));
		canvas.drawRect(bounds, paint);
		invalidate();
	}

	public void drawLine(float x1, float y1, float x2, float y2) {
		Debugger.log("ADevice-drawLine: " + x1 + " " + y1 + " " + x2 + " " + y2);
		path.reset();
		path.moveTo(x1, y1);
		path.lineTo(x2, y2);
		paint.setStyle(Paint.Style.STROKE);
		paint.setStrokeWidth(PATH_WIDTH);
		canvas.drawPath(path, paint);
		paint.setStyle(Paint.Style.FILL);
		paint.setStrokeWidth(REGULAR_WIDTH);
		invalidate();
	}

	public void drawLineTo(float x, float y) {
		Debugger.log("ADevice-drawLineTo: " + x + " " + y);
		path.reset();
		path.lineTo(x, y);
		paint.setStyle(Paint.Style.STROKE);
		paint.setStrokeWidth(PATH_WIDTH);
		canvas.drawPath(path, paint);
		paint.setStyle(Paint.Style.FILL);
		paint.setStrokeWidth(REGULAR_WIDTH);
		invalidate();
	}
}
