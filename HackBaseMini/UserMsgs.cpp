#include "main.h"

pfnUserMsgHook pResetHUD;
pfnUserMsgHook pSetFOV;
pfnUserMsgHook pTeamInfo;
pfnUserMsgHook pCurWeapon;
pfnUserMsgHook pDeathMsg;

int ResetHUD( const char *pszName , int iSize , void *pbuf )
{
	return pResetHUD( pszName , iSize , pbuf );
}

int SetFOV( const char *pszName , int iSize , void *pbuf )
{
	return pSetFOV( pszName , iSize , pbuf );
}

int TeamInfo( const char *pszName , int iSize , void *pbuf )
{
	return pTeamInfo( pszName , iSize , pbuf );
}

int CurWeapon( const char *pszName , int iSize , void *pbuf )
{
	return pCurWeapon( pszName , iSize , pbuf );
}

int DeathMsg( const char *pszName , int iSize , void *pbuf )
{
	return pDeathMsg( pszName , iSize , pbuf );
}