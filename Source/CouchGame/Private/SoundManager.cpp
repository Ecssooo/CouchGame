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

	//lien en hard code
	FSoftObjectPath DebugPath(TEXT("/Game/BP/SoundSystem/DA_SoundLibrary.DA_SoundLibrary"));

	//On utilise TryLoad() au lieu de LoadSynchronous()
	UObject* LoadedObject = DebugPath.TryLoad();

	// On cast l objet générique vers le type specifique
	LoadedSoundLibrary = Cast<USoundDataAsset>(LoadedObject);

	if (LoadedSoundLibrary)
	{
		UE_LOG(LogTemp, Warning, TEXT("Le chargement fonctionne de la library de son ! Le fichier est bien dans le build."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ERREUR Le fichier n'est pas dans le build"));
	}
}

bool USoundManager::PlaySound(FName SoundName, UAudioComponent* AudioComponent)      //play sound par rapport a un audiocomponent fait pour sfx
{
	//verifie si l audio component n est pas null
	if (AudioComponent == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Impossible l audiocomponent est NULL."));
		return false;
	}

	// verifie si la bibliotheque est charge
	if (LoadedSoundLibrary == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupAndPlaySound: LoadedSoundLibrary est NUL."));
		return false;
	}

	//Trouver le son dans le TArray
	FSoundDataStruct* FoundSound = LoadedSoundLibrary->SoundList.FindByPredicate(
		[&SoundName](const FSoundDataStruct& Entry)
		{
			return Entry.SoundName == SoundName;
		}
	);

	// Jouer le son si on l a trouver
	if (FoundSound && FoundSound->SoundFile)
	{
		float FinalVolume = 1.0f; // Le volume de la categorie
		//

		// --- On determine le volume de la categorie ---
		switch (FoundSound->SoundType)
		{
		case ESoundType::SFX:
			FinalVolume = FoundSound->VolumeMultiplier * VolumeMultiplierSFX * VolumeMultiplierGlobal;  //je multiple pour avoir le bon volume
			break;

		case ESoundType::Music:
			FinalVolume = FoundSound->VolumeMultiplier * VolumeMultiplierMusic * VolumeMultiplierGlobal;  //je multiple pour avoir le bon volume
			break;

		case ESoundType::Voice:
			FinalVolume = FoundSound->VolumeMultiplier * VolumeMultiplierVoice * VolumeMultiplierGlobal;  //je multiple pour avoir le bon volume
			break;
		}

		AudioComponent->SetSound(FoundSound->SoundFile); //recupere le son et le met dans le audiocomponent

		if (USoundWave* SoundWave = Cast<USoundWave>(FoundSound->SoundFile)) //creer une variable de type soundwave pour ensuite mettre le fichier en question en loop
		{
			SoundWave->bLooping = FoundSound->SoundLoop; //  modifie l asset en memoire mais le met en loop
		}
		AudioComponent->SetVolumeMultiplier(FinalVolume);
		AudioComponent->Play(); // start son
		UE_LOG(LogTemp, Warning, TEXT("La valeur FinalVolume est de : %f "), FinalVolume);  // pour dire ou afficher le float %f
		return true;
	}
	else
	{
		//erreur
		UE_LOG(LogTemp, Warning, TEXT("Son null ou erreur nom"), *SoundName.ToString());
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
	// 1. Sécurité
	if (AudioComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ArraySound: AudioComponent est NULL."));
		return false;
	}

	if (SoundNameList.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ArraySound: Array vide, rien à jouer."));
		return false;
	}

	// On boucle sur chaque nom demandé pour voir s'il existe dans la database
	for (const FName& NameToCheck : SoundNameList)
	{
		// On cherche le nom dans la liste (meme methode que PlaySound)
		FSoundDataStruct* FoundSound = LoadedSoundLibrary->SoundList.FindByPredicate(
			[&NameToCheck](const FSoundDataStruct& Entry)
			{
				return Entry.SoundName == NameToCheck;
			}
		);

		// Si le pointeur est null, c est que le son n existe pas
		if (FoundSound == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("SoundPlaylist ERREUR: Le son '%s' n'existe pas dans la Database ! Playlist annulée."), *NameToCheck.ToString());
			return false; 
		}
	}

	PlaySoundPlaylist(SoundNameList,AudioComponent);

	return true;
}

void USoundManager::SetMusicVolume()
{
	// On parcourt notre le dictionnaire pour retrouver tous les composants actifs
	for (auto& Elem : PlaylistMap)
	{
		UAudioComponent* ActiveComponent = Elem.Key; // La cler est le composant
		TArray<FName>& List = Elem.Value;           // La valeur est la liste

		// On vérifie qu'il est valide et qu'il joue
		if (ActiveComponent && ActiveComponent->IsPlaying())
		{
			// On a besoin de retrouver l'index actuel pour savoir quel son joue
			if (PlaylistIndexMap.Contains(ActiveComponent))
			{
				int32 CurrentIndex = PlaylistIndexMap[ActiveComponent];

				// Sécurité index
				if (List.IsValidIndex(CurrentIndex))
				{
					FName CurrentSoundName = List[CurrentIndex];

					// On retourne chercher les infos du son dans le DataAsset
					if (LoadedSoundLibrary)
					{
						FSoundDataStruct* FoundSound = LoadedSoundLibrary->SoundList.FindByPredicate(
							[&CurrentSoundName](const FSoundDataStruct& Entry) { return Entry.SoundName == CurrentSoundName; }
						);

						// verifie le type
						if (FoundSound && FoundSound->SoundType == ESoundType::Music)
						{

							float NewFinalVolume = FoundSound->VolumeMultiplier * VolumeMultiplierMusic * VolumeMultiplierGlobal;

							//ensuite je change le volume meme si le son est en cours
							ActiveComponent->SetVolumeMultiplier(NewFinalVolume);
						}
					}
				}
			}
		}
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
	if (LoadedSoundLibrary == nullptr) return;

	// Récuperer le nom du son actuel grace a l index stocker
	int32 CurrentIndex = PlaylistIndexMap[AudioComponent];
	TArray<FName>& CurrentList = PlaylistMap[AudioComponent];

	// Petite securiter si l index est hors limites
	if (!CurrentList.IsValidIndex(CurrentIndex)) return;

	FName CurrentSoundName = CurrentList[CurrentIndex];

	// Trouver le son dans le TArray
	FSoundDataStruct* FoundSound = LoadedSoundLibrary->SoundList.FindByPredicate(
		[&CurrentSoundName](const FSoundDataStruct& Entry)
		{
			return Entry.SoundName == CurrentSoundName;
		}
	);

	if (FoundSound && FoundSound->SoundFile)
	{
		float FinalVolume = 1.0f;

		// --- On determine le volume de la categorie ---
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

		
		// Si le son boucle, il ne se finit jamais, et on ne passe jamais au suivant !
		if (USoundWave* SoundWave = Cast<USoundWave>(FoundSound->SoundFile))
		{
			SoundWave->bLooping = false; // Forcer a false pour permettre l enchaînement
		}

		AudioComponent->SetVolumeMultiplier(FinalVolume);
		AudioComponent->Play(); // Start son

		UE_LOG(LogTemp, Log, TEXT("Playlist joue index %d : %s"), CurrentIndex, *CurrentSoundName.ToString());
	}
	else
	{
		// Si le son est introuvable, on simule sa fin pour passer au suivant immédiatement
		UE_LOG(LogTemp, Warning, TEXT("Son introuvable dans la playlist, passage au suivant."));
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