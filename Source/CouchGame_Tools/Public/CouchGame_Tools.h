#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FCouchGame_ToolsModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
#pragma region Use Keyboard 
    void CreateUseKeyboardToggle();

    void OnToggleKeyboardController();
    bool GetUseKeyboardController();
#pragma endregion

#pragma region Recompile blueprint

    void CreateRecompileButton();

    void RecompileAllBlueprint();


    
#pragma endregion

#pragma region Load level

    void CreateMainLevelButton();
    void LoadLevelButton();
    void CreateLoadLevelButton();
    
#pragma endregion
};
IMPLEMENT_MODULE(FCouchGame_ToolsModule, MyEditorModule)
