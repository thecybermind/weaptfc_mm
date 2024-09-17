#ifndef CM_SDK_UTIL_H
#define CM_SDK_UTIL_H

#define SETCLIENTMAXSPEED   (*g_engfuncs.pfnSetClientMaxspeed)

//sdk_util.cpp
void UTIL_LogPrintf( char*, ... );
edict_t* UTIL_EdictByIndex(int);
void META_CONS(char*, ...);
char* UTIL_VarArgs( char*, ... );
void ShowFXTeleport(vec_t, vec_t, vec_t, vec3_t);
bool CvarDisabled(const char*);

#endif //CM_SDK_UTIL_H
