/*
 *  CTextDevice.cpp
 *  drawing
 *
 *  Created by Mirette Marzouk on 10-03-23.
 *  Copyright 2010 U Waterloo. All rights reserved.
 *
 */

#include "CDevice.h"
#include "wrapper.h"

namespace SCGRenderer
{	
#if defined (_MAC) || defined (_iPHONE) 

    void updateFontHeight(int _fontHeight);  // implemented in fictionaries.cpp

    CDevice::CDevice(CGContextRef _context)
	{
		mContext = _context;
		mFont = nil;
		mStringAttr = nil;
        mMinCharWidth = 10;
	}
	//---------------------------------------------------------------------------
	CDevice::~CDevice()
	{
		if(mFont != nil)
			CFRelease(mFont);
		
		if (mStringAttr != nil)
			CFRelease(mStringAttr);
	}
	//---------------------------------------------------------------------------
	void CDevice::createFont(CFStringRef _name, CGFloat _size )
	{
		mFont = CTFontCreateWithName(_name, _size, NULL);
		
		// create the string attribute for the font name and ask the font to take the color from the fill color of the 
		// context.
		CFStringRef keys[] = { kCTFontAttributeName, kCTForegroundColorFromContextAttributeName };
		CFTypeRef values[] = { mFont , kCFBooleanTrue};
		mStringAttr = CFDictionaryCreate(NULL, (const void **)&keys, (const void **)&values, sizeof(keys) / sizeof(keys[0]), &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);     
        
        updateFontHeight(getFontHeight());
    }
	//---------------------------------------------------------------------------
    /*
     check if the font is already created it, check if it has the same _fontSize. If it is not created or it does not have the same size then create the font with the new size. 
     */
    bool CDevice::updateFontSize(CGFloat _fontSize)
    {        
        bool fontChanged = false;
        if (mFont == nil)
        {
            createFont(CFSTR("times"), _fontSize);
            fontChanged = true;
        }
        else if (CTFontGetSize(mFont) != _fontSize)
        {
            CFRelease(mFont);
            createFont(CFSTR("times"), _fontSize);
            fontChanged = true;
        }
        
        updateFontHeight(getFontHeight());
        
        return fontChanged;
    }
	//---------------------------------------------------------------------------
	// calculate the glyphs bounds and returns it in the SCGRECT_TYPE structure	
	SCGRECT CDevice::getStringBox(CFStringRef _str )
	{		
		// Create an attributed string with the current font and string.
		CFAttributedStringRef attrString = CFAttributedStringCreate(NULL, _str, mStringAttr);		
		
		CGContextSetTextPosition(mContext, 0.0, 0.0);
	
		// Create the line
		CTLineRef line = CTLineCreateWithAttributedString(attrString);
		
		//get the line dimensions
		CGRect glyphBounds = CTLineGetImageBounds(line, mContext); // the height here is the exact character height
		
		//clean up				
		CFRelease(line);
		CFRelease(attrString);
		
        // adjust the width of the rectangle if too small
        if (glyphBounds.size.width != 0 && glyphBounds.size.width < mMinCharWidth)
            glyphBounds.size.width = mMinCharWidth;
        
		// return in our structure
		SCGRECT myRect = SCGRECT(0,0,glyphBounds.size.width,getFontHeight());
		return myRect;
	}
	//---------------------------------------------------------------------------
	SCGRECT CDevice::getStringBox(wchar_t * _txt)
	{
		CFStringRef str = CFStringCreateWithCharacters(kCFAllocatorDefault, (const UniChar *) _txt, wcslen(_txt) * 2);
		
		SCGRECT rect = getStringBox(str);
		
		CFRelease(str);
		
		return rect;
	}
	//================================================================================
	// DRAWING
	//--------------------------------------------------------------------------------
	// disaply the _str in the rectangle that its top left corner is (_x,_y)
	void CDevice::drawText(CFStringRef _str ,float _x, float _y)
	{
		// Create an attributed string
		CFAttributedStringRef attrString = CFAttributedStringCreate(NULL, _str, mStringAttr);		

		// Draw the string
		CTLineRef line = CTLineCreateWithAttributedString(attrString);
		
		// the y of the text origin in mac is the baseline
		CGContextSetTextPosition(mContext, _x, _y + CTFontGetAscent(mFont));
		CTLineDraw(line, mContext);
		
		// release local resources
		CFRelease(line);
		CFRelease(attrString);
	}
	//--------------------------------------------------------------------------------
	void CDevice::drawText(wchar_t * _txt, float _x, float _y)
	{
		CFStringRef str = CFStringCreateWithCharacters(kCFAllocatorDefault, (const UniChar *) _txt, wcslen(_txt) * 2);
		
		drawText(str, _x, _y);
		
		CFRelease(str);
	}
	//---------------------------------------------------------------------------	
	// Draw a part of a circle arc between the (x1,y1) and (x2,y2) points. 
	void CDevice::drawArc(float _x1, float _y1, float _x2, float _y2, SCGRECT _rect)
	{
		CGContextMoveToPoint(mContext, _x1, _y1);
		CGContextAddArcToPoint(mContext, _x2, _y1, _x2,_y2,_x2 >_x1?_x2-_x1:_x1-_x2);
		CGContextStrokePath(mContext);
	}
	//--------------------------------------------------------------------------------
	void CDevice::fillRect(SCGRECT _rect)
	{
		CGContextFillRect(mContext, CGRectMake(_rect.left, _rect.top, _rect.width(), _rect.height()));
	}
	//--------------------------------------------------------------------------------
	void CDevice::drawLine(float _x1, float _y1, float _x2, float _y2)
	{
		CGContextMoveToPoint(mContext, _x1, _y1);
		CGContextAddLineToPoint(mContext, _x2, _y2);
		CGContextStrokePath(mContext);
	}
	//--------------------------------------------------------------------------------
	void CDevice::drawLineTo(float _x1, float _y1)
	{
		CGContextAddLineToPoint(mContext, _x1, _y1);
		CGContextStrokePath(mContext);
	}
	//================================================================================
	// SET color
	//--------------------------------------------------------------------------------
	void CDevice::setTextColor(TEXT_COLOR _color)
	{		
		if (_color == SELECTEDTEXTCOLOR)
		{
			CGContextSetRGBFillColor(mContext, MYSELECTEDTEXTCOLOR_R/255.0, MYSELECTEDTEXTCOLOR_G/255.0, MYSELECTEDTEXTCOLOR_B/255.0, 1.0);
		}	
		else
		{
			CGContextSetRGBFillColor(mContext, MYTEXTCOLOR_R/255.0,MYTEXTCOLOR_G/255.0,MYTEXTCOLOR_B/255.0, 1.0);
		}
	}
	//-----------------------------------------------------------------------------------
	void CDevice::setPenColor(PEN_COLOR _color)
	{
		if (_color == SELECTEDPENCOLOR)
		{
			CGContextSetRGBStrokeColor(mContext, MYSELECTEDTEXTCOLOR_R/255.0, MYSELECTEDTEXTCOLOR_G/255.0, MYSELECTEDTEXTCOLOR_B/255.0, 1.0);
		}	
		else
		{
			CGContextSetRGBStrokeColor(mContext, MYTEXTCOLOR_R/255.0,MYTEXTCOLOR_G/255.0,MYTEXTCOLOR_B/255.0, 1.0);
		}		
	}
	//--------------------------------------------------------------------------------
	void CDevice::setBackColor(BACK_COLOR _color)
	{		
		if (_color == HIGHLIGHTBACKCOLOR)
			CGContextSetRGBFillColor(mContext,MYBACKHILIGHT_R/255.0, MYBACKHILIGHT_G/255.0, MYBACKHILIGHT_B/255.0, 1.0);
		else 
			CGContextSetRGBFillColor(mContext,MYBACKCOLOR_R/255.0, MYBACKCOLOR_G/255.0, MYBACKCOLOR_B/255.0,1.0);
	}
	//--------------------------------------------------------------------------------
    void CDevice::setContext(CGContextRef _context)
    {
        mContext = _context;
    }
                             
#endif


//#############################################################################################################
#ifdef __ANDROID__
    CDevice::CDevice(JNIEnv* _env, jobject _device)
    {
    	env = _env;
    	device = _device;
    }
    CDevice::~CDevice()
    {
    }
    
    void CDevice::createFont(std::string _name, float _size)
    {
    	jstring name = env->NewStringUTF(_name.c_str());
    	jfloat size = (jfloat)_size;

    	jclass ADevice = env->GetObjectClass(device);
    	jmethodID jmi = env->GetMethodID(ADevice, "createFont", "(Ljava/lang/String;F)V");
    	env->CallVoidMethod(device, jmi, name, size);
    }

    SCGRECT CDevice::getStringBox(wchar_t * _txt)
    {
    	std::wstring wStr = std::wstring(_txt);
    	std::string str(wStr.begin(), wStr.end());
    	LOG("INSIDE: %s", _txt);
    	LOG("OUTSIDE: %s", str.c_str());
    	jstring jstr = env->NewStringUTF(str.c_str());
    	jclass ADevice = env->GetObjectClass(device);
    	jmethodID jmi = env->GetMethodID(ADevice, "getStringBox", "(Ljava/lang/String;)[I");
    	jintArray result = (jintArray)(env->CallObjectMethod(device, jmi, jstr));

    	jint* elements = env->GetIntArrayElements(result, 0);
    	int width = (int)(elements[0]);
    	int height = (int)(elements[1]);
    	env->ReleaseIntArrayElements(result, elements, 0);
    	return SCGRECT(0, 0, width, height);
    }

    void CDevice::setTextColor(TEXT_COLOR _color)
    {
    	int A = 255;
    	int R, G, B;
    	if (_color == SELECTEDTEXTCOLOR)
		{
			R = MYSELECTEDTEXTCOLOR_R;
			G = MYSELECTEDTEXTCOLOR_G;
			B = MYSELECTEDTEXTCOLOR_B;
		}	
		else
		{
			R = MYTEXTCOLOR_R;
			G = MYTEXTCOLOR_G;
			B = MYTEXTCOLOR_B;
		}	

		jclass ADevice = env->GetObjectClass(device);
		jmethodID jmi = env->GetMethodID(ADevice, "setTextColor", "(IIII)V");
		env->CallVoidMethod(device, jmi, (jint)A, (jint)R, (jint)G, (jint)B);
    }

    void CDevice::setPenColor(PEN_COLOR _color)
    {
    	int A = 255;
		int R, G, B;
    	if (_color == SELECTEDTEXTCOLOR)
		{
			R = MYSELECTEDTEXTCOLOR_R;
			G = MYSELECTEDTEXTCOLOR_G;
			B = MYSELECTEDTEXTCOLOR_B;
		}	
		else
		{
			R = MYTEXTCOLOR_R;
			G = MYTEXTCOLOR_G;
			B = MYTEXTCOLOR_B;
		}	

		jclass ADevice = env->GetObjectClass(device);
		jmethodID jmi = env->GetMethodID(ADevice, "setPenColor", "(IIII)V");
		env->CallVoidMethod(device, jmi, (jint)A, (jint)R, (jint)G, (jint)B);
    }

    void CDevice::setBackColor(BACK_COLOR _color)
    {
    	int A = 255;
    	int R, G, B;
    	if (_color == HIGHLIGHTBACKCOLOR)
    	{
    		R = MYBACKHILIGHT_R;
    		G = MYBACKHILIGHT_G;
    		B = MYBACKHILIGHT_B;
    	}
		else
		{
			R = MYBACKCOLOR_R;
			G = MYBACKCOLOR_G;
			B = MYBACKCOLOR_B;
		} 
		jclass ADevice = env->GetObjectClass(device);
		jmethodID jmi = env->GetMethodID(ADevice, "setBackColor", "(IIII)V");
		env->CallVoidMethod(device, jmi, (jint)A, (jint)R, (jint)G, (jint)B);
    }

    void CDevice::drawText(wchar_t* _txt, float _x, float _y)
    {
    	std::string str = android::wstr2str(_txt);
    	jstring jstr = env->NewStringUTF(str.c_str());
    	jfloat x = (jfloat)x;
    	jfloat y = (jfloat)y;

    	jclass ADevice = env->GetObjectClass(device);
    	jmethodID jmi = env->GetMethodID(ADevice, "drawText", "(Ljava/lang/String;FF)V");
    	env->CallVoidMethod(device, jmi, jstr, x, y);
    }

    void CDevice::drawArc(float _x1, float _y1, float _x2, float _y2, SCGRECT _rect)
    {
    	jfloat x1 = (jfloat)_x1;
    	jfloat y1 = (jfloat)_y1;
    	jfloat x2 = (jfloat)_x2;
    	jfloat y2 = (jfloat)_y2;
    	jfloat left = (jfloat)(_rect.left);
    	jfloat top = (jfloat)(_rect.top);
    	jfloat right = (jfloat)(_rect.right);
    	jfloat bottom = (jfloat)(_rect.bottom);

    	jclass ADevice = env->GetObjectClass(device);
    	jmethodID jmi = env->GetMethodID(ADevice, "drawArc", "(FFFFFFFF)V");
    	env->CallVoidMethod(device, jmi, x1, y1, x2, y2, left, top, right, bottom);
    }

    void CDevice::fillRect(SCGRECT _rect)
    {
    	jfloat left = (jfloat)(_rect.left);
    	jfloat top = (jfloat)(_rect.top);
    	jfloat right = (jfloat)(_rect.right);
    	jfloat bottom = (jfloat)(_rect.bottom);

    	jclass ADevice = env->GetObjectClass(device);
    	jmethodID jmi = env->GetMethodID(ADevice, "fillRect", "(FFFF)V");
    	env->CallVoidMethod(device, jmi, left, top, right, bottom);
    }

    void CDevice::drawLine(float _x1, float _y1, float _x2, float _y2)
    {
    	jfloat x1 = (jfloat)_x1;
    	jfloat y1 = (jfloat)_y1;
    	jfloat x2 = (jfloat)_x2;
    	jfloat y2 = (jfloat)_y2;

    	jclass ADevice = env->GetObjectClass(device);
    	jmethodID jmi = env->GetMethodID(ADevice, "drawLine", "(FFFF)V");
    	env->CallVoidMethod(device, jmi, x1, y1, x2, y2);
    }

    void CDevice::drawLineTo(float _x1, float _y1)
    {
    	jfloat x1 = (jfloat)_x1;
    	jfloat y1 = (jfloat)_y1;

    	jclass ADevice = env->GetObjectClass(device);
    	jmethodID jmi = env->GetMethodID(ADevice, "drawLineTo", "(FF)V");
    	env->CallVoidMethod(device, jmi, x1, y1);
    }


#endif

//#############################################################################################################
#ifdef _WINDOWS
	CDevice::~CDevice()
	{
		if(mFont != NULL)
			DeleteObject(mFont);
			
		if (mBrush)
			DeleteObject(mBrush);
	}
	//---------------------------------------------------------------------------
	void CDevice::createFont(std::string _name, float _size )
	{
		assert(_name != NULL);
		renderingFont = CreateFont(
			_size,                  // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_name.c_str());       // lpszFacename

		SelectObject(mHDC, mFont);
	}
	//---------------------------------------------------------------------------
	// calculate the glyphs bounds and returns it in the SCGRECT_TYPE structure	
	SCGRECT CDevice::getStringBox(wchar_t * _str )
	{
		SIZE txtSize;
		GetTextExtentPoint32W(_device, txt, wcslen(txt), &txtSize);
		
		// return in our structure
		SCGRECT mySCGRECT = SCGRECT(0,0,txtSize.x, txtSize.y);		
		return mySCGRECT;
	}
	//---------------------------------------------------------------------------
	// display _str in the previously calculated image bound strored in _rect at the _loc 
	void CDevice::drawText(wchar_t * _txt, float _x, float _y, SCGRECT _rect)
	{
		TextOutW(mHDC,_x,_y,_txt,wcslen(_txt));
	}
	//================================================================================
	// DRAWING
	//--------------------------------------------------------------------------------
	// Draw a part of a circle arc between the (x1,y1) and (x2,y2) points. in the SCGRECTangle _rect containing the ellipse
	void CDevice::drawArc(float _x1, float _y1, float _x2, float _y2, SCGRECT _rect)
	{
		Arc(mHDC,_rect.left, _rect.top, _rect.right, _rect.bottom, _x1, _y1, _x2, _y3);
	}
	//--------------------------------------------------------------------------------
	void CDevice::fillRect(SCGRECT _rect)
	{
		RECT rect(_rect.left,_rect.top, _rect.right, _rect.bottom);
		FillRect(mHDC,&rect,mBrush);
	}
	//--------------------------------------------------------------------------------
	void CDevice::drawLine(float _x1, float _y1, float _x2, float _y2)
	{
		MoveToEx(mHDC,_x1,_y1,NULL);
		LineTo(mHDC,_x2,_y2);	
	}
	//--------------------------------------------------------------------------------
	void CDevice::drawLineTo(float _x1, float _y1)
	{
		LineTo(mHDC,_x1,_y1);
	}
	//--------------------------------------------------------------------------------
	SCGRECT CDevice::getUnicodeExactBox(scg::unicode_char _ch)
	{
		// we need the exact character boxes
		GLYPHMETRICS glyph;
		MAT2 m2 = {{0, 1}, {0, 0}, {0, 0}, {0, 1}}; // identity matrix
		DWORD dwReturnValue;

		dwReturnValue = GetGlyphOutline(_device,crtChar, GGO_BITMAP, &glyph, 0, NULL, &m2);

		int t = mybox->bottom - FontAscent - glyph.gmptGlyphOrigin.y;  // get the real top of the character in the box
		int b = t + glyph.gmBlackBoxY;
		int l = mybox->left + i * step + glyph.gmptGlyphOrigin.x;
		int r = l + glyph.gmBlackBoxX;
		
		return SCGRECT(l,t,r,b);
	}
	//---------------------------------------------------------------------------------
	void CDevice::setTextColor(TEXT_COLOR _color)
	{
		COLORREF textColor;
		
		if (_color == SELECTEDTEXTCOLOR)
		{
			textColor = RGB(MYSELECTEDTEXTCOLOR_R,MYSELECTEDTEXTCOLOR_G,MYSELECTEDTEXTCOLOR_B);
		}	
		else
		{
			textColor = RGB(MYTEXTCOLOR_R,MYTEXTCOLOR_G,MYTEXTCOLOR_B);
		}
		SetTextColor (mHDC, textColor);
	}
	//-----------------------------------------------------------------------------------
	void CDevice::setPenColor(PEN_COLOR _color)
	{
		COLORREF penColor;
		if (_color = SELECTEDPENCOLOR)
		{
			penColor = RGB(MYSELECTEDTEXTCOLOR_R,MYSELECTEDTEXTCOLOR_G,MYSELECTEDTEXTCOLOR_B);
		}	
		else
		{
			penColor = RGB(MYTEXTCOLOR_R,MYTEXTCOLOR_G,MYTEXTCOLOR_B);
		}
		SetDCPenColor (mHDC, penColor);
	}
	//--------------------------------------------------------------------------------
	void CDevice::setBackColor(BACK_COLOR _color)
	{
		COLORREF backColor;
		
		if (_color == HIGHLIGHTBACKCOLOR)
			backColor = RGB(MYBACKHILIGHT_R, MYBACKHILIGHT_G, MYBACKHILIGHT_B);
		else 
			backColor = RGB(MYBACKCOLOR_R, MYBACKCOLOR_G, MYBACKCOLOR_B);

		if (mBrush)
			DeleteObject(mBrush);
			
		mBrush = CreateSolidBrush(backColor);
	}
#endif
}
