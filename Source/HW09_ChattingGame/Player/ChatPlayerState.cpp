#include "Player/ChatPlayerState.h"

#include "Net/UnrealNetwork.h"

AChatPlayerState::AChatPlayerState() : PlayerName(TEXT("None")), CurrentGuessCount(0), MaxGuessCount(3)
{
	//레플리케이션 true로 기본값 변경
	bReplicates = true;
}

void AChatPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerName);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

FString AChatPlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerName + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") +
		FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoString;
}
