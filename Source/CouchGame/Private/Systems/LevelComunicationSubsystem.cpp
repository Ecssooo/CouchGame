#include "Systems/LevelComunicationSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/PartitionLevel.h"


void ULevelComunicationSubsystem::InitLevelData()
{
	for (int i = 0; i < 6; i++)
	{
		FCubeLevel level;
		level.IdLevel = i;

		for (int j = 0; j < 9; j++)
		{
			FSubCubeLevel sublevel;
			sublevel.IdSublevel = j;
			sublevel.isDiscover = false;
			level.Sublevels.Add(sublevel);
		}
		AllLevels.Add(level);
	}
}

FSubCubeLevel& ULevelComunicationSubsystem::FindSubLevel(int IdFace, int IdSubLevel)
{
	return AllLevels[IdFace].Sublevels[IdSubLevel];	
}

void ULevelComunicationSubsystem::DiscoveredSubLevel(int IdFace, int IdSubLevel)
{
	FSubCubeLevel& SubLevel = FindSubLevel(IdFace, IdSubLevel);
	SubLevel.isDiscover = true;
	//Si c'est la même ID Face
	
	
	AriseSubLevel(IdSubLevel);
}

void ULevelComunicationSubsystem::GetPartitionLevelsInWorld()
{
	TArray<AActor*> FoundActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APartitionLevel::StaticClass(), FoundActor);
	AllPartitionLevels.Empty();
	for (int i = 0; i < FoundActor.Num(); i++)
	{
		APartitionLevel* PartitionLevel = Cast<APartitionLevel>(FoundActor[i]);
		if (!PartitionLevel) continue;
		AllPartitionLevels.Add(PartitionLevel);
	}
}

APartitionLevel* ULevelComunicationSubsystem::FindPartitionLevel(int Id)
{
	for (int i = 0; i < AllPartitionLevels.Num(); i++ )
	{
		if (Id == AllPartitionLevels[i]->IdSublevel)
		{
			return AllPartitionLevels[i];
		}
	}
	
	return nullptr;
}

void ULevelComunicationSubsystem::AriseSubLevel(int id)
{
	APartitionLevel* PL = FindPartitionLevel(id);
	if (!PL) return;
	PL->DiscoverSubLevel();
}

void ULevelComunicationSubsystem::LevelLoaded(int idFace)
{
	for (int i = 0; i < AllLevels[idFace].Sublevels.Num(); i++)
	{
		if (AllLevels[idFace].Sublevels[i].isDiscover)
		{
			APartitionLevel* PL = FindPartitionLevel(i);
			if (!PL) continue;
			PL->DiscoverSubLevel();
		}
	}
}