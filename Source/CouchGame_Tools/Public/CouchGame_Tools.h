#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FCouchGame_ToolsModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
 
    void RegisterMenuExtensions();
};
IMPLEMENT_MODULE(FCouchGame_ToolsModule, MyEditorModule)
