#pragma once

class AutoOffset
{
private:
	DWORD HwBase , HwSize , HwEnd;
	DWORD ClBase , ClSize , ClEnd;
	DWORD UiBase , UiSize , UiEnd;
	DWORD HlBase , HlSize , HlEnd;

	BOOL __comparemem( const UCHAR *buff1 , const UCHAR *buff2 , UINT size );
	ULONG __findmemoryclone( const ULONG start , const ULONG end , const ULONG clone , UINT size );
	ULONG __findreference( const ULONG start , const ULONG end , const ULONG address );

	DWORD FarProc( const DWORD Address , DWORD LB , DWORD HB );

public:
	BYTE HLType;

	bool GetRendererInfo();

	unsigned Absolute( DWORD Addr );
	void Error( const PCHAR Msg );
	DWORD GetModuleSize( const DWORD Address );

	PVOID ClientFuncs();
	PVOID EngineFuncs();
	PVOID StudioFuncs();

	PUserMsg FindUserMsgBase();
};

extern AutoOffset c_Offset;