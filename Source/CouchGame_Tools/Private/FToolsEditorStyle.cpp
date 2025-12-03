#include "FToolsEditorStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Misc/Paths.h"
#include "SlateOptMacros.h"

#define IMAGE_BRUSH(RelativePath, Size) FSlateImageBrush(StyleInstance->RootToContentDir(RelativePath, TEXT(".png")), Size)

TSharedPtr<FSlateStyleSet> FToolsEditorStyle::StyleInstance = nullptr;


FName FToolsEditorStyle::GetStyleSetName()
{
	return TEXT("FToolsEditorStyle");	
}

void FToolsEditorStyle::Initialize()
{
	if (StyleInstance.IsValid()) return;

	StyleInstance = MakeShared<FSlateStyleSet>(GetStyleSetName());

	FString ModulePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Source/CouchGame_Tools/Ressources"));
	StyleInstance->SetContentRoot(ModulePath);

	StyleInstance->Set("GameTools.KeyboardIcon", new IMAGE_BRUSH(TEXT("clavier"), FVector2D(20,20)));
	StyleInstance->Set("GameTools.RecompileIcon", new IMAGE_BRUSH(TEXT("tools"), FVector2D(20,20)));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance.Get());
}

void FToolsEditorStyle::Shutdown()
{
	if (StyleInstance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance.Get());
		ensure(StyleInstance.IsUnique());
		StyleInstance.Reset();
	}
}

TSharedPtr<FSlateStyleSet> FToolsEditorStyle::Get()
{
	return StyleInstance;
}
	