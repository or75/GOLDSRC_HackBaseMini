#pragma once

#include "Main.h"

#define FL_NONE	0 
#define FL_OUTLINE	2 
#define FL_BACKDROP	4 
#define FL_CENTER	8
#define FL_BOLD 16
#define FL_ALL	255 

class cFont
{
public:
	cFont( char* , int , int );
	int iStringLen( const char *fmt , ... );
	void DrawString( int x , int y , int r , int g , int b , const char *fmt , ... );
	void DrawStringCenter( int x , int y , int r , int g , int b , const char *fmt , ... );
	void Print( int x , int y , int r , int g , int b , int a , BYTE flags , char *szbuf , ... );
	void InitText();
	int   height;
	int   width;
private:
	void	Render( int x , int y , int r , int g , int b , int a , char *string );
	short	cheight;
	short	cwidth[255];
	char   name[20];
	UINT   g_FontListID;
};

extern cFont g_Info;