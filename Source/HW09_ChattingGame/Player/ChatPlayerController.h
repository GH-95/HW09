#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChatPlayerController.generated.h"


UCLASS()
class HW09_CHATTINGGAME_API AChatPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AChatPlayerController();
	
	virtual void BeginPlay() override;
	//입력한 채팅을 서버로 보내는 함수
	void SetChatMessage(const FString& InMessage);
	//입력한 채팅을 출력하는 함수
	void PrintChatMessageString(const FString& InMessage);

	//서버에서 실행 될 함수
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InMessage);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	//Widget을 화면에 띄우기 위한 변수들
	UPROPERTY(editAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> ChatWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> ChatWidgetInstance;

	UPROPERTY()
	UUserWidget* ChatWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextInstance;
	
public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};
