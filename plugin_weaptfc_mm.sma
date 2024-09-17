#include <core>
#include <string>
#include <admin>
#include <adminlib>
#include <plugin>

#define ACCESS_WEAPTFC ACCESS_BAN

forward admin_restrictweap(HLCmd, HLData, HLUser, UID);
forward admin_restrictgren(HLCmd, HLData, HLUser, UID);
forward admin_unrestrictall(HLCmd, HLData, HLUser, UID);
forward admin_restricthelp(HLCmd, HLData, HLUser, UID);
forward admin_blockfov(HLCmd, HLData, HLUser, UID);

public admin_restrictweap(HLCmd, HLData, HLUser, UID) {
  new Data[MAX_DATA_LENGTH];
  convert_string(HLData, Data, MAX_DATA_LENGTH);

  new Exec[MAX_DATA_LENGTH];
  snprintf(Exec, MAX_DATA_LENGTH, "restrict 0 %s", Data);
  exec(Exec);

  return PLUGIN_HANDLED;
}

public admin_restrictgren(HLCmd, HLData, HLUser, UID) {
  new Data[MAX_DATA_LENGTH];
  convert_string(HLData, Data, MAX_DATA_LENGTH);

  new Exec[MAX_DATA_LENGTH];
  snprintf(Exec, MAX_DATA_LENGTH, "restrict 1 %s", Data);
  exec(Exec);

  return PLUGIN_HANDLED;
}

public admin_unrestrictall(HLCmd, HLData, HLUser, UID) {
  new Data[MAX_DATA_LENGTH];
  convert_string(HLData, Data, MAX_DATA_LENGTH);
  
  new Exec[MAX_DATA_LENGTH];
  snprintf(Exec, MAX_DATA_LENGTH, "unrestrictall %s", Data);
  exec(Exec);

  return PLUGIN_HANDLED;
}

public admin_restricthelp(HLCmd, HLData, HLUser, UID) {
  new Data[MAX_DATA_LENGTH];
  convert_string(HLData, Data, MAX_DATA_LENGTH);
  
  if (streq(Data, "id")) {
    selfmessage("Possible grenade ids:");
    selfmessage("    0 - primary             1 - secondary");
    selfmessage(" ");
    selfmessage("Possible weapon ids:");
    selfmessage("    0 - crowbar             9 - medikit");
    selfmessage("    1 - shotgun            10 - super nailgun");
    selfmessage("    2 - nailgun            11 - assault cannon");
    selfmessage("    3 - sniperrifle        12 - flamethrower");
    selfmessage("    4 - autorifle          13 - incendiary rocket launcher");
    selfmessage("    5 - super shotgun      14 - knife");
    selfmessage("    6 - rocket launcher    15 - tranquilizer");
    selfmessage("    7 - grenade launcher   16 - wrench");
    selfmessage("    8 - pipebomb launcher  17 - railgun");
  } else if (streq(Data,"class")) {
    selfmessage("Possible classes:");
    selfmessage("    0 - all                 5 - medic");
    selfmessage("    1 - scout               6 - hwguy");
    selfmessage("    2 - sniper              7 - pyro");
    selfmessage("    3 - soldier             8 - spy");
    selfmessage("    4 - demoman             9 - engineer");
  } else {
    selfmessage("Unknown help category:");
    selfmessage(Data);
  }
  
  return PLUGIN_HANDLED;
}

public admin_blockfov(HLCmd, HLData, HLUser, UID) {
  new Data[MAX_DATA_LENGTH];
  convert_string(HLData, Data, MAX_DATA_LENGTH);

  new Exec[MAX_DATA_LENGTH];
  snprintf(Exec, MAX_DATA_LENGTH, "weaptfc_blockfov %s", Data);
  exec(Exec);

  return PLUGIN_HANDLED;
}

public plugin_init() {
  new weaptfc_version[MAX_NUMBER_LENGTH];
  getstrvar("weaptfc_version",weaptfc_version,MAX_NUMBER_LENGTH);
  plugin_registerinfo("TFC Weapon Restrictions - by CyberMind", "AdminMod frontend for TFC Weapon Restriction MetaMod plugin (weaptfc_mm)", weaptfc_version);
  
  plugin_registercmd("admin_restrictweap", "admin_restrictweap", ACCESS_WEAPTFC, "admin_restrictweap <id> <class> <0 | 1> - Enable/disable given weapon for given class");
  plugin_registercmd("admin_restrictgren", "admin_restrictgren", ACCESS_WEAPTFC, "admin_restrictgren <id> <class> <0 | 1> - Enable/disable given grenade for given class");
  plugin_registercmd("admin_unrestrictall", "admin_unrestrictall", ACCESS_WEAPTFC, "admin_unrestrictall [class=0] - Unrestrict all items for given class");
  plugin_registercmd("admin_restricthelp", "admin_restricthelp", ACCESS_WEAPTFC, "admin_restricthelp <id | class> - Gives help on the given subject");

  plugin_registercmd("admin_blockfov", "admin_blockfov", ACCESS_WEAPTFC, "admin_blockfov <0 | 1> - Enable/disable FOV command blocking");

  return PLUGIN_CONTINUE;  
}