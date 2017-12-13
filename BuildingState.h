#pragma once

enum class BuildingState
{
	eQueued = 0,
	eFindingWorker,
	eFindingPosition,
	eIssuingBuild,
	eWaitingOnBuildStart,
	eConstructionInProgress,
	eInterrupted,
	eInterrupted_FindingNewWorker,
	eInterrupted_Resuming,	//Jumps back to eConstructionInProgress
	eCompleted
};
