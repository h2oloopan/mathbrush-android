/*
 *  XMLParser.h
 *  staticCoacoaRenderingLib
 *
 *  Created by Mirette Marzouk on 10-04-07.
 *  Copyright 2010 U Waterloo. All rights reserved.
 *
 */

#ifdef _XMLSUPPORT

#ifndef XMLParser_H_
#define XMLParser_H_


#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/AbstractDOMParser.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMLSParser.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMError.hpp>
#include <xercesc/dom/DOMLocator.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMElement.hpp>

#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#else
#include <iostream.h>
#endif
	
	
XERCES_CPP_NAMESPACE_USE

namespace SCGRenderer
{
	
	DOMElement *getRootElement(std::string _mathML);
	
	// ---------------------------------------------------------------------------
	//  Simple error handler deriviative to install on parser
	// ---------------------------------------------------------------------------
	class CErrorHandler : public DOMErrorHandler
	{
	public:
		// -----------------------------------------------------------------------
		//  Constructors and Destructor
		// -----------------------------------------------------------------------
		CErrorHandler();
		~CErrorHandler();
		
		
		// -----------------------------------------------------------------------
		//  Getter methods
		// -----------------------------------------------------------------------
		bool getSawErrors() const;
		
		
		// -----------------------------------------------------------------------
		//  Implementation of the DOM ErrorHandler interface
		// -----------------------------------------------------------------------
		bool handleError(const DOMError& domError);
		void resetErrors();
		
		
		private :
		// -----------------------------------------------------------------------
		//  Unimplemented constructors and operators
		// -----------------------------------------------------------------------
		CErrorHandler(const CErrorHandler&);
		void operator=(const CErrorHandler&);
		
		
		// -----------------------------------------------------------------------
		//  Private data members
		//
		//  fSawErrors
		//      This is set if we get any errors, and is queryable via a getter
		//      method. Its used by the main code to suppress output if there are
		//      errors.
		// -----------------------------------------------------------------------
		bool    fSawErrors;
	};
	
	//============================================================================
	// ---------------------------------------------------------------------------
	//  This is a simple class that lets us do easy (though not terribly efficient)
	//  trancoding of XMLCh data to local code page for display.
	// ---------------------------------------------------------------------------
	class StrX
	{
		public :
		// -----------------------------------------------------------------------
		//  Constructors and Destructor
		// -----------------------------------------------------------------------
		StrX(const XMLCh* const toTranscode)
		{
			// Call the private transcoding method
			fLocalForm = XMLString::transcode(toTranscode);
		}
		
		~StrX()
		{
			XMLString::release(&fLocalForm);
		}
		
		
		// -----------------------------------------------------------------------
		//  Getter methods
		// -----------------------------------------------------------------------
		const char* localForm() const
		{
			return fLocalForm;
		}
		
		private :
		// -----------------------------------------------------------------------
		//  Private data members
		//
		//  fLocalForm
		//      This is the local code page form of the string.
		// -----------------------------------------------------------------------
		char*   fLocalForm;
	};
	
	inline XERCES_STD_QUALIFIER ostream& operator<<(XERCES_STD_QUALIFIER ostream& target, const StrX& toDump)
	{
		target << toDump.localForm();
		return target;
	}
	
	inline bool CErrorHandler::getSawErrors() const
	{
		return fSawErrors;
	}
	
}


#endif
#endif