#include "main.h"

#define CompareMemory(Buff1, Buff2, Size) __comparemem((const UCHAR *)Buff1, (const UCHAR *)Buff2, (UINT)Size)
#define FindMemoryClone(Start, End, Clone, Size) __findmemoryclone((const ULONG)Start, (const ULONG)End, (const ULONG)Clone, (UINT)Size)
#define FindReference(Start, End, Address) __findreference((const ULONG)Start, (const ULONG)End, (const ULONG)Address)

AutoOffset c_Offset;

// from pascal ~
typedef unsigned Cardinal;
typedef unsigned* PCardinal;
typedef unsigned** PPCardinal;

bool AutoOffset::GetRendererInfo()
{
	DWORD GameUI = (DWORD)GetModuleHandleA( OFF_1 );
	DWORD vgui = (DWORD)GetModuleHandleA( OFF_2 );
	DWORD vgui2 = (DWORD)GetModuleHandleA( OFF_3 );
	DWORD d3dim = (DWORD)GetModuleHandleA( OFF_4 );

	HwBase = (DWORD)GetModuleHandleA( OFF_5 ); // Hardware

	if ( HwBase == NULL )
	{
		HwBase = (DWORD)GetModuleHandleA( OFF_6 ); // Software
		if ( HwBase == NULL )
		{
			HwBase = (DWORD)GetModuleHandleA( NULL ); // Non-Steam?
			if ( HwBase == NULL ) // Invalid module handle.
			{
				Error( OFF_7 );
			}
			else
				HLType = RENDERTYPE_UNDEFINED;
		}
		else
			HLType = RENDERTYPE_SOFTWARE;
	}
	else
	{
		if ( d3dim == NULL )
			HLType = RENDERTYPE_HARDWARE;
		else
			HLType = RENDERTYPE_D3D;
	}

	HwSize = (DWORD)GetModuleSize( HwBase );

	if ( HwSize == NULL )
	{
		switch ( HwSize )
		{
		case RENDERTYPE_HARDWARE: HwSize = 0x122A000; break;
		case RENDERTYPE_UNDEFINED: HwSize = 0x2116000; break;
		case RENDERTYPE_SOFTWARE: HwSize = 0xB53000; break;
		default:Error( OFF_8 );
		}
	}

	HwEnd = HwBase + HwSize - 1;

	ClBase = (DWORD)GetModuleHandleA( OFF_9 );

	if ( ClBase != NULL ) {
		ClSize = (DWORD)GetModuleSize( ClBase );
		ClEnd = ClBase + ClSize - 1;
	}
	else {
		ClBase = HwBase;
		ClEnd = HwEnd;
		ClSize = HwSize;
	}

	if ( GameUI != NULL )
	{
		UiBase = GameUI;
		UiSize = (DWORD)GetModuleSize( UiBase );
		UiEnd = UiBase + UiSize - 1;
	}

	HlBase = (DWORD)GetModuleHandleA( NULL );
	HlSize = (DWORD)GetModuleSize( HlBase );
	HlEnd = HlBase + HlSize - 1;

	return ( HwBase && ClBase && GameUI && vgui && vgui2 && HlBase );
}

unsigned AutoOffset::Absolute( DWORD Addr )
{
	return Cardinal( Addr ) + *(PCardinal)(Addr)+4;
}

void AutoOffset::Error( const PCHAR Msg )
{
	MessageBoxA( 0 , Msg , OFF_A , MB_OK | MB_ICONERROR );
	ExitProcess( 0 );
}

DWORD AutoOffset::GetModuleSize( const DWORD Address )
{
	return PIMAGE_NT_HEADERS( Address + (DWORD)PIMAGE_DOS_HEADER( Address )->e_lfanew )->OptionalHeader.SizeOfImage;
}

DWORD AutoOffset::FarProc( const DWORD Address , DWORD LB , DWORD HB )
{
	return ( ( Address < LB ) || ( Address > HB ) );
}

BOOL AutoOffset::__comparemem( const UCHAR *buff1 , const UCHAR *buff2 , UINT size )
{
	for ( UINT i = 0; i < size; i++ , buff1++ , buff2++ )
	{
		if ( ( *buff1 != *buff2 ) && ( *buff2 != 0xFF ) )
			return FALSE;
	}
	return TRUE;
}

ULONG AutoOffset::__findmemoryclone( const ULONG start , const ULONG end , const ULONG clone , UINT size )
{
	for ( ULONG ul = start; ( ul + size ) < end; ul++ )
	{
		if ( CompareMemory( ul , clone , size ) )
			return ul;
	}
	return NULL;
}

ULONG AutoOffset::__findreference( const ULONG start , const ULONG end , const ULONG address )
{
	UCHAR Pattern[5];
	Pattern[0] = 0x68;
	*(ULONG*)&Pattern[1] = address;
	return FindMemoryClone( start , end , Pattern , sizeof( Pattern ) - 1 );
}

PVOID AutoOffset::ClientFuncs()
{
	DWORD Address = (DWORD)FindMemoryClone( HwBase , HwEnd , OFF_B , strlen( OFF_B ) );
	PVOID ClientPtr = (PVOID)*(PDWORD)( FindReference( HwBase , HwEnd , Address ) + 0x13 ); // all patch

	if ( FarProc( (DWORD)ClientPtr , HwBase , HwEnd ) )
		Error( OFF_C );

	return ClientPtr;
}

PVOID AutoOffset::EngineFuncs()
{
	PVOID EnginePtr = nullptr;

	BYTE Offset[5] =
	{
		0x22 ,	// OLD PATCH HL/CS
		0x23 ,	// DOD STEAM
		0x1C ,	// NEW PATCH CS
		0x1D ,	// STEAM NEW PATCH
		0x37	// STEAM HL
	};

	for ( BYTE bOffset = 0; bOffset < sizeof( Offset ); bOffset++ )
	{
		EnginePtr = (cl_enginefunc_t*)*(PDWORD)( (DWORD)g_pClient->Initialize + Offset[bOffset] );

		if ( FarProc( (DWORD)EnginePtr , HwBase , HwEnd ) && FarProc( (DWORD)EnginePtr , HlBase , HlEnd ) )
		{
			if ( !FarProc( (DWORD)EnginePtr , ClBase , ClEnd ) )
				goto found_offset;
		}
	}

	Error( OFF_D );

found_offset:

	return EnginePtr;
}

PVOID AutoOffset::StudioFuncs()
{
	PVOID StudioPtr = (engine_studio_api_t*)*(DWORD*)( (DWORD)g_pClient->HUD_GetStudioModelInterface + 0x30 ); // old patch / dod

	if ( FarProc( (DWORD)StudioPtr , HwBase , HwEnd ) &&
		FarProc( (DWORD)StudioPtr , HlBase , HlEnd ) &&
		FarProc( (DWORD)StudioPtr , ClBase , ClEnd ) /*FOR DOD*/ )
	{
		StudioPtr = (engine_studio_api_t*)*(DWORD*)( (DWORD)g_pClient->HUD_GetStudioModelInterface + 0x1A ); // new patch / steam	

		if ( FarProc( (DWORD)StudioPtr , ClBase , ClEnd ) )
			Error( OFF_E );
	}

	return StudioPtr;
}

PUserMsg AutoOffset::FindUserMsgBase()
{
	BYTE Pattern_UserMsg[8] =
	{
		0x52 , 0x50 , 0xE8 , 0xFF , 0xFF , 0xFF , 0xFF , 0x83
	};

	BYTE Pattern_UserMsg2[12] =
	{
		0xFF , 0xFF , 0xFF , 0x0C ,
		0x56 , 0x8B , 0x35 , 0xFF , 0xFF , 0xFF , 0xFF , 0x57
	};

	Cardinal Address = Absolute( (Cardinal)FindMemoryClone( (DWORD)g_Engine.pfnHookUserMsg , (DWORD)g_Engine.pfnHookUserMsg + 0x32 ,
		&Pattern_UserMsg , sizeof( Pattern_UserMsg ) ) + 3 );

	if ( FarProc( Address , HwBase , HwEnd ) )
		Error( OFF_F );

	Address = (Cardinal)FindMemoryClone( (DWORD)Address , (DWORD)Address + 0x32 ,
		&Pattern_UserMsg2 , sizeof( Pattern_UserMsg2 ) ) + 7;

	if ( FarProc( Address , HwBase , HwEnd ) )
		Error( OF_10 );

	return PUserMsg( **PPCardinal( Address ) );
}