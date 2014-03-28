package com.mathbrush.views;

import android.content.Context;
import android.widget.TextView;
import android.util.AttributeSet;
import android.graphics.*;

public class FormulaView extends TextView {
	public FormulaView(Context c, AttributeSet attrs) {
		super(c, attrs);
	}

	public void clean() {
		//display nothing on this view
		this.setText("");
	}
}
