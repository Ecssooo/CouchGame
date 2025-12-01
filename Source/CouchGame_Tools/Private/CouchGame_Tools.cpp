#include "CouchGame_Tools.h"

#include "FToolsEditorStyle.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Systems/CharacterSettings.h"

#define LOCTEXT_NAMESPACE "FCouchGame_ToolsModule"

void FCouchGame_ToolsModule::StartupModule()
{
	FToolsEditorStyle::Initialize();
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCouchGame_ToolsModule::CreateUseKeyboardToggle));
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCouchGame_ToolsModule::CreateRecompileButton));
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
			Blueprint = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), nullptr, *Asset.ObjectPath.ToString()));
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
#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FCouchGame_ToolsModule, CouchGame_Tools)