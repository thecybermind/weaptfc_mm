
#include "main.h"

const char* msg_name = NULL;
//look for "ResetHUD" message, player respawned
void ENGINE_API_MESSAGE_BEGIN(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed) {

	msg_name = GET_USER_MSG_NAME(PLID, msg_type, NULL);

	if (msg_name == NULL)
		RETURN_META(MRES_IGNORED);

	if (FStrEq("ResetHUD", msg_name)) {
		HandleRespawn(ed);
	}

	RETURN_META(MRES_IGNORED);
}

//these aren't being set as hooks, but are here for later use
void ENGINE_API_MESSAGE_END(void) {
	msg_name = NULL;

	RETURN_META(MRES_IGNORED);
}

void ENGINE_API_WRITE_BYTE(int iValue) {

	RETURN_META(MRES_IGNORED);
}

void ENGINE_API_WRITE_CHAR(int iValue) {

	RETURN_META(MRES_IGNORED);
}

void ENGINE_API_WRITE_SHORT(int iValue) {

	RETURN_META(MRES_IGNORED);
}

void ENGINE_API_WRITE_LONG(int iValue) {

	RETURN_META(MRES_IGNORED);
}

void ENGINE_API_WRITE_ANGLE(float flValue) {

	RETURN_META(MRES_IGNORED);
}

void ENGINE_API_WRITE_COORD(float flValue) {

	RETURN_META(MRES_IGNORED);
}

void ENGINE_API_WRITE_STRING(const char *sz) {
		
	RETURN_META(MRES_IGNORED);
}

void ENGINE_API_WRITE_ENTITY(int iValue) {

	RETURN_META(MRES_IGNORED);
}

enginefuncs_t g_HookedEngineFunctionTable;
C_DLLEXPORT int GetEngineFunctions(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion) {
	if(!pengfuncsFromEngine) {
		LOG_ERROR(PLID, "GetEngineFunctions called with null pengfuncsFromEngine");
		return(FALSE);
	}
	else if(*interfaceVersion != ENGINE_INTERFACE_VERSION) {
		LOG_ERROR(PLID, "GetEngineFunctions version mismatch; requested=%d ours=%d", *interfaceVersion, ENGINE_INTERFACE_VERSION);
		// Tell metamod what version we had, so it can figure out who is 
		// out of date.
		*interfaceVersion = ENGINE_INTERFACE_VERSION;
		return(FALSE);
	}

	g_HookedEngineFunctionTable.pfnMessageBegin = ENGINE_API_MESSAGE_BEGIN;
/*	g_HookedEngineFunctionTable.pfnMessageEnd = ENGINE_API_MESSAGE_END;
	g_HookedEngineFunctionTable.pfnWriteByte = ENGINE_API_WRITE_BYTE;
	g_HookedEngineFunctionTable.pfnWriteChar = ENGINE_API_WRITE_CHAR;
	g_HookedEngineFunctionTable.pfnWriteShort = ENGINE_API_WRITE_SHORT;
	g_HookedEngineFunctionTable.pfnWriteLong = ENGINE_API_WRITE_LONG;
	g_HookedEngineFunctionTable.pfnWriteAngle = ENGINE_API_WRITE_ANGLE;
	g_HookedEngineFunctionTable.pfnWriteCoord = ENGINE_API_WRITE_COORD;
	g_HookedEngineFunctionTable.pfnWriteString = ENGINE_API_WRITE_STRING;
*/	
	memcpy(pengfuncsFromEngine, &g_HookedEngineFunctionTable, sizeof(enginefuncs_t));
	return TRUE;
}
