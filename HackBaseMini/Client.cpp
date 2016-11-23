#include "main.h"

// дл€ примера обычный bhop
cvar_s* bhop;

void HookUserMessages()
{
	pUserMsgBase = c_Offset.FindUserMsgBase();

	pResetHUD = HookUserMsg( "ResetHUD" , ResetHUD );
	pSetFOV = HookUserMsg( "SetFOV" , SetFOV );
	pTeamInfo = HookUserMsg( "TeamInfo" , TeamInfo );
	pCurWeapon = HookUserMsg( "CurWeapon" , CurWeapon );
	pDeathMsg = HookUserMsg( "DeathMsg" , DeathMsg );
}

void InitHack()
{
	g_Engine.Con_Printf( "\n\n\t%s\n\n" , CL_INFO );
	bhop = g_Engine.pfnRegisterVariable( "bhop" , "0" , 0 );
}

void HUD_Frame( double time )
{
	static bool FirstFrame = true;

	if ( FirstFrame )
	{
		g_Screen.iSize = sizeof( SCREENINFO );
		g_Engine.pfnGetScreenInfo( &g_Screen );
		g_Info.InitText();
		HookUserMessages();
		InitHack();
		FirstFrame = false;
	}

	g_Engine.pfnGetScreenInfo( &g_Screen );
	return g_Client.HUD_Frame( time );
}

void HUD_Redraw( float time , int intermission )
{
	g_Client.HUD_Redraw( time , intermission );
}

int HUD_Key_Event( int down , int keynum , const char *pszCurrentBinding )
{
	return g_Client.HUD_Key_Event( down , keynum , pszCurrentBinding );
}

// самый обычный bhop через кнопку space
void BhopFunction( usercmd_s *cmd )
{
	static bool bunnyhop = false;
	int IN_SPACE = GetAsyncKeyState( VK_SPACE );

	if ( bhop->value )
	{
		if ( cmd->buttons&IN_JUMP )
		{
			if ( bunnyhop && IN_SPACE )
			{
				cmd->buttons &= ~IN_JUMP;
				bunnyhop = false;
			}
			else
			{
				bunnyhop = true;
			}
		}
	}
}

void CL_CreateMove( float frametime , usercmd_s *cmd , int active )
{
	g_Client.CL_CreateMove( frametime , cmd , active );
	BhopFunction( cmd );
}

void SetViewAngles( float * Angles )
{
	g_Engine.Con_Printf( "\tAngles_x: %0.2f , Angles_y: %0.2f\n" , Angles[0] , Angles[1] );
	g_Engine.SetViewAngles( Angles );
}

void SetRenderModel( struct model_s *model )
{
	g_Engine.Con_Printf( "\tmodel: %s\n" , model->name );
	g_Studio.SetRenderModel( model );
}

void HookFunction()
{
	// Client Hook

	g_pClient->HUD_Frame = HUD_Frame;
	g_pClient->HUD_Redraw = HUD_Redraw;
	g_pClient->HUD_Key_Event = HUD_Key_Event;
	g_pClient->CL_CreateMove = CL_CreateMove;

	// Engine Hook

	//g_pEngine->SetViewAngles = SetViewAngles;

	// Studio hook

	//g_pStudio->SetRenderModel = SetRenderModel;
}