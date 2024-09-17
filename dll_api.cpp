
#include "main.h"

//this file contains functions to hook the DLL API

//was going to use this, but not anymore
//no longer being set as a hook in GetEntityAPI2
void DLL_API_ServerActivate(edict_t *pEdictList, int edictCount, int clientMax) {
	RETURN_META(MRES_IGNORED);
}

//used to check when a player changes class or team or is sprinting
void DLL_API_PlayerPreThink(edict_t* pEntity) {
	int pIndex = ENTINDEX(pEntity);
	if (pIndex <= 0 || pIndex > 32) RETURN_META(MRES_IGNORED);

	if (g_lastclass[pIndex] != pEntity->v.playerclass) {
		HandleRegCmds(g_RegClassCmds, "%i %i %i", pIndex, g_lastclass[pIndex], pEntity->v.playerclass);
		g_lastclass[pIndex] = pEntity->v.playerclass;
	}
	if (g_lastteam[pIndex] != pEntity->v.team) {
		HandleRegCmds(g_RegTeamCmds, "%i %i %i", pIndex, g_lastteam[pIndex], pEntity->v.team);
		g_lastteam[pIndex] = pEntity->v.team;
	}

	RETURN_META(MRES_IGNORED);
}

//used to initialize the player arrays
BOOL DLL_API_ClientConnect(edict_t* pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128]) {
	int pIndex = ENTINDEX(pEntity);
	if (pIndex > 0 && pIndex <= 32) {
		g_curweapon[pIndex] = -1;
		g_lastweapon[pIndex] = -1;

		g_lastclass[pIndex] = TFC_CLASS_NONE;
		g_classchange[pIndex] = 0;
		g_lastteam[pIndex] = 0;
	}

	RETURN_META_VALUE(MRES_IGNORED, TRUE);
}

//checks for various commands
//"fov"
//class commands
//weapon commands
//grenade commands
//"lastinv"
//"+attack2" / "_special"
void DLL_API_ClientCommand(edict_t* pEntity) {
	int pIndex = ENTINDEX(pEntity);
	if (pIndex <= 0 || pIndex > 32) RETURN_META(MRES_IGNORED);

	int pClass = pEntity->v.playerclass;
	if (!isvalidclass(pClass)) RETURN_META(MRES_IGNORED);

	const char* cmd = CMD_ARGV(0);

	//fov block check
	//dont bother checking if cvar is disabled
	if ((int)CVAR_GET_FLOAT("weaptfc_blockfov") && FStrEq(cmd, "fov")) {
		//only block if a number is supplied
		//and if the number isn't 0, 89, or 90
		if (CMD_ARGC() == 2 && atoi(CMD_ARGV(1)) != 89 && atoi(CMD_ARGV(1)) != 90 && atoi(CMD_ARGV(1)) != 0)
			RETURN_META(MRES_SUPERCEDE);
	}

	//class change checking
	//dont bother checking if cvar is disabled
	else if ((int)CVAR_GET_FLOAT("weaptfc_blockfastswitch") && FStrEq(cmd, "scout") || FStrEq(cmd, "soldier") || FStrEq(cmd, "sniper") || FStrEq(cmd, "demoman") || FStrEq(cmd, "medic") || FStrEq(cmd, "hwguy") || FStrEq(cmd, "pyro") || FStrEq(cmd, "spy") || FStrEq(cmd, "engineer") || FStrEq(cmd, "random")) {
		if (!g_classchange[pIndex])
			g_classchange[pIndex] = time(NULL);
		else {
			if (time(NULL) - g_classchange[pIndex] < 2) {
				CLIENT_PRINTF(pEntity, print_center, g_szClassQuick);
				RETURN_META(MRES_SUPERCEDE);
			}
		}
	}

	//primary grenade restriction checking
	else if (FStrEq(cmd, "+gren1") || FStrEq(cmd, "primeone")) {
		if (isgrenbanned(GRENADE_PRIMARY, pClass)) {
			CLIENT_PRINTF(pEntity, print_center, g_szBadGren);
			RETURN_META(MRES_SUPERCEDE);
		}
	}

	//secondary grenade restriction checking
	else if (FStrEq(cmd, "+gren2") || FStrEq(cmd, "primetwo")) {
		if (isgrenbanned(GRENADE_SECONDARY, pClass)) {
			CLIENT_PRINTF(pEntity, print_center, g_szBadGren);
			RETURN_META(MRES_SUPERCEDE);
		}
	}
	
	//previous weapon restriction checking
	else if (FStrEq(cmd, "lastinv")) {
		if (isweaponbanned(g_lastweapon[pIndex], pClass)) {
			CLIENT_PRINTF(pEntity, print_center, g_szBadWeap);
			RETURN_META(MRES_SUPERCEDE);
		}
		if (g_lastweapon[pIndex] != -1) {
			int temp = g_curweapon[pIndex];
			g_curweapon[pIndex] = g_lastweapon[pIndex];
			g_lastweapon[pIndex] = temp;
		}
	}

	//weapon restriction checking
	else if (!strncmp(cmd, "tf_weapon_", 10)) {
		char* pcmd = (char*)cmd;	//get a copy of the command pointer for modifying
		pcmd += 10;	//get part of string after "tf_weapon_"
		
		int weaponindex = findweaponnum(pcmd);
		if (weaponindex == -1) RETURN_META(MRES_IGNORED);

		if (isweaponbanned(weaponindex, pClass)) {
			CLIENT_PRINTF(pEntity, print_center, g_szBadWeap);
			RETURN_META(MRES_SUPERCEDE);
		}

		//track weapons (last weapon is for selective "lastinv" blocking)
		//this is also done in HandleRespawn() since no command is given
		//to select weapon at respawn
		//but only do if it's not the same as the current weapon
		if (weaponindex != g_curweapon[pIndex]) {
			g_lastweapon[pIndex] = g_curweapon[pIndex];
			g_curweapon[pIndex] = weaponindex;
		}
	}

	//+attack2 weapon restriction checking
	else if (FStrEq(cmd, "_special")) {
		if (isweaponbanned(g_attack2weaps[pClass], pClass)) {
			CLIENT_PRINTF(pEntity, print_center, g_szBadWeap);
			RETURN_META(MRES_SUPERCEDE);
		}
		//store weapon change here too, but only if the weapon actually changes
		if (g_attack2weaps[pClass] != -1 && g_attack2weaps[pClass] != g_curweapon[pIndex]) {
			g_lastweapon[pIndex] = g_curweapon[pIndex];
			g_curweapon[pIndex] = g_attack2weaps[pClass];
		}
	}

	RETURN_META(MRES_IGNORED);
}

DLL_FUNCTIONS g_HookedDLLFunctionTable;
C_DLLEXPORT int GetEntityAPI2( DLL_FUNCTIONS* pFunctionTable, int* interfaceVersion ){
	if(!pFunctionTable) {
                LOG_ERROR(PLID, "GetEntityAPI2 called with null pFunctionTable");
                return(FALSE);
        }

//	g_HookedDLLFunctionTable.pfnServerActivate = DLL_API_ServerActivate;
	g_HookedDLLFunctionTable.pfnPlayerPreThink = DLL_API_PlayerPreThink;
	g_HookedDLLFunctionTable.pfnClientConnect = DLL_API_ClientConnect;
	g_HookedDLLFunctionTable.pfnClientCommand = DLL_API_ClientCommand;

	memcpy( pFunctionTable, &g_HookedDLLFunctionTable, sizeof( DLL_FUNCTIONS ) );

	return(TRUE);
}
