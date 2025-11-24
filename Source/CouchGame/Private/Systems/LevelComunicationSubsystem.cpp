#include "Systems/LevelComunicationSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/LevelComunicationManager.h"


class ALevelComunicationManager;

void ULevelComunicationSubsystem::InitLevelData()
{
	for (int i = 0; i < 6; i++)
	{
		FCubeLevelOld level;
		level.IdLevel = i;

		for (int j = 0; j < 9; j++)
		{
			FSubCubeLevelOld sublevel;
			sublevel.IdSublevel = j;
			sublevel.isDiscover = false;
			level.Sublevels.Add(sublevel);
		}
		AllLevels.Add(level);
	}
}

FSubCubeLevelOld& ULevelComunicationSubsystem::FindSubLevel(int IdFace, int IdSubLevel)
{
	return AllLevels[IdFace].Sublevels[IdSubLevel];	
}

void ULevelComunicationSubsystem::DiscoveredSubLevel(int IdFace, int IdSubLevel)
{
	FSubCubeLevelOld& SubLevel = FindSubLevel(IdFace, IdSubLevel);
	SubLevel.isDiscover = true;
	
	ALevelComunicationManager* ComManager = Cast<ALevelComunicationManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelComunicationManager::StaticClass()));
	if (!ComManager) return;

	// if (ComManager->LevelID == IdFace)
	// {
	// 	ComManager->LoadSpecificPartition(IdSubLevel);
	// }
}



// void ULevelComunicationSubsystem::AriseSubLevel(int id)
// {
// 	APartitionLevel* PL = FindPartitionLevel(id);
// 	if (!PL) return;
// 	PL->DiscoverSubLevel();
// }