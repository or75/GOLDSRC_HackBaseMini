#pragma once

#include "Main.h"

template <int XORSTART , int BUFLEN , int XREFKILLER>
class XorStr
{
private:
	XorStr();
public:
	char s[BUFLEN];
	XorStr( const char* xs );
	~XorStr(){ for ( int i = 0; i<BUFLEN; i++ )s[i] = 0; }
};

template <int XORSTART , int BUFLEN , int XREFKILLER>
XorStr<XORSTART , BUFLEN , XREFKILLER>::XorStr( const char* xs )
{
	int xvalue = XORSTART;
	int i = 0;
	for ( ; i<( BUFLEN - 1 ); i++ ) {
		s[i] = xs[i - XREFKILLER] ^ xvalue;
		xvalue += 1;
		xvalue %= 256;
	}
	s[BUFLEN - 1] = 0;
}

pfnUserMsgHook HookUserMsg( char *szMsgName , pfnUserMsgHook pfn );

// tru 228 encryption string
// zaza
// xorgen.html

#define ERR_1 /*ERROR: Couldn't find */XorStr<0x1D,22,0xE89AFF93>("\x58\x4C\x4D\x6F\x73\x18\x03\x67\x4A\x53\x4B\x4C\x47\x0D\x5F\x0C\x4B\x47\x41\x54\x11"+0xE89AFF93).s
#define ERR_2 /* message.*/XorStr<0xA5,10,0x277DCBC3>("\x85\xCB\xC2\xDB\xDA\xCB\xCC\xC9\x83"+0x277DCBC3).s
#define ERR_3 /*Error HookUserMsg*/XorStr<0x58,18,0x8D2ECAC5>("\x1D\x2B\x28\x34\x2E\x7D\x16\x30\x0F\x0A\x37\x10\x01\x17\x2B\x14\x0F"+0x8D2ECAC5).s

#define FNT_1 /*Lucida Console*/XorStr<0x46,15,0x9B300C88>("\x0A\x32\x2B\x20\x2E\x2A\x6C\x0E\x21\x21\x23\x3E\x3E\x36"+0x9B300C88).s

#define OFF_1 /*GameUI.dll*/XorStr<0xB2,11,0x4CCF0293>("\xF5\xD2\xD9\xD0\xE3\xFE\x96\xDD\xD6\xD7"+0x4CCF0293).s
#define OFF_2 /*vgui.dll*/XorStr<0xA0,9,0xC9524A04>("\xD6\xC6\xD7\xCA\x8A\xC1\xCA\xCB"+0xC9524A04).s
#define OFF_3 /*vgui2.dll*/XorStr<0x26,10,0x92009506>("\x50\x40\x5D\x40\x18\x05\x48\x41\x42"+0x92009506).s
#define OFF_4 /*d3dim.dll*/XorStr<0x64,10,0xCB42D1DE>("\x00\x56\x02\x0E\x05\x47\x0E\x07\x00"+0xCB42D1DE).s
#define OFF_5 /*hw.dll*/XorStr<0xD6,7,0x08859227>("\xBE\xA0\xF6\xBD\xB6\xB7"+0x08859227).s
#define OFF_6 /*sw.dll*/XorStr<0x88,7,0xD6F8F31E>("\xFB\xFE\xA4\xEF\xE0\xE1"+0xD6F8F31E).s
#define OFF_7 /*Invalid module handle.*/XorStr<0xAB,23,0xF17012FC>("\xE2\xC2\xDB\xCF\xC3\xD9\xD5\x92\xDE\xDB\xD1\xC3\xDB\xDD\x99\xD2\xDA\xD2\xD9\xD2\xDA\xEE"+0xF17012FC).s
#define OFF_8 /*Invalid renderer type.*/XorStr<0xC1,23,0x5BA7C136>("\x88\xAC\xB5\xA5\xA9\xAF\xA3\xE8\xBB\xAF\xA5\xA8\xA8\xBC\xAA\xA2\xF1\xA6\xAA\xA4\xB0\xF8"+0x5BA7C136).s
#define OFF_9 /*client.dll*/XorStr<0xDE,11,0xA1A6E575>("\xBD\xB3\x89\x84\x8C\x97\xCA\x81\x8A\x8B"+0xA1A6E575).s
#define OFF_A /*Fatal Error*/XorStr<0x75,12,0x2125E45C>("\x33\x17\x03\x19\x15\x5A\x3E\x0E\x0F\x11\x0D"+0x2125E45C).s
#define OFF_B /*ScreenFade*/XorStr<0x5D,11,0xD631359B>("\x0E\x3D\x2D\x05\x04\x0C\x25\x05\x01\x03"+0xD631359B).s
#define OFF_C /*Couldn't find ClientPtr pointer.*/XorStr<0x76,33,0x1DB28261>("\x35\x18\x0D\x15\x1E\x15\x5B\x09\x5E\x19\xE9\xEF\xE6\xA3\xC7\xE9\xEF\xE2\xE6\xFD\xDA\xFF\xFE\xAD\xFE\xE0\xF9\xFF\xE6\xF6\xE6\xBB"+0x1DB28261).s
#define OFF_D /*Couldn't find EnginePtr pointer.*/XorStr<0xD0,33,0x3848CB44>("\x93\xBE\xA7\xBF\xB0\xBB\xF1\xA3\xF8\xBF\xB3\xB5\xB8\xFD\x9B\xB1\x87\x88\x8C\x86\xB4\x91\x94\xC7\x98\x86\x83\x85\x98\x88\x9C\xC1"+0x3848CB44).s
#define OFF_E /*Couldn't find StudioPtr pointer.*/XorStr<0xAF,33,0x5FDEC9C4>("\xEC\xDF\xC4\xDE\xD7\xDA\x92\xC2\x97\xDE\xD0\xD4\xDF\x9C\xEE\xCA\xCA\xA4\xA8\xAD\x93\xB0\xB7\xE6\xB7\xA7\xA0\xA4\xBF\xA9\xBF\xE0"+0x5FDEC9C4).s
#define OFF_F /*Couldn't find UserMsgBase #1 pointer.*/XorStr<0xAC,38,0x199B0F16>("\xEF\xC2\xDB\xC3\xD4\xDF\x95\xC7\x94\xD3\xDF\xD9\xDC\x99\xEF\xC8\xD9\xCF\xF3\xCC\xA7\x83\xA3\xB0\xA1\xE5\xE5\xF6\xE8\xB9\xA5\xA2\xA2\xB9\xAB\xBD\xFE"+0x199B0F16).s
#define OF_10 /*Couldn't find UserMsgBase #2 pointer.*/XorStr<0xEC,38,0x27579DD0>("\xAF\x82\x9B\x83\x94\x9F\xD5\x87\xD4\x93\x9F\x99\x9C\xD9\xAF\x88\x99\x8F\xB3\x8C\x67\x43\x63\x70\x61\x25\x25\x35\x28\x79\x65\x62\x62\x79\x6B\x7D\x3E"+0x27579DD0).s

#define CL_INFO /*CS 1.6 HACK BASE BY _OR_75*/XorStr<0xCC,27,0xF9978B87>("\x8F\x9E\xEE\xFE\xFE\xE7\xF2\x9B\x95\x96\x9D\xF7\x9A\x98\x89\x9E\xFC\x9F\x87\xFF\xBF\xAE\xB0\xBC\xD3\xD0"+0xF9978B87).s