#include "main.h"
#include "PEB.h"

void SetupHook()
{
	while ( !c_Offset.GetRendererInfo() )
		Sleep( 100 );

	g_pClient = (cl_clientfunc_t*)c_Offset.ClientFuncs();
	g_pEngine = (cl_enginefunc_t*)c_Offset.EngineFuncs();
	g_pStudio = (engine_studio_api_t*)c_Offset.StudioFuncs();

Hook:

	memcpy( &g_Client , g_pClient , sizeof( cl_clientfunc_t ) );
	memcpy( &g_Engine , g_pEngine , sizeof( cl_enginefunc_t ) );
	memcpy( &g_Studio , g_pStudio , sizeof( engine_studio_api_t ) );

	if ( !g_Client.V_CalcRefdef || !g_Engine.V_CalcShake || !g_Studio.StudioSetupSkin )
		goto Hook;

	HookFunction();
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL , DWORD fdwReason , LPVOID lpReserved )
{
	HANDLE hThread = 0;

	if ( fdwReason == DLL_PROCESS_ATTACH )
	{
		DisableThreadLibraryCalls( hinstDLL );
		GetModuleFileNameA( hinstDLL , BaseDir , sizeof( BaseDir ) );
			
		char* pos = BaseDir + strlen( BaseDir );
		while ( pos >= BaseDir && *pos != '\\' ) --pos; pos[1] = 0;

		UnlinkModuleFromPEB( hinstDLL );

		if ( RemoveHeader( hinstDLL ) )
		{
			hThread = CreateThread( 0 , 0 , (LPTHREAD_START_ROUTINE)SetupHook , 0 , 0 , 0 );

			if ( !HideThread( hThread ) )
			{
				TerminateThread( hThread , 0 );
				return FALSE;
			}
		}

	}

	return TRUE;
}