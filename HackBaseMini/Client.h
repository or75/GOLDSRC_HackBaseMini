#pragma once

#include "Main.h"

void HookUserMessages();

void HUD_Frame( double time );
void HUD_Redraw( float time , int intermission );

int HUD_Key_Event( int down , int keynum , const char *pszCurrentBinding );
void CL_CreateMove( float frametime , usercmd_s *cmd , int active );
int HUD_AddEntity( int type , struct cl_entity_s *ent , const char *modelname );

void HookFunction();