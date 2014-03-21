# This make file makes a library that renders given an expression tree. No support for MathML.
# Hence the code that uses it does not have to import the xerces library.
# For the MathML feature to be included the library should contain the BuildTreeFromMathML.cpp also
# the constant _XMLSUPPORT should be defined in the compilation and the XMLCOMPILEROPTIONS should be added

# use make TEST to make the program that uses the rendering library and run it it automatically makes MACTARGET
# use make to create a static libraries for all architectures and copy them it to ../lib
# each architecture can be built separately too
#
# Change -D_MAC to -D_WINDOWS when compiling on a tablet and to -D_iPHONE for iphone
# ------------------------------------------------------------------------------------------------------
# general compilation options for debug add -g
OPTIONS=-c

# the device root and sdks
DEVROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/
DEVSDKROOT=$(DEVROOT)SDKs/iPhoneOS6.1.sdk
DEVCOMPILER=$(DEVROOT)usr/bin/llvm-gcc-4.2 $(OPTIONS)
DEVAR=$(DEVROOT)usr/bin/ar
DEVRANLIB=$(DEVROOT)usr/bin/ranlib

# the simulator root and sdk and other tools
SIMROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/
SIMSDKROOT=$(SIMROOT)SDKs/iPhoneSimulator6.1.sdk
SIMCOMPILER=$(SIMROOT)usr/bin/llvm-gcc-4.2 $(OPTIONS)
SIMAR=$(SIMROOT)usr/bin/ar
SIMRANLIB=$(SIMROOT)usr/bin/ranlib

# the MAC compilation
MACCOMPILER=g++ $(OPTIONS)

# the library directories
ROOTDIR=$(CURDIR)
INCLUDEDIR=$(ROOTDIR)/include
LIBDIR=$(ROOTDIR)/lib
SOURCEDIR=$(ROOTDIR)/src

# Dependencies directories
RECINCLUDEDIR=$(CURDIR)/../mathReco/include

# The libxml2 compiler options
XMLCOMPILEROPTIONS = xml2-config --cflags

# the source files
SOURCEFILES=CDevice.cpp BuildTreeFromExpr.cpp dictionaries.cpp Functs.cpp Lines.cpp Symbol.cpp SymbolBox.cpp BuildTreeFromMathML.cpp

# first dependency is the default
TARGETS: SIMTARGET ARM6TARGET ARM7TARGET MACTARGET
	lipo $(LIBDIR)/rendering_i386.a $(LIBDIR)/rendering_armv6.a $(LIBDIR)/rendering_armv7.a -create -output $(LIBDIR)/rendering.a

SIMTARGET:
	cd $(SOURCEDIR); $(SIMCOMPILER) -I$(INCLUDEDIR) -I$(RECINCLUDEDIR) `$(XMLCOMPILEROPTIONS)` -D_XMLSUPPORT -isysroot $(SIMSDKROOT) -mmacosx-version-min=10.6 -D__IPHONE_OS_VERSION_MIN_REQUIRED=30000 -D_iPHONE -arch i386 $(SOURCEFILES)
	$(SIMAR) rc rendering_i386.a $(SOURCEDIR)/*.o
	$(SIMRANLIB) rendering_i386.a
	cp rendering_i386.a $(LIBDIR) 
	make clean
	
ARM6TARGET:
	cd $(SOURCEDIR);$(DEVCOMPILER) -I$(INCLUDEDIR) -I$(RECINCLUDEDIR) `$(XMLCOMPILEROPTIONS)` -D_XMLSUPPORT -isysroot $(DEVSDKROOT) -miphoneos-version-min=4.3 -D_iPHONE -arch armv6 -c $(SOURCEFILES)
	$(DEVAR) rc rendering_armv6.a $(SOURCEDIR)/*.o
	$(DEVRANLIB) rendering_armv6.a
	cp rendering_armv6.a $(LIBDIR)
	make clean

ARM7TARGET:	
	cd $(SOURCEDIR);$(DEVCOMPILER) -I$(INCLUDEDIR) -I$(RECINCLUDEDIR) `$(XMLCOMPILEROPTIONS)` -D_XMLSUPPORT -isysroot $(DEVSDKROOT) -miphoneos-version-min=4.3 -D_iPHONE -arch armv7 -c $(SOURCEFILES)
	$(DEVAR) rc rendering_armv7.a $(SOURCEDIR)/*.o
	$(DEVRANLIB) rendering_armv7.a
	cp rendering_armv7.a $(LIBDIR)
	make clean
	
MACTARGET: 
	cd $(SOURCEDIR); $(MACCOMPILER) -I$(INCLUDEDIR) -I$(RECINCLUDEDIR) `$(XMLCOMPILEROPTIONS)` -D_XMLSUPPORT -D_MAC -arch i386 -c $(SOURCEFILES)
	ar -r libRendering.a $(SOURCEDIR)/*.o
	cp libRendering.a $(LIBDIR) 
	make clean

clean:
	rm -f $(SOURCEDIR)/*.o
	rm -f *.a
	
distclean:
	make clean
	rm -f $(LIBDIR)/*.*
	
##############################################################################
# to test the library generated from the make file on MAC use make TEST
# linker flags for generating the test program
# when the XML is supported we need to add the lbxml2 library reference
LINKERFLAGS=`xml2-config --cflags --libs` -L/$(CURDIR)/../mathReco/lib  -L$(LIBDIR) -F/System/Library/Frameworks
LIBRARIES = -lmathreco -lRendering -framework CoreFoundation -framework ApplicationServices
TEST: MACTARGET
	cd $(SOURCEDIR); g++ main.cpp -I$(INCLUDEDIR) -I$(RECINCLUDEDIR) -D_MAC -arch i386  $(LINKERFLAGS) $(LIBRARIES)
	cd $(SOURCEDIR); ./a.out

