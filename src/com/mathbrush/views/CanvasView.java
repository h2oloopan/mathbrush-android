package com.mathbrush.views;

import android.content.Context;
import android.graphics.*;
import android.view.*;
import android.util.AttributeSet;
import java.util.ArrayList;
import java.lang.Math;

import com.mathbrush.MathBrush;
import com.mathbrush.tools.*;
import com.mathbrush.tasks.*;

public class CanvasView extends View {
    //For drawing
	private Canvas  mCanvas;
    private Bitmap  mBitmap;  
    private Path    mPath;
    private Paint   mPaint;
    private ArrayList<Point> mPoints;

    private float mX, mY;
    private static final float TOUCH_TOLERANCE = 4;
    private static final int REC_SKIP = 2;
    private int skip = 0;

    public CanvasView(Context c, AttributeSet attrs) {
        super(c, attrs);

        mPath = new Path();
        mPaint = new Paint();
        mPaint.setAntiAlias(true);
        mPaint.setDither(true);
        mPaint.setColor(0xFFFF0000);
        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setStrokeJoin(Paint.Join.ROUND);
        mPaint.setStrokeCap(Paint.Cap.ROUND);
        mPaint.setStrokeWidth(12);
        mPoints = new ArrayList<Point>();
    }

    public void clean() {
        //clean up the canvas
        this.onDraw(mCanvas);
        invalidate();
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        if (mCanvas == null) {
        	mBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
            mCanvas = new Canvas(mBitmap);
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        canvas.drawColor(0xFFAAAAAA);
        canvas.drawBitmap(mBitmap, 0, 0, mPaint);
        canvas.drawPath(mPath, mPaint);
    }

    //Touch related functions
    private void touch_start(float x, float y) {
        mPath.reset();
        mPath.moveTo(x, y);
        mX = x;
        mY = y;

        //store the point
        skip = 1;
        mPoints = new ArrayList<Point>();
        mPoints.add(new Point((int)Math.floor(x), (int)Math.floor(y)));
    }
    private void touch_move(float x, float y) {
        float dx = Math.abs(x - mX);
        float dy = Math.abs(y - mY);
        if (dx >= TOUCH_TOLERANCE || dy >= TOUCH_TOLERANCE) {
            mPath.quadTo(mX, mY, (x + mX)/2, (y + mY)/2);
            mX = x;
            mY = y;

            //store the point
            if (skip >= REC_SKIP) {
                mPoints.add(new Point((int)Math.floor(x), (int)Math.floor(y)));
                skip = 0;
            }
            skip++;
        }
    }
    private void touch_up() {
        mPath.lineTo(mX, mY);
        // commit the path to our offscreen
        mCanvas.drawPath(mPath, mPaint);
        // kill this so we don't double draw
        mPath.reset();

        //Add stroke to recognizer
        RecTask task = new RecTask((MathBrush)this.getContext());
        task.execute(mPoints);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        float x = event.getX();
        float y = event.getY();

        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                touch_start(x, y);
                invalidate();
                break;
            case MotionEvent.ACTION_MOVE:
                touch_move(x, y);
                invalidate();
                break;
            case MotionEvent.ACTION_UP:
                touch_up();
                invalidate();
                break;
        }
        return true;
    }
}
