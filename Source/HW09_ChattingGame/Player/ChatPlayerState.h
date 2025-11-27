#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ChatPlayerState.generated.h"

UCLASS()
class HW09_CHATTINGGAME_API AChatPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AChatPlayerState();
	//레플리케이션 함수
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FString GetPlayerInfoString();
	
public:
	UPROPERTY(Replicated)
	FString PlayerName;
	
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;
	
	UPROPERTY(Replicated)
	int32 MaxGuessCount;
};
