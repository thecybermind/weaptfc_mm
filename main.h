#ifndef CM_MAIN_H
#define CM_MAIN_H

#include <extdll.h>
#include <time.h>
#include <dllapi.h>
#include <enginecallback.h>
#include <meta_api.h>
#include <mutil.h>

#include "dll_api.h"
#include "engine_api.h"
#include "sdk_util.h"
#include "CStrings.h"
#include "version.h"

#define MAX_LOGMSG_LEN		1024

#define NUM_WEAPONS		18
#define NUM_CLASSES		10	//9+1
#define NUM_GRENADES		2

#define GRENADE_PRIMARY		0
#define GRENADE_SECONDARY	1

#define TFC_CLASS_NONE		0
#define TFC_CLASS_SCOUT		1
#define TFC_CLASS_SNIPER	2
#define TFC_CLASS_SOLDIER	3
#define TFC_CLASS_DEMOMAN	4
#define TFC_CLASS_MEDIC		5
#define TFC_CLASS_HWGUY		6
#define TFC_CLASS_PYRO		7
#define TFC_CLASS_SPY		8
#define TFC_CLASS_ENGINEER	9

//list of cvars, registered in plugin_main_init();
extern cvar_t g_cvars[];

//tracking class and team changes
extern int g_lastclass[33];
extern time_t g_classchange[33];
extern int g_lastteam[33];

//store registered commands
extern CStrings g_RegClassCmds;
extern CStrings g_RegRespawnCmds;
extern CStrings g_RegTeamCmds;

//track weapons
extern int g_curweapon[33];
extern int g_lastweapon[33];

//misc
extern char* g_weaponnames[NUM_WEAPONS];
extern int g_defaultweaps[NUM_CLASSES];
extern int g_attack2weaps[NUM_CLASSES];

extern bool g_weaponstatus[NUM_WEAPONS][NUM_CLASSES];
extern bool g_grenstatus[NUM_GRENADES][NUM_CLASSES];

extern const char* g_szBadWeap;
extern const char* g_szBadGren;
extern const char* g_szClassQuick;
extern const char* g_szSprinting;
extern const char* g_szNoMsg;

//main plugin "entry point"
void plugin_main_init(void);

//helper functions
int findweaponnum(const char*);
bool isvalidweapon(int);
bool isvalidclass(int);
bool isvalidgren(int);
bool isweaponbanned(int, int);
bool isgrenbanned(int, int);
void setweapon_all(int, bool);
void setweapon_oneclass(int, int, bool);
void setgren_all(int, bool);
void setgren_oneclass(int, int, bool);

//called when a player respawns
void HandleRespawn(edict_t*);

//server command handlers
void restrict(void);
void unrestrictall(void);

void reg_class_cmd(void);
void reg_respawn_cmd(void);
void reg_team_cmd(void);

//run all registered commands in the given cstrlist with the given format string and params
void HandleRegCmds(CStrings&, const char*, ...);

#endif //CM_MAIN_H
