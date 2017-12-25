# FradBot
Beginner Starcraft2 Bot

Current state:  Defeats all medium AI better than 80% of the time.

General:
* Generalize the rebalance logic in econ/supply manager to some kind of event system.  Game time is probably preferred.
* I haven't touched Query() yet, but I threw a few comments below for things that look promising.
* Speed checks.  Add some logic per game step in.  Avg across 5-10 seconds and look for max/avg.  student comp I saw fails you at:
** More than 1 frame longer than 10 seconds, or
** more than 10 frames longer than 1 second, or
** more than 320 frames longer than 85 milliseconds.

Supply manager issues:
* Supply can't keep up at higher levels.  Ocassionally supply blocked.  Seems happy till about 60-70.

Econ manager issues:
* SCVs keep going off to natural expansions when they built near the choke.
* Lower all depots on build?  SCVs get stuck occasionally.

Army manager issues:
* Queue units at the choke.  Getting too strung out because some are close and some are behind the line.
* Build orders.  Tough to beat the hard AI without a good plan.

Construction manager issues:
* Maybe claiming queue failure should also force the builder to clear his order?  I did see one real case where a builder got sent very far, the timeout was hit saying the build failed, but then he started.  The caller went and re-queued the intended building, resulting in a dupe.
* ConstructionTaskState::eInterrupted, ConstructionTaskState::eInterrupted_FindingNewWorker, ConstructionTaskState::eInterrupted_Resuming are unreachable.  No handling for lost builders.
* DoBuildingPositionsMatch:  We make an assumption of 1.0f distance in 2 positions is a match.  I have not dug into positioning to confirm this or see if there's a better method.
* HandleWaitingOnBuildStart:  We iterate through all units to find buildings under construction.  Can we find a filter that would slim this to just buildings?
* Loss of worker.  I've done no testing on this.  It's completely unhandled.  [EDIT:  I think I had this happen.  HandleConfirmingOrders had a nullptr builder.  I put in a check to bounce back in the queue here.  This was definitely pre-build-start.]

BuildingPlacement issues:
* Random building placement is a stupid way to place buildings.  This needs improved greatly.
* Query()->Placement(sc2::ABILITY_ID::BUILD_CREEPTUMOR, location) looks promising too.

Upgrades issues:
* Does this give us a better way to detect upgrades?  Query()->GetAbilitiesForUnit(unit)
* Upgrade spam once you get all the ebay upgrades but don't have an armory - it just keeps trying like mad to schedule an upgrade you can't do.

Structures manager issues:
* How to manage Utils::Get...Units()?  Should we copy these out to the appropriate managers for Structures, Workers, Army? (latter 2 don't exist yet).  For now they live in utils.  Leaning toward copying them 3x.
* Structure->building maybe should be private too.  Can we move all this into structure?

BaseLocationInitializer issues:
* This only seems to find exactly 4 mineral patches per base.  Good enough for what we want... but why not all of them? [https://github.com/Blizzard/s2client-api/issues/225]

Expansion issues:
* I need an expansion manager, I think.
* Almost certainly the construction manager is kicking the tasks from the queue and then re-building because of the long walk (see issues in construction manager).  More reason to go around this.

Suggestions:
* http://www.teamliquid.net/forum/starcraft-2/529138-improving-mineral-gathering-rate-in-sc2
* using statements in header is bad practice, clean that up.
* Now that we have base locations, go back and fix econ/construction to use the base, not random world wide




# Setup

## Get the precompiled Starcraft2 API from Blizzard's Github page
* Note:  The latest zip tested with this bot is included in the /reference/ folder.

See "Precompiled Libs" near the end.  Unzip the include & lib folders here.
https://github.com/Blizzard/s2client-api#coding-standard

Unzip, put the lib & include folders in a folder called 'sc2api-precompiled'.
Sibling to that, create your folder for this project -- 'FradBot'

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
* -m "RelativePath\File.SC2Map"

Maps can be provided as a full path ("D:\games\Blizzard\StarCraft II\Maps\Ladder2017Season1\PaladinoTerminalLE.SC2Map") or as a relative path inside the starcraft 2 install directory maps folder ("Ladder2017Season1\BelShirVestigeLE.SC2Map").  In all cases, the map must include the file extension.
Default map if not selected is Ladder2017Season1\BelShirVestigeLE.SC2Map.  You must get these files from the Blizzard API map download below.
Instructions for installing maps here:  https://github.com/Blizzard/s2client-proto#downloads

When ready, simply execute the compiled application with the appropriate command line parameters.


# Special project notes
NOTE:  pdb files are not included with precompiled libs, you will get a pile of warnings compiling in debug mode.  I disabled all 4099 warnings in Project Settings -> Linker -> Command Line -> Additional Options
