#include "main.h"

cvar_t g_cvars[] = {
	{"weaptfc_version",		WEAPTFC_VERSION,	FCVAR_EXTDLL | FCVAR_SERVER,	0,	NULL},
	{"weaptfc_blockfov",		"0",			FCVAR_EXTDLL,			0,	NULL},
	{"weaptfc_blockfastswitch",	"0",			FCVAR_EXTDLL,			0,	NULL},

	{NULL,				NULL,			0,				0,	NULL}
};

int g_lastclass[33];
time_t g_classchange[33];
int g_lastteam[33];

CStrings g_RegClassCmds;
CStrings g_RegRespawnCmds;
CStrings g_RegTeamCmds;

int g_curweapon[33];
int g_lastweapon[33];

char* g_weaponnames[NUM_WEAPONS] =
	{
		"axe",
		"shotgun",
		"ng",
		"sniperrifle",
		"autorifle",
		"supershotgun",
		"rpg",
		"gl",
		"pl",
		"medikit",
		"superng",
		"ac",
		"flamethrower",
		"ic",
		"knife",
		"tranq",
		"spanner",
		"railgun"
	};

int g_defaultweaps[NUM_CLASSES] = {-1, 2, 3, 6, 7,10,11,12,15,17};
int g_attack2weaps[NUM_CLASSES] = {-1,-1,-1,-1,-1, 9,11,12,-1,-1};

bool g_weaponstatus[NUM_WEAPONS][NUM_CLASSES];
bool g_grenstatus[NUM_GRENADES][NUM_CLASSES];

const char* g_szBadWeap = "That weapon is restricted\nfor your class\n";
const char* g_szBadGren = "That grenade is restricted\nfor your class\n";
const char* g_szClassQuick = "You are trying to change\nclasses too fast\n";
const char* g_szSprinting = "You are currently springing\nYou may not shoot, duck, or reload.";
const char* g_szNoMsg = "\n";

//plugin attached to metamod
void plugin_main_init(void) {
	for (int i = 0; g_cvars[i].name; ++i)
		CVAR_REGISTER(&(g_cvars[i]));
	
	REG_SVR_COMMAND("restrict", restrict);
	REG_SVR_COMMAND("unrestrictall", unrestrictall);
	REG_SVR_COMMAND("reg_class_cmd", reg_class_cmd);
	REG_SVR_COMMAND("reg_respawn_cmd", reg_respawn_cmd);
	REG_SVR_COMMAND("reg_team_cmd", reg_team_cmd);

	//clear arrays
	memset(g_weaponstatus, 0, sizeof(g_weaponstatus));
	memset(g_grenstatus, 0, sizeof(g_grenstatus));
}



//look for a weapon array index based on name
int findweaponnum(const char* weaponname) {
	if (weaponname == NULL) return -1;
	if (strlen(weaponname) < 2) return -1;

	for (int i = 0; i < NUM_WEAPONS; ++i) {
		if (FStrEq(weaponname, g_weaponnames[i]))
			return i;
	}
	return -1;
}

//is this weapon in the correct bounds?
bool isvalidweapon(int weaponindex) {
	return !(weaponindex < 0 || weaponindex >= NUM_WEAPONS);
}

//is this class in the correct bounds?
bool isvalidclass(int classnum) {
	return !(classnum < 1 || classnum >= NUM_CLASSES);
}

//is this grenade in the correct bounds?
bool isvalidgren(int grenindex) {
	return !(grenindex < 0 || grenindex >= NUM_GRENADES);
}

//checks the weapon banned status
bool isweaponbanned(int weaponindex, int classnum) {
	if (!isvalidweapon(weaponindex) || !isvalidclass(classnum)) return 0;

	return g_weaponstatus[weaponindex][classnum];
}

//checks the grenade banned status
bool isgrenbanned(int grenindex, int classnum) {
	if (!isvalidgren(grenindex)) return 0;

	return g_grenstatus[grenindex][classnum];
}

//set all weapons for given class to the given state
void setweapon_all(int weaponindex, bool state) {
	if (!isvalidweapon(weaponindex)) return;

	for (int classnum = 1; classnum < NUM_CLASSES; ++classnum)
		setweapon_oneclass(weaponindex, classnum, state);
}

//set a single weapon/class state
void setweapon_oneclass(int weaponindex, int classnum, bool state) {
	if (!isvalidweapon(weaponindex) || !isvalidclass(classnum)) return;

	g_weaponstatus[weaponindex][classnum] = state;
}

//set all grenades for given class to the given state
void setgren_all(int grenindex, bool state) {
	if (!isvalidgren(grenindex)) return;

	for (int classnum = 1; classnum < NUM_CLASSES; ++classnum)
		setgren_oneclass(grenindex, classnum, state);
}

//set a single grenade/class state
void setgren_oneclass(int grenindex, int classnum, bool state) {
	if (!isvalidgren(grenindex) || !isvalidclass(classnum)) return;
	
	g_grenstatus[grenindex][classnum] = state;
}



//called when a player respawns (from ENGINE_API_MESSAGE_BEGIN())
void HandleRespawn(edict_t* pEntity) {
	int pIndex = ENTINDEX(pEntity);
	if (pIndex <= 0 || pIndex > 32) RETURN_META(MRES_IGNORED);

	int pClass = pEntity->v.playerclass;
	if (!isvalidclass(pClass)) RETURN_META(MRES_IGNORED);

	//assign current weapon (since no command is used)
	//and clear last weapon
	g_lastweapon[pIndex] = -1;
	g_curweapon[pIndex] = g_defaultweaps[pClass];

	HandleRegCmds(g_RegRespawnCmds, "%i", pIndex);
}



void restrict(void) {
	if (CMD_ARGC() == 2 && FStrEq("help",CMD_ARGV(1))) {
		META_CONS("Command Usage: %s <type> <id> <class> <0 | 1>", CMD_ARGV(0));
		META_CONS(" ");
		META_CONS("For more information, use: %s help <type | id | class>", CMD_ARGV(0));
		return;
	}
	if (CMD_ARGC() == 2 && FStrEq("list",CMD_ARGV(1))) {
		META_CONS("Restriction status:");
		META_CONS("Grenades:");
		int i;
		for (i = 0; i < NUM_GRENADES; ++i)
			META_CONS("%i - %i%i%i%i%i%i%i%i%i", i, g_grenstatus[i][1], g_grenstatus[i][2], g_grenstatus[i][3], g_grenstatus[i][4], g_grenstatus[i][5], g_grenstatus[i][6], g_grenstatus[i][7], g_grenstatus[i][8], g_grenstatus[i][9]);
		META_CONS(" ");
		META_CONS("Weapons:");
		for (i = 0; i < NUM_WEAPONS; ++i)
			META_CONS("%i - %i%i%i%i%i%i%i%i%i", i, g_weaponstatus[i][1], g_weaponstatus[i][2], g_weaponstatus[i][3], g_weaponstatus[i][4], g_weaponstatus[i][5], g_weaponstatus[i][6], g_weaponstatus[i][7], g_weaponstatus[i][8], g_weaponstatus[i][9]);

		return;
	}
	if (CMD_ARGC() == 3 && FStrEq("help",CMD_ARGV(1))) {
		if (FStrEq("type",CMD_ARGV(2))) {
			META_CONS("Type can be either 0 for weapons or 1 for grenades");
			META_CONS(" ");
			META_CONS("For more information, use: %s help id", CMD_ARGV(0));
		} else if (FStrEq("id",CMD_ARGV(2))) {
			META_CONS("Possible grenade ids:");
			META_CONS("    0 - primary             1 - secondary");
			META_CONS(" ");
			META_CONS("Possible weapon ids:");
			META_CONS("    0 - crowbar             9 - medikit");
			META_CONS("    1 - shotgun            10 - super nailgun");
			META_CONS("    2 - nailgun            11 - assault cannon");
			META_CONS("    3 - sniperrifle        12 - flamethrower");
			META_CONS("    4 - autorifle          13 - incendiary rocket launcher");
			META_CONS("    5 - super shotgun      14 - knife");
			META_CONS("    6 - rocket launcher    15 - tranquilizer");
			META_CONS("    7 - grenade launcher   16 - wrench");
			META_CONS("    8 - pipebomb launcher  17 - railgun");
		} else if (FStrEq("class",CMD_ARGV(2))) {
			META_CONS("Possible classes:");
			META_CONS("    0 - all                 5 - medic");
			META_CONS("    1 - scout               6 - hwguy");
			META_CONS("    2 - sniper              7 - pyro");
			META_CONS("    3 - soldier             8 - spy");
			META_CONS("    4 - demoman             9 - engineer");
		} else {
			META_CONS("Invalid help category: %s",CMD_ARGV(2));
			META_CONS("Valid help categories are:");
			META_CONS("    type");
			META_CONS("    id");
			META_CONS("    class");
		}
		return;
	}

	if (CMD_ARGC() != 5) {
		META_CONS("Invalid number of arguments, try: %s help", CMD_ARGV(0));
		return;
	}
	
	bool type = ( atoi(CMD_ARGV(1)) ? 1 : 0 );
	int id = atoi(CMD_ARGV(2));
	int classnum = atoi(CMD_ARGV(3));
	bool ban = ( atoi(CMD_ARGV(4)) ? 1 : 0 );

	//if restricting weapons
	if (!type) {
		if (!classnum)
			setweapon_all(id, ban);
		else
			setweapon_oneclass(id, classnum, ban);
	//if restricting grenades
	} else {
		if (!classnum)
			setgren_all(id, ban);
		else
			setgren_oneclass(id, classnum, ban);
	}
}

void unrestrictall(void) {
	if (CMD_ARGC() > 2 || (CMD_ARGC() == 2 && FStrEq("help",CMD_ARGV(1)))) {
		META_CONS("Command Usage: %s [class=0]", CMD_ARGV(0));
		META_CONS(" ");
		META_CONS("For more information, use: restrict help class");
		return;
	}
	int classnum = ( CMD_ARGC() == 2 ? atoi(CMD_ARGV(1)) : 0 );
	
	for (int i = 0; i < NUM_WEAPONS; ++i) {
		if (!classnum) {
			setweapon_all(i, 0);
			setgren_all(GRENADE_PRIMARY, 0);
			setgren_all(GRENADE_SECONDARY, 0);
		} else {
			setweapon_oneclass(i, classnum, 0);
			setgren_oneclass(GRENADE_PRIMARY, classnum, 0);
			setgren_oneclass(GRENADE_SECONDARY, classnum, 0);
		}
	}
}

//adds new registered strings to the list
void reg_class_cmd(void) {
	if (CMD_ARGC() > 1) g_RegClassCmds.AddString(CMD_ARGS());
}

void reg_respawn_cmd(void) {
	if (CMD_ARGC() > 1) g_RegRespawnCmds.AddString(CMD_ARGS());
}

void reg_team_cmd(void) {
	if (CMD_ARGC() > 1) g_RegTeamCmds.AddString(CMD_ARGS());
}

//run all registered commands in the given cstrlist with the given format string and params
void HandleRegCmds(CStrings& cstrlist, const char* szParams, ...) {
	va_list argptr;
	static char string[1024];

	//go through each string in the given list
	unsigned int i = 0;
	while (i < cstrlist.GetSize()) {
		const char* szReg = cstrlist[i];
		
		if (szReg) {
			string[0] = '\0';
			va_start(argptr, szParams);
			vsnprintf(string, 1024, szParams, argptr);
			string[1023] = '\0';
			va_end(argptr);
			SERVER_COMMAND(UTIL_VarArgs("%s %s\n", szReg, string));
		}
		++i;
	}
	/*

	return string;
	*/
}
