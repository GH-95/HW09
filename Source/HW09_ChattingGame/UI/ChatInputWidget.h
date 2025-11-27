#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatInputWidget.generated.h"

class UEditableTextBox;

UCLASS()
class HW09_CHATTINGGAME_API UChatInputWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* EditableTextBox_ChatInput;

public:
	//OnChatTextCommited이벤트에 바인딩하는 함수
	virtual void NativeConstruct() override;
	//OnChatTextCommited이벤트를 지워주는 함수
	virtual void NativeDestruct() override;
	
protected:
	//채팅 입력을 한 후 Enter를 눌렀을 떄 실행되는 함수
	UFUNCTION()
	void OnChatTextCommited(const FText& Text, ETextCommit::Type CommitMethod);
};
