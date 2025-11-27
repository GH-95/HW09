#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ChatGameState.generated.h"

class AChatPlayerController;

UCLASS()
class HW09_CHATTINGGAME_API AChatGameState : public AGameState
{
	GENERATED_BODY()

public:
	//로그인 했다고 모든 클라에게 출력
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& NameString = FString(TEXT("Player")));
	
	//입력받은 채팅을 모든 클라에게 출력
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCPrintChatMessageString(const FString& InMessage);
};
