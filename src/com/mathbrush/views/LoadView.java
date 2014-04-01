package com.mathbrush.views;

import java.io.*;
import android.content.Context;
import android.util.AttributeSet;
import android.view.*;
import android.content.res.AssetManager;
import android.widget.TextView;
import android.graphics.*;

import com.mathbrush.MathBrush;
import com.mathbrush.tools.*;
import com.mathbrush.R;

public class LoadView extends TextView {
	private Recognizer recognizer;
	private MathBrush main;
	public LoadView(Context c, AttributeSet attrs) {
		super(c, attrs);
	}
}