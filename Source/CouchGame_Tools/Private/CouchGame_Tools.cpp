#include "CouchGame_Tools.h"

#define LOCTEXT_NAMESPACE "FCouchGame_ToolsModule"

void FCouchGame_ToolsModule::StartupModule()
{
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCouchGame_ToolsModule::RegisterMenuExtensions));
}

void FCouchGame_ToolsModule::ShutdownModule()
{
	// Unregister the startup function
	UToolMenus::UnRegisterStartupCallback(this);
 
	// Unregister all our menu extensions
	UToolMenus::UnregisterOwner(this);
}

void FCouchGame_ToolsModule::RegisterMenuExtensions()
{
}



#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FCouchGame_ToolsModule, CouchGame_Tools)