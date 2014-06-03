package com.mathbrush.tools;

import com.mathbrush.tools.ADevice;
import com.mathbrush.tools.Debugger;

public class Renderer {
	
	public Renderer() {
		System.loadLibrary("wrappers");
	}

	public native void init(ADevice device, int fontSize);
	//public native boolean display(String mathML, int x, int y);
	public native boolean display(long exprTree, int x, int y);
}