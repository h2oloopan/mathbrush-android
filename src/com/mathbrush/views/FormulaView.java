package com.mathbrush.views;

import android.content.Context;
import android.webkit.WebView;
import android.util.AttributeSet;
import android.graphics.*;

public class FormulaView extends WebView {


	public FormulaView(Context c, AttributeSet attrs) {
		super(c, attrs);
		//initialization
		this.getSettings().setJavaScriptEnabled(true);
		this.loadDataWithBaseURL("file:///android_asset/MathJax/", "<script type='text/javascript' src='MathJax.js?config=Tex-AMS-MML_SVG'></script>"
							  	+ "<span id='math'></span>","text/html","utf-8","");
	}

	public void update(String mathML) {
		this.loadUrl("javascript:document.getElementById('math').innerHTML='\\\\[" + mathML + "\\\\]';");
		this.loadUrl("javascript:MathJax.Hub.Queue(['Typeset',MathJax.Hub]);");
	}

	public void clean() {
		//display nothing on this view
		//this.setText("");
		this.loadUrl("javascript:document.getElementById('math').innerHTML='';");
		this.loadUrl("javascript:MathJax.Hub.Queue(['Typeset',MathJax.Hub]);");
	}
}
