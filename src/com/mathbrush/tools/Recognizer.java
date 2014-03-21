package com.mathbrush.tools;

import com.mathbrush.wrappers.RecognitionWrapper;

public class Recognizer {
	
	private RecognitionWrapper wrapper;
	
	public Recognizer() {
		this.wrapper = new RecognitionWrapper();
	}
	
	public void recognize() {
		//Test the wrapper here for now!
		wrapper.initRecognizer("", "", "");
	}
}
