# sc2api-scv
Beginner (SCV Level) Starcraft2 API

# Setup

## Get the precompiled Starcraft2 API from Blizzard's Github page
See "Precompiled Libs" near the end.  Unzip the include & lib folders here.
https://github.com/Blizzard/s2client-api#coding-standard

Unzip, put the lib & include folders in a folder called 'sc2api-precompiled'.
Sibling to that, create your folder for this project -- 'sc1api-scv'

## One time configuration
Project Settings -> C++ -> General - Additional Include Directories should point to ../sc2api-precompiled/include  (or wherever you load these files)
Project Settings -> Linker -> General - Additional Library Directories should point to ../sc2api-precompiled/lib  (or wherever you load these files)

NOTE:  pdb files are not included with precompiled libs, you will get a pile of warnings compiling in debug mode.

# Running the bot

main() has a few parameters you can configure when executing.

* CreateParticipant -- Sets the race of your bot.  Bot is currently very hardcoded to play as Terran.  Other races are unlikely to function.
* CreateComputer -- Sets the race of your opponent and its difficulty.
* StartGame -- Sets the map to use for this game.

When ready, simply execute the compiled application.
