#pragma once

class FToolsEditorStyle
{
public:
	static void Initialize();
	static void Shutdown();

	static TSharedPtr<FSlateStyleSet> Get();
	static FName GetStyleSetName();

private:
	static TSharedPtr<FSlateStyleSet> StyleInstance;
};
