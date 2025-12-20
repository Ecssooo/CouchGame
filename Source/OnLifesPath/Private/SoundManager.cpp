#include "SoundManager.h"
#include "SoundDataAsset.h" 
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

USoundManager::USoundManager()
{
}

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

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &USoundManager::HandlePreLoadMap);
}

bool USoundManager::PlaySound(FName SoundName, UAudioComponent* AudioComponent)      //play sound par rapport a un audiocomponent
{
	if (AudioComponent == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Impossible l audiocomponent est NULL."));
		return false;
	}

	// On verifie si le cache est vide
	if (CachedSoundList.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlaySound: La liste de sons (Cache) est VIDE."));
		return false;
	}

	//On cherche dans CachedSoundList
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

		if (PlaylistMap.Contains(AudioComponent))
		{
			AudioComponent->OnComponentDeactivated.RemoveDynamic(this, &USoundManager::OnComponentDeactivated);
			AudioComponent->OnAudioFinishedNative.RemoveAll(this);

			PlaylistMap.Remove(AudioComponent);
			PlaylistIndexMap.Remove(AudioComponent);
		}
		return true;
	}
	return false;
}


//new

bool USoundManager::SoundPlaylist(const TArray<FName>& SoundNameList, UAudioComponent* AudioComponent) //check les donnees recu et est appeler en blueprint
{
	if (AudioComponent == nullptr) return false;
	if (SoundNameList.Num() == 0) return false;

	//Securiter sur le cache
	if (CachedSoundList.Num() == 0) return false;

	for (const FName& NameToCheck : SoundNameList)
	{
		//On cherche dans CachedSoundList
		FSoundDataStruct* FoundSound = CachedSoundList.FindByPredicate(
			[&NameToCheck](const FSoundDataStruct& Entry)
			{
				return Entry.SoundName == NameToCheck;
			}
		);

		if (FoundSound == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("SoundPlaylist ERREUR: Le son '%s' n existe pas !"), *NameToCheck.ToString());
			return false;
		}
	}

	PlaySoundPlaylist(SoundNameList, AudioComponent);
	return true;
}

void USoundManager::SetMusicVolume()
{
	if (CachedSoundList.Num() == 0) return;

	// Si la liste est vide (grace a HandlePreLoadMap) (crash)
	if (PlaylistMap.Num() == 0) return;

	//Si le monde est en destruction, on arrete tout
	if (UWorld* World = GetWorld())
	{
		if (World->bIsTearingDown) return;
	}

	//boucle avec iterateur
	for (auto It = PlaylistMap.CreateIterator(); It; ++It)
	{
		UAudioComponent* ActiveComponent = It.Key();

		if (IsValid(ActiveComponent))
		{
			if (ActiveComponent->IsPlaying())
			{
				if (PlaylistIndexMap.Contains(ActiveComponent))
				{
					int32 CurrentIndex = PlaylistIndexMap[ActiveComponent];
					TArray<FName>& List = It.Value();

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
			It.RemoveCurrent();
			if (ActiveComponent)
			{
				PlaylistIndexMap.Remove(ActiveComponent);
			}
		}
	}
}



void USoundManager::PlaySoundPlaylist(TArray<FName> SoundList, UAudioComponent* AudioComponent) //enregistre le resultat
{
	if (AudioComponent == nullptr) return;

	// si deja enregistrer evite les doublons
	if (PlaylistMap.Contains(AudioComponent))
	{
		//On utilise OnComponentDeactivated
		AudioComponent->OnComponentDeactivated.RemoveDynamic(this, &USoundManager::OnComponentDeactivated);
		AudioComponent->OnAudioFinishedNative.RemoveAll(this);
	}

	PlaylistMap.Add(AudioComponent, SoundList);
	PlaylistIndexMap.Add(AudioComponent, 0);

	//Bind
	AudioComponent->OnAudioFinishedNative.AddUObject(this, &USoundManager::OnPlaylistAudioFinished);

	//On s abonne a la desactivation
	if (!AudioComponent->OnComponentDeactivated.IsAlreadyBound(this, &USoundManager::OnComponentDeactivated))
	{
		AudioComponent->OnComponentDeactivated.AddDynamic(this, &USoundManager::OnComponentDeactivated);
	}

	PlayNextSoundInPlaylist(AudioComponent);
}

void USoundManager::PlayNextSoundInPlaylist(UAudioComponent* AudioComponent) //joue les sons
{

	if (!PlaylistMap.Contains(AudioComponent) || !PlaylistIndexMap.Contains(AudioComponent)) return;

	//On verifie le cache local
	if (CachedSoundList.Num() == 0) return;

	int32 CurrentIndex = PlaylistIndexMap[AudioComponent];
	TArray<FName>& CurrentList = PlaylistMap[AudioComponent];

	if (!CurrentList.IsValidIndex(CurrentIndex)) return;

	FName CurrentSoundName = CurrentList[CurrentIndex];

	//On cherche dans CachedSoundList
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
			UE_LOG(LogTemp, Log, TEXT("Playlist terminee, on reboucle au début."));
		}

		// On joue le nouveau son
		PlayNextSoundInPlaylist(FinishedComponent);
	}
}

void USoundManager::OnComponentDeactivated(UActorComponent* Component)
{
	// On cast en AudioComponent
	UAudioComponent* AudioComp = Cast<UAudioComponent>(Component);

	if (AudioComp)
	{
		// Si ce composant etait dans notre playlist, on le retire
		if (PlaylistMap.Contains(AudioComp))
		{
			// Nettoyage final
			AudioComp->OnComponentDeactivated.RemoveDynamic(this, &USoundManager::OnComponentDeactivated);
			AudioComp->OnAudioFinishedNative.RemoveAll(this);

			PlaylistMap.Remove(AudioComp);
			PlaylistIndexMap.Remove(AudioComp);

			UE_LOG(LogTemp, Log, TEXT("SoundManager: AudioComponent desactive/detruit, retire de la PlaylistMap."));
		}
	}
}

void USoundManager::HandlePreLoadMap(const FString& MapName)
{
	// nettoyage 
	UE_LOG(LogTemp, Warning, TEXT("SoundManager: Changement de map detecter (%s). Nettoyage forcer."), *MapName);

	PlaylistMap.Empty();
	PlaylistIndexMap.Empty();
}

void USoundManager::Deinitialize()
{
	// On vide le tableau local a la fin
	CachedSoundList.Empty();

	Super::Deinitialize();
}

