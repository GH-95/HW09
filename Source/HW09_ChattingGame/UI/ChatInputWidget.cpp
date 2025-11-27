#include "UI/ChatInputWidget.h"

#include "Components/EditableTextBox.h"
#include "Player/ChatPlayerController.h"

void UChatInputWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatTextCommited) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &UChatInputWidget::OnChatTextCommited);
	}
}

void UChatInputWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatTextCommited) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &UChatInputWidget::OnChatTextCommited);
	}
}

void UChatInputWidget::OnChatTextCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (APlayerController* OwningPlayerController = GetOwningPlayer())
		{
			if (AChatPlayerController* OwningChatPlayerController = Cast<AChatPlayerController>(OwningPlayerController))
			{
				OwningChatPlayerController->SetChatMessage(Text.ToString()); //Text를 String으로 형변환
				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}
