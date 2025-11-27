#pragma once

#include "CoreMinimal.h"

class ChatFunctionLibrary
{
public:
	static void MyPrintString(const AActor* WorldContextActor, const FString& String, float TimeToDisplay = 1.f,
							  FColor Color = FColor::Cyan)
	{
		if (GEngine && WorldContextActor)
		{
			if (WorldContextActor->GetNetMode() == NM_Client || WorldContextActor->GetNetMode() == NM_ListenServer)
			{
				GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Color, String);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), *String);
			}
		}
	}
};
