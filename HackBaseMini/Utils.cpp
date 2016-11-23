#include "main.h"

PUserMsg UserMsgByName( char* szMsgName )
{
	PUserMsg Ptr = nullptr;
	Ptr = pUserMsgBase;
	while ( Ptr->next )
	{
		if ( !strcmp( Ptr->name , szMsgName ) )
			return Ptr;
		Ptr = Ptr->next;
	}
	Ptr->pfn = 0;
	return Ptr;
}

pfnUserMsgHook HookUserMsg( char *szMsgName , pfnUserMsgHook pfn )
{
	PUserMsg Ptr = nullptr;
	pfnUserMsgHook Original = nullptr;
	Ptr = UserMsgByName( szMsgName );
	if ( Ptr->pfn != 0 ) {
		Original = Ptr->pfn;
		Ptr->pfn = pfn;
		return Original;
	}
	else
	{
		char Message[256];
		strcpy_s( Message , ERR_1 );
		strcat_s( Message , szMsgName );
		strcat_s( Message , ERR_2 );
		MessageBoxA( 0 , Message , ERR_3 , MB_OK | MB_ICONERROR );
	}
	return Original;
}