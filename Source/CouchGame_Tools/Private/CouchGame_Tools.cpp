#include "CouchGame_Tools.h"

#include "FToolsEditorStyle.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Systems/CharacterSettings.h"
#include "LevelEditorSubsystem.h"

#define LOCTEXT_NAMESPACE "FCouchGame_ToolsModule"

void FCouchGame_ToolsModule::StartupModule()
{
	FToolsEditorStyle::Initialize();
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCouchGame_ToolsModule::CreateUseKeyboardToggle));
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCouchGame_ToolsModule::CreateRecompileButton));
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCouchGame_ToolsModule::CreateMainLevelButton));
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCouchGame_ToolsModule::CreateLoadLevelButton));
}

void FCouchGame_ToolsModule::ShutdownModule()
{
	// Unregister the startup function
	UToolMenus::UnRegisterStartupCallback(this);
	FToolsEditorStyle::Shutdown();
	// Unregister all our menu extensions
	UToolMenus::UnregisterOwner(this);
}
#pragma region Use Keyboard
void FCouchGame_ToolsModule::CreateUseKeyboardToggle()
{
	UToolMenu* Toolbar = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");

	FToolMenuSection& Section = Toolbar->FindOrAddSection("PluginTools");

	Section.AddEntry(
		FToolMenuEntry::InitToolBarButton("UseKeyboard",
			
		FUIAction(FExecuteAction::CreateRaw(this,&FCouchGame_ToolsModule::OnToggleKeyboardController),
		FCanExecuteAction(),
		FIsActionChecked::CreateRaw(this, &FCouchGame_ToolsModule::GetUseKeyboardController)),
		
	LOCTEXT("UseKeyboard_Label", "Keyboard"),

	LOCTEXT("UseKeyboard_ToolTip", "Use Keyboard controller for debug"),

	FSlateIcon(FToolsEditorStyle::GetStyleSetName(), "GameTools.KeyboardIcon"),
	EUserInterfaceActionType::ToggleButton
	)
	);
}

void FCouchGame_ToolsModule::OnToggleKeyboardController()
{
	UCharacterSettings* CharacterSettings = GetMutableDefault<UCharacterSettings>();
	CharacterSettings->UsKeyboardControl = !CharacterSettings->UsKeyboardControl;
	CharacterSettings->SaveConfig();
	CharacterSettings->TryUpdateDefaultConfigFile();
}

bool FCouchGame_ToolsModule::GetUseKeyboardController()
{
	UCharacterSettings* CharacterSettings = GetMutableDefault<UCharacterSettings>();
	return CharacterSettings->UsKeyboardControl;
}



#pragma endregion

#pragma region Recompile

void FCouchGame_ToolsModule::CreateRecompileButton()
{
	UToolMenu* Toolbar = UToolMenus::Get()->ExtendMenu("ContentBrowser.Toolbar");

	FToolMenuSection& Section = Toolbar->FindOrAddSection("Save");

	Section.AddEntry(FToolMenuEntry::InitToolBarButton(
		"Recompile blueprint",
		FUIAction(FExecuteAction::CreateRaw(this, &FCouchGame_ToolsModule::RecompileAllBlueprint)),
			LOCTEXT("RecompileAllBlueprints_Label", ""),
			LOCTEXT("RecompileAllBlueprints_Tooltip", "Recompile all Blueprints in the project"),
			FSlateIcon(FToolsEditorStyle::GetStyleSetName(), "GameTools.RecompileIcon"))
		);
}

void FCouchGame_ToolsModule::RecompileAllBlueprint()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FARFilter Filter;
	Filter.ClassPaths.Add(FTopLevelAssetPath(TEXT("/Script/Engine"), TEXT("Blueprint"))); // UBlueprint
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Add(FName("/Game")); // Content root, adapter si besoin

	TArray<FAssetData> AssetList;
	AssetRegistry.GetAssets(Filter, AssetList);

	int32 RecompileCount = 0;

	for (const FAssetData& Asset : AssetList)
	{
		// Charger le Blueprint depuis le package
		UBlueprint* Blueprint = Cast<UBlueprint>(Asset.GetAsset());
		if (!Blueprint)
		{
			// Blueprint = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), nullptr, *Asset.ObjectPath.ToString()));
		}

		if (Blueprint)
		{
			if (Blueprint->IsPossiblyDirty())
			{
				FKismetEditorUtilities::CompileBlueprint(Blueprint);
				RecompileCount++;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Recompiled %d Blueprints"), RecompileCount);
}
#pragma endregion

#pragma region Load Level
void FCouchGame_ToolsModule::CreateMainLevelButton()
{
	UToolMenu* Toolbar = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");

	FToolMenuSection& Section = Toolbar->FindOrAddSection("LevelSection");

	Section.AddEntry(
		FToolMenuEntry::InitToolBarButton("LoadMainLevel",
			
		FUIAction(FExecuteAction::CreateRaw(this,&FCouchGame_ToolsModule::LoadLevelButton)),
	LOCTEXT("LoadMainLevel_Label", "MainLevel"),
	LOCTEXT("LoadMainLevel_ToolTip", "Use Keyboard controller for debug"),

	FSlateIcon(FToolsEditorStyle::GetStyleSetName(), "GameTools.MainLevel")
	)
	);
}

void FCouchGame_ToolsModule::LoadLevelButton()
{
	ULevelEditorSubsystem* LevelSubsystem =
		GEditor->GetEditorSubsystem<ULevelEditorSubsystem>();

	if (LevelSubsystem)
	{
		LevelSubsystem->LoadLevel(TEXT("/Game/Levels/Cube/Main.Main"));
	}
}

void FCouchGame_ToolsModule::CreateLoadLevelButton()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	UToolMenu* Toolbar = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");

	FToolMenuSection& Section = Toolbar->FindOrAddSection("LevelSection");

	Section.AddSubMenu(
		"FacesDropDown",
		LOCTEXT("CouchGameDropdownLabel", "FaceDropDown"),
		LOCTEXT("CouchGameDropdownTooltip", "FaceDropDown"),
		FNewToolMenuDelegate::CreateLambda([](UToolMenu* SubMenu)
		{
			FToolMenuSection& SubSection = SubMenu->AddSection("CouchGameSection", LOCTEXT("SubsectionLabel", "Load Face"));
			for (int i = 1; i<=6; i++)
			{
				SubSection.AddMenuEntry(
				*FString::Printf(TEXT("Face%i"), i),
				FText::Format(LOCTEXT("OptionALabel", "Face {index}"), i),
				FText::Format(LOCTEXT("OptionALabel", "Face {index}"), i),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateLambda([i] {
					ULevelEditorSubsystem* LevelSubsystem = GEditor->GetEditorSubsystem<ULevelEditorSubsystem>();
					LevelSubsystem->LoadLevel(FString::Printf(TEXT("/Game/Levels/Cube/Face_%d"), i));
				})
				));
			}
		}),
		false);
}


#pragma endregion
#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FCouchGame_ToolsModule, CouchGame_Tools)