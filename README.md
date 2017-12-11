# sc2api-scv
Beginner (SCV Level) Starcraft2 API

# Setup

## Get the precompiled Starcraft2 API from Blizzard's Github page
* Note:  The latest zip tested with this bot is included in the /reference/ folder.

See "Precompiled Libs" near the end.  Unzip the include & lib folders here.
https://github.com/Blizzard/s2client-api#coding-standard

Unzip, put the lib & include folders in a folder called 'sc2api-precompiled'.
Sibling to that, create your folder for this project -- 'sc1api-scv'

## Config
Project Settings -> C++ -> General - Additional Include Directories should point to ../sc2api-precompiled/include
Project Settings -> Linker -> General - Additional Library Directories should point to ../sc2api-precompiled/lib

NOTE:  pdb files are not included with precompiled libs, you will get a pile of warnings compiling in debug mode.

# Running the bot

main() has a few parameters you can configure when executing.

* CreateParticipant -- Sets the race of your bot.
* CreateComputer -- Sets the race of your opponent.
* StartGame -- Sets the map to use for this game.

When ready, simply execute the compiled application.
