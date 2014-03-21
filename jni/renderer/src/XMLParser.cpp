/*
 *  XMLParser.cpp
 *  staticCoacoaRenderingLib
 *
 *  Created by Mirette Marzouk on 10-04-07.
 *  Copyright 2010 U Waterloo. All rights reserved.
 *
 */

#ifdef _XXXMLSUPPORT

#include "XMLParser.h"

//static char const * gXMLInMemBuf;
//std::string gXMLStr;

namespace SCGRenderer
{
	DOMLSParser *gParser=0;
	extern std::string parseErrorStr;
	//============================================================================
	// functions to intialize and terminate the parser
	//-------------------------------------------------
	// returns true if successful
	bool initParser()
	{	
		if (gParser)
			return true;
		
		char * mathMLPath;
		mathMLPath = getenv ("MathML2");
		if (mathMLPath==NULL)
		{
			parseErrorStr = "MathML2 path is not set in the environment variables";
			return false;
		}
		
		// Initialize the XML4C system
		try
		{
			XMLPlatformUtils::Initialize();
		}
		catch (const XMLException& toCatch)
		{
			parseErrorStr = "Can not initialize parse";
			XERCES_STD_QUALIFIER cerr << "Error during initialization! :\n"
			<< StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;
			return false;
		}
		
		// Instantiate the DOM parser.
		static const XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
		DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(gLS);
		DOMLSParser       *parser = ((DOMImplementationLS*)impl)->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
		DOMConfiguration  *config = parser->getDomConfig();
		
		config->setParameter(XMLUni::fgDOMNamespaces, true);
		config->setParameter(XMLUni::fgXercesSchema, true);
		config->setParameter(XMLUni::fgXercesHandleMultipleImports, true);
		config->setParameter(XMLUni::fgXercesSchemaFullChecking, true);
		config->setParameter(XMLUni::fgDOMValidate, true);
		config->setParameter(XMLUni::fgDOMElementContentWhitespace, true);
		
		// enable datatype normalization - default is off
		config->setParameter(XMLUni::fgDOMDatatypeNormalization, true);
		
		// And create our error handler and install it
		CErrorHandler errorHandler;
		config->setParameter(XMLUni::fgDOMErrorHandler, &errorHandler);
		
		errorHandler.resetErrors();
	
		gParser->resetDocumentPool();
		return true;
	}
	//-------------------------------------------------
	void terminateParser()
	{
		if (gParser)
		{
			//
			//  Delete the parser itself.  Must be done prior to calling Terminate, below.
			//
			gParser->release();
			
			// And call the termination method
			XMLPlatformUtils::Terminate();
		}
	}
	//---------------------------------------------------
	DOMElement * getRootElement(std::string _mathML)
	{
		if (!initParser())
			return 0;
		
		
		XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc = 0;
		
		try
		{
			gParser->resetDocumentPool();
//			doc = gParser->parseURI(xmlFile);
		}
		
		catch (const XMLException& toCatch)
		{
			XERCES_STD_QUALIFIER cerr << "\nError during parsing: '"
			<< "Exception message is:  \n"
			<< StrX(toCatch.getMessage()) << "\n" << XERCES_STD_QUALIFIER endl;
			parseErrorStr = "Error During Parsing (XML)";
			return 0;
		}
		catch (const DOMException& toCatch)
		{
			const unsigned int maxChars = 2047;
			XMLCh errText[maxChars + 1];
			
			XERCES_STD_QUALIFIER cerr << "\nDOM Error during parsing: '" 
			<< "DOMException code is:  " << toCatch.code << XERCES_STD_QUALIFIER endl;
			
			if (DOMImplementation::loadDOMExceptionMsg(toCatch.code, errText, maxChars))
				XERCES_STD_QUALIFIER cerr << "Message is: " << StrX(errText) << XERCES_STD_QUALIFIER endl;
			
			parseErrorStr = "Error During Parsing (DOM)";
			return 0;
		}
		catch (...)
		{
			XERCES_STD_QUALIFIER cerr << "\nUnexpected exception during parsing: '";
			parseErrorStr = "General error during parsing";
			return 0;
		}
		
		//
		//  Extract the DOM tree, get the list of all the elements and report the
		//  length as the count of elements.
		//
		if (doc) 
		{
			return doc->getDocumentElement();
		}
		else 
		{
			return 0;
		}

	}		
	//============================================================================
	// ---------------------------------------------------------------------------
	//  Our Error handler that Overrides the DOM ErrorHandler interface
	// ---------------------------------------------------------------------------
	CErrorHandler::CErrorHandler() : fSawErrors(false){}
	//-----------------------------------------------------
	CErrorHandler::~CErrorHandler(){}
	//-----------------------------------------------------
	bool CErrorHandler::handleError(const DOMError& domError)
	{
		fSawErrors = true;
		if (domError.getSeverity() == DOMError::DOM_SEVERITY_WARNING)
			XERCES_STD_QUALIFIER cerr << "\nWarning at file ";
		else if (domError.getSeverity() == DOMError::DOM_SEVERITY_ERROR)
			XERCES_STD_QUALIFIER cerr << "\nError at file ";
		else
			XERCES_STD_QUALIFIER cerr << "\nFatal Error at file ";
		
		XERCES_STD_QUALIFIER cerr << StrX(domError.getLocation()->getURI())
		<< ", line " << domError.getLocation()->getLineNumber()
		<< ", char " << domError.getLocation()->getColumnNumber()
		<< "\n  Message: " << StrX(domError.getMessage()) << XERCES_STD_QUALIFIER endl;
		
		return true;
	}
	//-----------------------------------------------------
	
	void CErrorHandler::resetErrors()
	{
		fSawErrors = false;
	}
}
#endif