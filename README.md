# sc2api-scv
Beginner (SCV Level) Starcraft2 API

Current state:  Defeats all medium AI better than 80% of the time.

Major known issues:
* Multi-actions on a step.  If a barracks needs built, it will often build as many as it can.  Same with refineries and any other building.
* Supply can't keep up.  Regularly supply blocked.

# Setup

## Get the precompiled Starcraft2 API from Blizzard's Github page
* Note:  The latest zip tested with this bot is included in the /reference/ folder.

See "Precompiled Libs" near the end.  Unzip the include & lib folders here.
https://github.com/Blizzard/s2client-api#coding-standard

Unzip, put the lib & include folders in a folder called 'sc2api-precompiled'.
Sibling to that, create your folder for this project -- 'sc1api-scv'

## One time configuration
Project Settings -> C++ -> General - Additional Include Directories should point to ../sc2api-precompiled/include  (or wherever you load these files)
Project Settings -> Linker -> General - Additional Library Directories should point to ../sc2api-precompiled/lib  (or wherever you load these files)


# Running the bot

The bot will currently always play as Terran.  There are a several command line parameters you can use when launching the bot.  Note all commands are case sensitive.
**Opponent Race**
* -or Random (default value)
* -ot Terran
* -oz Zerg
* -op Protoss

**Game Speed**
* -s0 Super fast AI game speed.  (default value)
* -s1 Realtime game speed option 1.
* -s2 Realtime game speed option 2.  See code for the small differences in option 1 and option 2.  Also read:  https://github.com/Blizzard/s2client-api/issues/179

**Opponent Difficulty**
* -dVE Very Easy
* -dE Easy
* -dM Medium (default value)
* -dMH Medium Hard
* -dH Hard
* -dHVH Hard Very Hard
* -dVH Very Hard
* -dI Insane (cheat mode)

**Map**
TODO:  Map not yet configurable

When ready, simply execute the compiled application with the appropriate command line parameters.


# Special project notes
NOTE:  pdb files are not included with precompiled libs, you will get a pile of warnings compiling in debug mode.  I disabled all 4099 warnings in Project Settings -> Linker -> Command Line -> Additional Options
