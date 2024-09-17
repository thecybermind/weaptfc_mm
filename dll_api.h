#ifndef CM_DLL_API_H
#define CM_DLL_API_H

void DLL_API_ServerActivate(edict_t*, int, int);
void DLL_API_PlayerPreThink(edict_t*);
BOOL DLL_API_ClientConnect(edict_t*, const char*, const char*, char[128]);
void DLL_API_ClientCommand(edict_t*);

#endif //CM_DLL_API_H
