# WEAPTFC_MM

* Allows restriction of any weapon and grenade by class (i.e. can prevent medic from using super shotgun, but not other classes)  
* Allows blocking of the 'fov' client command  
* Allows scripting command registration when a player respawns, or changes class or team  
* Allows delayed class-changes, preventing several TFC bugs/exploits  
    
    ========
    COMMANDS
    ========
    
    restrict <type> <id> <class> <0 | 1> - restricts or unrestricts given item for given class
    --------------------------------------------------------------------------------------------
    <type> is the type of item being restricted:
        0 - weapons              1 - grenades
    
    <id> is the weapon/grenade id:
        Grenade ids:
        0 - primary              1 - secondary
    
        Weapon ids:
        0 - crowbar              9 - medikit
        1 - shotgun              10 - super nailgun
        2 - nailgun              11 - assault cannon
        3 - sniperrifle          12 - flamethrower
        4 - autorifle            13 - incendiary rocket launcher
        5 - super shotgun        14 - knife
        6 - rocket launcher      15 - tranquilizer
        7 - grenade launcher     16 - wrench
        8 - pipebomb launcher    17 - railgun
    
    <class> is the class to restrict the weapon for:
        0 - all                  5 - medic
        1 - scout                6 - hwguy
        2 - sniper               7 - pyro
        3 - soldier              8 - spy
        4 - demoman              9 - engineer
    
    <0 | 1> is to set the status of the restriction:
        0 - unrestricted         1 - restricted
    
    
    
    unrestrictall [class=0] - unrestricts all items (grenades and weapons) for given class
    ----------------------------------------------------------------------------------------
    [class=0] is the class to unrestrict, if omitted, 0 is used meaning all classes
    (effectively allowing all classes to use all items)
    
    
    
    reg_class_cmd <cmd> - registers a command to execute when a player changes class
    --------------------------------------------------------------------------------
    <cmd> is any server command to execute, it will be executed in the following manner:
        <cmd> <playerindex> <oldclass> <newclass>
    
    
    
    reg_respawn_cmd <cmd> - registers a command to execute when a player respawns
    -----------------------------------------------------------------------------
    <cmd> is any server command to execute, it will be executed in the following manner:
        <cmd> <playerindex>
    
    
    
    reg_team_cmd <cmd> - registers a command to execute when a player changes team
    -----------------------------------------------------------------------------
    <cmd> is any server command to execute, it will be executed in the following manner:
        <cmd> <playerindex> <oldteam> <newteam>
    
    
    
    =====
    CVARS
    =====
    
    weaptfc_version                the version of this MetaMod plugin
    weaptfc_blockfov            if enabled, blocks users from using the FOV command to zoom-in
    weaptfc_blockfastswitch        if enabled, does not allow class-changes within 2 seconds
