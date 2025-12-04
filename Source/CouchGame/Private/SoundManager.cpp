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
		return true;
	}
	else
	{
		//erreur
		UE_LOG(LogTemp, Warning, TEXT("Son null ou erreur nom"), *SoundName.ToString());
		return false;
	}
}

bool USoundManager::StopSound(UAudioComponent* AudioComponent)     //stop sound par rapport a un audiocomponent fait pour sfx
{
	// verifie si la bibliotheque est charge
	if (LoadedSoundLibrary == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupAndPlaySound: LoadedSoundLibrary est NUL."));
		return false;
	}

	//verifie si l audio component n est pas null
	if (AudioComponent != nullptr) {
		//juste je stop audiocomponent en question
		AudioComponent->Stop(); // stop
		return true; // ok son off
	}else {
		//erreur
		UE_LOG(LogTemp, Warning, TEXT("Impossible l'audiocomponent est NULL."));
		return false;
	}
}


