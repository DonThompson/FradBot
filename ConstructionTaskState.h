#pragma once

enum class ConstructionTaskState
{
	eQueued = 0,
	eFindingPosition,
	eIssuingBuild,
	eConfirmOrders,
	eWaitingOnBuildStart,
	eConstructionInProgress,
	eInterrupted,
	eInterrupted_FindingNewWorker,
	eInterrupted_Resuming,	//Jumps back to eConstructionInProgress
	eCompleted
};
