#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ChatGameMode.generated.h"

class AChatPlayerController;

UCLASS()
class HW09_CHATTINGGAME_API AChatGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
		
	//로그인 했을 때 호출하는 함수
	virtual void OnPostLogin(AController* NewPlayer) override;
	
public:
	//0~9까지 생성해서 3자리의 숫자 설정
	FString SecretNumber();
	
	//입력 숫자 확인
	bool IsGuessNumber(const FString& GuessNumber);
	
	//결과 값
	FString Result(const FString& InSecretNumberString, const FString& GuessNumberString);
	
	//멀티캐스트 호출 함수
	void PrintChatMessageString(const FString& MessageString, AChatPlayerController* ChatPC);

	//시도 횟수 추가 함수
	void IncreaseGuessCount(AChatPlayerController* ChatPC);
	
	void ResetGame();
	
	void ResetGameDelay();
	
	void JudgeGame(AChatPlayerController* ChatPC, int InStrikeCount);
	
protected:
	//생성된 3자리수 저장 변수
	FString SecretNumberString;
	
	//접속한 컨트롤러를 저장하는 배열
	TArray<TObjectPtr<AChatPlayerController>> AllPlayerControllers;
};
