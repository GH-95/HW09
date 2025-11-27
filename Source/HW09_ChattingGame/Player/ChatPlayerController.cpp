#include "Player/ChatPlayerController.h"

#include "ChatPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "HW09_ChattingGame.h"
#include "Game/ChatGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AChatPlayerController::AChatPlayerController()
{
	bReplicates = true;
}

void AChatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsLocalController())
	{
		if (ChatWidgetClass)
		{
			ChatWidgetInstance = CreateWidget<UUserWidget>(this, ChatWidgetClass);
			if (ChatWidgetInstance)
			{
				ChatWidgetInstance->AddToViewport();
			}
		}
		if (NotificationTextClass)
		{
			NotificationTextInstance = CreateWidget<UUserWidget>(this, NotificationTextClass);
			if (NotificationTextInstance)
			{
				NotificationTextInstance->AddToViewport();
			}
		}
	}
}

void AChatPlayerController::SetChatMessage(const FString& InMessage)
{
	if (IsLocalController())
	{
		if (AChatPlayerState* ChatPS = GetPlayerState<AChatPlayerState>())
		{
			//레플리케이션으로 복제된 클라이언트가 채팅시에 서버로 보냄
			FString CombinedMessage = ChatPS->GetPlayerInfoString() + TEXT(": ") + InMessage;
			ServerRPCPrintChatMessageString(CombinedMessage);
		}
	}
}

void AChatPlayerController::PrintChatMessageString(const FString& InMessage)
{
	ChatFunctionLibrary::MyPrintString(this, InMessage, 10.f);
}

void AChatPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, NotificationText);
}

void AChatPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InMessage)
{
	if (AGameModeBase* GM = UGameplayStatics::GetGameMode(this))
	{
		if (AChatGameMode* ChatGM = Cast<AChatGameMode>(GM))
		{
			ChatGM->PrintChatMessageString(InMessage, this);
		}
	}
}
