#include "main.h"

void UTIL_LogPrintf( char *fmt, ... )
{
	va_list			argptr;
	static char		string[1024];
	
	va_start ( argptr, fmt );
	vsnprintf ( string, sizeof(string), fmt, argptr );
	va_end   ( argptr );

	// Print to server console
	ALERT( at_logged, "%s", string );
}

//Get an edict based on playerindex
edict_t* UTIL_EdictByIndex(int playerIndex) {
	edict_t* pPlayerEdict = NULL;
  
	if ( playerIndex > 0 && playerIndex <= gpGlobals->maxClients ) {
		pPlayerEdict = INDEXENT( playerIndex );
		if ( !pPlayerEdict || pPlayerEdict->free ) {
			pPlayerEdict = NULL;
		}
	}
	return pPlayerEdict;
}

// Print to console.
//used from log_meta.cpp
void META_CONS(char *fmt, ...) {
	va_list ap;
	char buf[MAX_LOGMSG_LEN];
	unsigned int len;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	len=strlen(buf);
	if(len < sizeof(buf)-2)		// -1 null, -1 for newline
		strcat(buf, "\n");
	else
		buf[len-1] = '\n';

	SERVER_PRINT(buf);
}

//Format a string
char* UTIL_VarArgs( char *format, ... )
{
	va_list		argptr;
	static char		string[1024];

	va_start (argptr, format);
	vsprintf(string, format, argptr);
	va_end (argptr);

	return string;
}

void ShowFXTeleport(vec_t x, vec_t y, vec_t z, vec3_t origin) {
	MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, origin);
	WRITE_BYTE(TE_TELEPORT);
	WRITE_COORD(x);
	WRITE_COORD(y);
	WRITE_COORD(z);
	MESSAGE_END();
}

bool CvarDisabled(const char* cvar) {
	const char* str = CVAR_GET_STRING(cvar);
	return (FStrEq(str, "") || FStrEq(str, "0"));
}