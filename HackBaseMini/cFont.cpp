#include "cFont.h"

#pragma warning(disable:4996)
#pragma warning(disable:4244)

cFont::cFont( char *i_name , int i_width , int i_height )
{
	strcpy( name , i_name );
	width = i_width;
	height = i_height;
}

void cFont::InitText()
{
	HDC hDC;
	HFONT hFont;
	HFONT hOldFont;

	hDC = wglGetCurrentDC();
	g_FontListID = glGenLists( 256 );

	hFont = CreateFontA( height , width , 0 , 0 , FW_DONTCARE , FALSE , FALSE , FALSE , ANSI_CHARSET ,
		OUT_TT_ONLY_PRECIS , CLIP_DEFAULT_PRECIS , PROOF_QUALITY ,
		DEFAULT_PITCH | FF_DONTCARE , name );

	hOldFont = (HFONT)SelectObject( hDC , hFont );

	wglUseFontBitmapsA( hDC , 0 , 255 , g_FontListID );
	wglUseFontBitmapsA( hDC , 0 , 255 , g_FontListID );

	for ( int i = 0; i < 255; i++ )
	{
		SIZE s;
		char line[2] = { (char)i , 0 };
		GetTextExtentPointA( hDC , line , 1 , &s );
		cwidth[i] = s.cx;
		cheight = s.cy;
	}

	SelectObject( hDC , hOldFont );
	DeleteObject( hFont );
}

int cFont::iStringLen( const char *fmt , ... )
{
	va_list va_alist;
	char buf[256];
	va_start( va_alist , fmt );
	_vsnprintf( buf , sizeof( buf ) , fmt , va_alist );
	va_end( va_alist );
	int iWidth , iHeight;
	g_Engine.pfnDrawConsoleStringLen( buf , &iWidth , &iHeight );
	return iWidth;
}

void cFont::DrawString( int x , int y , int r , int g , int b , const char *fmt , ... )
{
	va_list va_alist;
	char buf[256];
	va_start( va_alist , fmt );
	_vsnprintf( buf , sizeof( buf ) , fmt , va_alist );
	va_end( va_alist );
	g_Engine.pfnDrawSetTextColor( (float)r / 255.0f , (float)g / 255.0f , (float)b / 255.0f );
	g_Engine.pfnDrawConsoleString( x , y , buf );
}

void cFont::DrawStringCenter( int x , int y , int r , int g , int b , const char *fmt , ... )
{
	va_list va_alist;
	char buf[256];
	va_start( va_alist , fmt );
	_vsnprintf( buf , sizeof( buf ) , fmt , va_alist );
	va_end( va_alist );
	int iWidth = iStringLen( "%s" , buf );
	g_Engine.pfnDrawSetTextColor( (float)r / 255.0f , (float)g / 255.0f , (float)b / 255.0f );
	g_Engine.pfnDrawConsoleString( x - iWidth / 2 , y , buf );
}

void cFont::Print( int x , int y , int r , int g , int b , int a , BYTE flags , char *string , ... )
{
	char		strText[256];
	va_list		argumentPtr;

	va_start( argumentPtr , string );
	vsprintf( strText , string , argumentPtr );
	va_end( argumentPtr );

	// fix crash
	if ( c_Offset.HLType == RENDERTYPE_UNDEFINED || c_Offset.HLType == RENDERTYPE_SOFTWARE
		|| c_Offset.HLType == RENDERTYPE_D3D )
	{
		if ( flags & FL_CENTER )
			DrawStringCenter( x , y - 10 , r , g , b , strText , argumentPtr );
		else
			DrawString( x , y - 10 , r , g , b , strText , argumentPtr );
		return;
	}

	glDisable( GL_TEXTURE_2D );

	int drawlen = 0;
	for ( char *p = strText; *p; p++ ) { drawlen += cwidth[*p]; }

	if ( flags & FL_CENTER ) { x -= ( drawlen / 2 ); }
	if ( flags & FL_OUTLINE )
	{
		Render( x - 1 , y - 1 , 0 , 0 , 0 , 200 , strText );
		Render( x + 1 , y - 1 , 0 , 0 , 0 , 200 , strText );
		Render( x - 1 , y + 1 , 0 , 0 , 0 , 200 , strText );
		Render( x + 1 , y + 1 , 0 , 0 , 0 , 200 , strText );
	}
	if ( flags & FL_BACKDROP )
	{
		Render( x , y - 1 , 0 , 0 , 0 , 255 , strText );
		Render( x , y - 1 , 0 , 0 , 0 , 255 , strText );
		Render( x - 1 , y , 0 , 0 , 0 , 255 , strText );
		Render( x - 1 , y , 0 , 0 , 0 , 255 , strText );

		Render( x - 1 , y - 1 , 0 , 0 , 0 , 255 , strText );
		Render( x , y - 1 , 0 , 0 , 0 , 255 , strText );
		Render( x - 1 , y - 1 , 0 , 0 , 0 , 255 , strText );
		Render( x , y - 1 , 0 , 0 , 0 , 255 , strText );
	}
	if ( flags & FL_BOLD )
	{
		Render( x - 1 , y - 1 , r , g , b , a , strText );
		Render( x + 1 , y - 1 , r , g , b , a , strText );
		Render( x - 1 , y + 1 , r , g , b , a , strText );
		Render( x + 1 , y + 1 , r , g , b , a , strText );
	}

	Render( x , y , r , g , b , a , strText );

	glEnable( GL_TEXTURE_2D );
}

void cFont::Render( int x , int y , int r , int g , int b , int a , char *string )
{
	int i = 0;
	while ( x < 0 )
	{
		x += cwidth[string[i]];
		i++; if ( !string[i] ) { return; }
	}

	glColor4ub( r , g , b , a );
	glRasterPos2i( x , y );

	glHint( GL_PERSPECTIVE_CORRECTION_HINT , GL_NICEST );
	glPushAttrib( GL_LIST_BIT );
	glListBase( g_FontListID );
	glCallLists( strlen( string ) - i , GL_UNSIGNED_BYTE , string + i );
	glPopAttrib();
}

cFont g_Info( FNT_1 , 9 , 11 );