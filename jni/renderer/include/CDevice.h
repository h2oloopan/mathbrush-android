/*
 *  CTextDevice.h
 *  drawing
 *
 *  Created by Mirette Marzouk on 10-03-23.
 *  Copyright 2010 U Waterloo. All rights reserved.
 *
 *  This class will use the core text library to draw text to the graphics context
 */

#ifndef CDEVICE_H_
#define CDEVICE_H_

#ifdef _MAC
	#include <ApplicationServices/ApplicationServices.h>
#endif

#ifdef _WINDOWS
	#include <windows.h>
#endif

#ifdef _iPHONE
	#include <CoreText/CoreText.h>
	#include <CoreGraphics/CGContext.h>
#endif

#ifdef __ANDROID__
#include <jni.h>
#endif

#include "Types.h"

namespace SCGRenderer
{	

#if defined(_MAC) || defined(_iPHONE)

	class CDevice
	{
	private:
		CGContextRef mContext;
		CTFontRef mFont;
		CFDictionaryRef mStringAttr;
        int mMinCharWidth;
		
	public:
		CDevice(CGContextRef _context);
		~CDevice();
		void createFont(CFStringRef _name, CGFloat _size);
		SCGRECT getStringBox(CFStringRef  _str);
		SCGRECT getStringBox(wchar_t * _txt);
		void setTextColor(TEXT_COLOR _color);
		void setPenColor(PEN_COLOR _color);
		void setBackColor(BACK_COLOR _color);
		void drawText(CFStringRef _str, float _x, float _y);
		void drawText(wchar_t * _txt, float _x, float _y);
		void drawArc(float _x1, float _y1, float _x2, float _y2,SCGRECT _rect);
		void fillRect(SCGRECT _rect);
		void drawLine(float _x1, float _y1, float _x2, float _y2);
		void drawLineTo(float _x1, float _y1);
        void setContext(CGContextRef _context);
        bool updateFontSize(CGFloat _fontSize);
		
		SCGRECT getUnicodeExactBox(scg::unicode_char _ch ){return getStringBox((wchar_t *) _ch);};
	
		CGFloat getFontHeight(){return CTFontGetDescent(mFont) + CTFontGetAscent(mFont);}

	};
#endif
	
#ifdef __ANDROID__
	class CDevice
	{
	private:
		JNIEnv* env;
		jobject canvas;
	public:
		CDevice(JNIEnv* _env, jobject _canvas, jobject _paint);
		~CDevice();
		void createFont(std::string _name, float _size);
		SCGRECT getStringBox(wchar_t * _txt);
		void setTextColor(TEXT_COLOR _color);
		void setPenColor(PEN_COLOR _color);
		void setBackColor(BACK_COLOR _color);
		void drawText(CFStringRef _str, float _x, float _y);
		void drawText(wchar_t * _txt, float _x, float _y);
		void drawArc(float _x1, float _y1, float _x2, float _y2,SCGRECT _rect);
		void fillRect(SCGRECT _rect);
		void drawLine(float _x1, float _y1, float _x2, float _y2);
		void drawLineTo(float _x1, float _y1);
	};

#endif	

#ifdef _WINDOWS	
	class CDevice
	{
	private:
		HDC mHDC;
		HFONT mFont;
		HBRUSH mBrush;
		
	public:
		CDevice(){};
		~CDevice(){};
		void createFont(std::string _name, float _size);
		SCGRECT getStringBox(wchar_t * _str);
		void display(wchar_t * _txt, float _x, float _y, SCGRECT _rect );
		void setTextColor(TEXT_COLOR _color);
		void setPenColor(PEN_COLOR _color);
		void setBackColor(BACK_COLOR _color);
		void drawArc(float _x1, float _y1, float _x2, float _y2);
		void fillRect(SCGRECT _rect);
		void drawLine(float _x1, float _y1, float _x2, float _y2);
		void drawLineTo(float _x1, float _y1);
		SCGRECT getUnicodeExactBox(scg::unicode_char _ch); 
	};
#endif
}

#endif