#include "Game/ChatGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Player/ChatPlayerController.h"

void AChatGameState::MulticastRPCBroadcastLoginMessage_Implementation(const FString& NameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			AChatPlayerController* ChatPC = Cast<AChatPlayerController>(PC);
			if (ChatPC)
			{
				FString PlayerName = NameString + TEXT(" Join Game.");
				ChatPC->PrintChatMessageString(PlayerName);
			}
		}
	}
}

void AChatGameState::MulticastRPCPrintChatMessageString_Implementation(const FString& InMessage)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			AChatPlayerController* ChatPC = Cast<AChatPlayerController>(PC);
			if (ChatPC)
			{
				ChatPC->PrintChatMessageString(InMessage);
			}
		}
	}
}
