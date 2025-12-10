#include "SoundManager.h"
#include "SoundDataAsset.h" 
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

USoundManager::USoundManager()
{
}

//version lien dans le init
//void USoundManager::Initialize(FSubsystemCollectionBase& Collection)
//{
//	Super::Initialize(Collection);
//
//	if (SoundLibraryPath.IsValid())
//	{
//		LoadedSoundLibrary = SoundLibraryPath.LoadSynchronous();
//	}
//
//	if (LoadedSoundLibrary == nullptr)
//	{
//		UE_LOG(LogTemp, Error, TEXT("LoadedSoundLibrary est NULL. ERREUR dans le chemin DefaultGame.ini : %s"), *SoundLibraryPath.ToString());
//	}
//	else
//	{
//		UE_LOG(LogTemp, Log, TEXT("Sound Library chargée avec succès."));
//	}
//}

void USoundManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FSoftObjectPath DebugPath(TEXT("/Game/BP/SoundSystem/DA_SoundLibrary.DA_SoundLibrary"));
	UObject* LoadedObject = DebugPath.TryLoad();
	USoundDataAsset* TempLibrary = Cast<USoundDataAsset>(LoadedObject);

	if (TempLibrary)
	{
		CachedSoundList = TempLibrary->SoundList; //copie des donnees dans le tableau

		UE_LOG(LogTemp, Warning, TEXT("Succès : %d sons copiés dans le Cache du SoundManager."), CachedSoundList.Num());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CRITICAL : Impossible de charger DA_SoundLibrary pour la copie !"));
	}
}

bool USoundManager::PlaySound(FName SoundName, UAudioComponent* AudioComponent)      //play sound par rapport a un audiocomponent fait pour sfx
{
	if (AudioComponent == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Impossible l audiocomponent est NULL."));
		return false;
	}

	// MODIFICATION ICI : On vérifie si notre cache est vide, plus le pointeur
	if (CachedSoundList.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlaySound: La liste de sons (Cache) est VIDE."));
		return false;
	}

	// MODIFICATION ICI : On cherche dans CachedSoundList
	FSoundDataStruct* FoundSound = CachedSoundList.FindByPredicate(
		[&SoundName](const FSoundDataStruct& Entry)
		{
			return Entry.SoundName == SoundName;
		}
	);

	if (FoundSound && FoundSound->SoundFile)
	{
		float FinalVolume = 1.0f;

		switch (FoundSound->SoundType)
		{
		case ESoundType::SFX:
			FinalVolume = FoundSound->VolumeMultiplier * VolumeMultiplierSFX * VolumeMultiplierGlobal;
			break;
		case ESoundType::Music:
			FinalVolume = FoundSound->VolumeMultiplier * VolumeMultiplierMusic * VolumeMultiplierGlobal;
			break;
		case ESoundType::Voice:
			FinalVolume = FoundSound->VolumeMultiplier * VolumeMultiplierVoice * VolumeMultiplierGlobal;
			break;
		}

		AudioComponent->SetSound(FoundSound->SoundFile);

		if (USoundWave* SoundWave = Cast<USoundWave>(FoundSound->SoundFile))
		{
			SoundWave->bLooping = FoundSound->SoundLoop;
		}
		AudioComponent->SetVolumeMultiplier(FinalVolume);
		AudioComponent->Play();
		// UE_LOG(LogTemp, Warning, TEXT("La valeur FinalVolume est de : %f "), FinalVolume);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Son null ou erreur nom : %s"), *SoundName.ToString());
		return false;
	}
}

bool USoundManager::StopSound(UAudioComponent* AudioComponent)     //stop sound par rapport a un audiocomponent
{
	if (AudioComponent != nullptr)
	{
		AudioComponent->Stop();

		// Si c'était une playlist
		if (PlaylistMap.Contains(AudioComponent))
		{
			// On utilise RemoveAll sur le Natif
			AudioComponent->OnAudioFinishedNative.RemoveAll(this);

			PlaylistMap.Remove(AudioComponent);
			PlaylistIndexMap.Remove(AudioComponent);
		}
		return true;
	}
	// ...
	return false;
}


//new

bool USoundManager::SoundPlaylist(const TArray<FName>& SoundNameList, UAudioComponent* AudioComponent) //check les données reçu et est appelé en blueprint
{
	if (AudioComponent == nullptr) return false;
	if (SoundNameList.Num() == 0) return false;

	// MODIFICATION ICI : Sécurité sur le cache
	if (CachedSoundList.Num() == 0) return false;

	for (const FName& NameToCheck : SoundNameList)
	{
		// MODIFICATION ICI : On cherche dans CachedSoundList
		FSoundDataStruct* FoundSound = CachedSoundList.FindByPredicate(
			[&NameToCheck](const FSoundDataStruct& Entry)
			{
				return Entry.SoundName == NameToCheck;
			}
		);

		if (FoundSound == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("SoundPlaylist ERREUR: Le son '%s' n'existe pas !"), *NameToCheck.ToString());
			return false;
		}
	}

	PlaySoundPlaylist(SoundNameList, AudioComponent);
	return true;
}

void USoundManager::SetMusicVolume()
{
	if (CachedSoundList.Num() == 0)
	{
		return;
	}

	TArray<UAudioComponent*> ComponentsToRemove;

	for (auto& Elem : PlaylistMap)
	{
		UAudioComponent* ActiveComponent = Elem.Key;
		TArray<FName>& List = Elem.Value;

		if (IsValid(ActiveComponent))
		{
			if (ActiveComponent->IsPlaying())
			{
				if (PlaylistIndexMap.Contains(ActiveComponent))
				{
					int32 CurrentIndex = PlaylistIndexMap[ActiveComponent];

					if (List.IsValidIndex(CurrentIndex))
					{
						FName CurrentSoundName = List[CurrentIndex];

						FSoundDataStruct* FoundSound = CachedSoundList.FindByPredicate(
							[&CurrentSoundName](const FSoundDataStruct& Entry) { return Entry.SoundName == CurrentSoundName; }
						);

						if (FoundSound && FoundSound->SoundType == ESoundType::Music)
						{
							float NewFinalVolume = FoundSound->VolumeMultiplier * VolumeMultiplierMusic * VolumeMultiplierGlobal;
							ActiveComponent->SetVolumeMultiplier(NewFinalVolume);
						}
					}
				}
			}
		}
		else
		{
			ComponentsToRemove.Add(ActiveComponent);
		}
	}

	for (UAudioComponent* CompToRemove : ComponentsToRemove)
	{
		PlaylistMap.Remove(CompToRemove);
		PlaylistIndexMap.Remove(CompToRemove);
	}
}



void USoundManager::PlaySoundPlaylist(TArray<FName> SoundList, UAudioComponent* AudioComponent) //enregistre le resultat
{

	// On associe cet AudioComponent a la liste de sons fournie
	PlaylistMap.Add(AudioComponent, SoundList);

	// On initialise son compteur a 0 (le début de la liste)
	PlaylistIndexMap.Add(AudioComponent, 0);


	AudioComponent->OnAudioFinishedNative.RemoveAll(this);


	AudioComponent->OnAudioFinishedNative.AddUObject(this, &USoundManager::OnPlaylistAudioFinished);

	// 3. Lecture
	PlayNextSoundInPlaylist(AudioComponent);
}

void USoundManager::PlayNextSoundInPlaylist(UAudioComponent* AudioComponent) //joue les sons
{

	if (!PlaylistMap.Contains(AudioComponent) || !PlaylistIndexMap.Contains(AudioComponent)) return;

	// MODIFICATION ICI : On vérifie le cache local
	if (CachedSoundList.Num() == 0) return;

	int32 CurrentIndex = PlaylistIndexMap[AudioComponent];
	TArray<FName>& CurrentList = PlaylistMap[AudioComponent];

	if (!CurrentList.IsValidIndex(CurrentIndex)) return;

	FName CurrentSoundName = CurrentList[CurrentIndex];

	// MODIFICATION ICI : On cherche dans CachedSoundList
	FSoundDataStruct* FoundSound = CachedSoundList.FindByPredicate(
		[&CurrentSoundName](const FSoundDataStruct& Entry)
		{
			return Entry.SoundName == CurrentSoundName;
		}
	);

	if (FoundSound && FoundSound->SoundFile)
	{
		float FinalVolume = 1.0f;

		switch (FoundSound->SoundType)
		{
		case ESoundType::SFX:
			FinalVolume = FoundSound->VolumeMultiplier * VolumeMultiplierSFX * VolumeMultiplierGlobal;
			break;
		case ESoundType::Music:
			FinalVolume = FoundSound->VolumeMultiplier * VolumeMultiplierMusic * VolumeMultiplierGlobal;
			break;
		case ESoundType::Voice:
			FinalVolume = FoundSound->VolumeMultiplier * VolumeMultiplierVoice * VolumeMultiplierGlobal;
			break;
		}

		AudioComponent->SetSound(FoundSound->SoundFile);

		if (USoundWave* SoundWave = Cast<USoundWave>(FoundSound->SoundFile))
		{
			SoundWave->bLooping = false;
		}

		AudioComponent->SetVolumeMultiplier(FinalVolume);
		AudioComponent->Play();

		UE_LOG(LogTemp, Log, TEXT("Playlist joue index %d : %s"), CurrentIndex, *CurrentSoundName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Son introuvable, suivant."));
		OnPlaylistAudioFinished(AudioComponent);
	}
}

void USoundManager::OnPlaylistAudioFinished(UAudioComponent* FinishedComponent)
{
	// On verifie que c est bien un component qui joue une playlist
	if (PlaylistMap.Contains(FinishedComponent) && PlaylistIndexMap.Contains(FinishedComponent))
	{
		// On recupere l index par reference pour le modifier
		int32& Index = PlaylistIndexMap[FinishedComponent];
		const TArray<FName>& List = PlaylistMap[FinishedComponent];

		// On passe au suivant
		Index++;

		// Si on a depasse la fin de la liste, on revient a 0
		if (Index >= List.Num())
		{
			Index = 0;
			UE_LOG(LogTemp, Log, TEXT("Playlist terminée, on reboucle au début."));
		}

		// On joue le nouveau son
		PlayNextSoundInPlaylist(FinishedComponent);
	}
}

void USoundManager::Deinitialize()
{
	// On vide le tableau local a la fin
	CachedSoundList.Empty();

	Super::Deinitialize();
}

