#include "Game/ChatGameMode.h"

#include "ChatGameState.h"
#include "Player/ChatPlayerController.h"
#include "Player/ChatPlayerState.h"

void AChatGameMode::BeginPlay()
{
	Super::BeginPlay();

	//생성된 3자리 숫자 저장
	SecretNumberString = SecretNumber();
	UE_LOG(LogTemp, Error, TEXT("%s"), *SecretNumberString)
}

void AChatGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (AChatPlayerController* ChatPC = Cast<AChatPlayerController>(NewPlayer))
	{
		//게임 접속 시 출력
		ChatPC->NotificationText = FText::FromString(TEXT("In Game"));

		//배열 안에 게임 접속하는 순서대로 추가
		AllPlayerControllers.Add(ChatPC);

		if (AChatPlayerState* ChatPS = ChatPC->GetPlayerState<AChatPlayerState>())
		{
			if (AChatGameState* ChatGS = GetGameState<AChatGameState>())
			{
				//레플리케이션으로 복제된 클라이언트 번호가 입장시에 출력
				ChatPS->PlayerName = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
				ChatGS->MulticastRPCBroadcastLoginMessage(ChatPS->PlayerName);
			}
		}
	}
}

FString AChatGameMode::SecretNumber()
{
	//1~9까지 중복없이 배열에 숫자 생성
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}
	//난수 생성기를 틱으로 초기화 해서 매번 다른 결과를 추출
	FMath::RandInit(FDateTime::Now().GetTicks());
	//Numbers의 숫자는 변하지 않음
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString SecretResult;
	// 3번 반복해서 3자리의 숫자 생성
	for (int32 i = 0; i < 3; ++i)
	{
		//숫자를 선택
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		//선택된 숫자를 문자열에 추가
		SecretResult.Append(FString::FromInt(Numbers[Index]));
		//중복되지 않게 선택된 숫자를 제거
		Numbers.RemoveAt(Index);
	}
	return SecretResult; // 3자리의 숫자 반환
}

bool AChatGameMode::IsGuessNumber(const FString& GuessNumber)
{
	//입력한 수가 3자리 숫자가 맞는지 확인
	bool bCanPlay = false; //기본 설정은 false

	do
	{
		//숫자의 길이가 3자리가 아니면 유효하지 않음
		if (GuessNumber.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true; //0이거나 중복이거나 숫자가 아닌경우를 확인하기 위한 bool값
		TSet<TCHAR> UniqueDigits; //중복확인을 위해 숫자를 저장
		//숫자의 각 문자 확인
		for (TCHAR c : GuessNumber)
		{
			//숫자가 아니거나 0을 포함하면 유효하지 않음
			if (FChar::IsDigit(c) == false || c == '0')
			{
				bIsUnique = false;
				break;
			}
			UniqueDigits.Add(c); //유효하다면 추가
		}
		//추가된 숫자가 3개가 아니면 유효하지 않음
		if (UniqueDigits.Num() != 3)
		{
			bIsUnique = false;
		}
		//0이나 숫자가 아니거나 중복된 경우 유효하지 않음
		if (bIsUnique == false)
		{
			break;
		}
		//3가지의 경우를 모두 통과했다면 유효
		bCanPlay = true;
	}
	while (false); //한번만 실행함
	return bCanPlay;
}

FString AChatGameMode::Result(const FString& InSecretNumberString, const FString& GuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	//숫자 확인
	for (int32 i = 0; i < 3; ++i)
	{
		//위치와 숫자가 같으면 Strike
		if (InSecretNumberString[i] == GuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			//숫자는 같지만 위치가 다르면 Ball
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), GuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}
	//둘다아니면 아웃
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}
	//아웃이 아니면 출력
	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void AChatGameMode::PrintChatMessageString(const FString& MessageString, AChatPlayerController* ChatPC)
{
	//메세지에서 뒷부분 3글자 확인
	int Index = MessageString.Len() - 3;
	FString GuessNumber = MessageString.RightChop(Index);
	//숫자 게임 유효성 확인
	if (IsGuessNumber(GuessNumber) == true)
	{
		IncreaseGuessCount(ChatPC);
		//숨겨진 숫자와 입력 숫자 결과 계산
		FString ResultString = Result(SecretNumberString, GuessNumber);
		//원본 메세지와 결과를 합쳐서 출력
		if (AGameStateBase* GameStateBase = GetWorld()->GetGameState())
		{
			if (AChatGameState* ChatGameState = Cast<AChatGameState>(GameStateBase))
			{
				if (HasAuthority())
				{
					if (AChatPlayerState* ChatPS = ChatPC->GetPlayerState<AChatPlayerState>())
					{
						//스트라이크 결과 값을 가져와서 JudgeGame에 판정을 확인 후 출력
						int32 StrikeCount = FCString::Atoi(*ResultString.Left(1));
						JudgeGame(ChatPC, StrikeCount);

						FString CombinedMessageString = ChatPS->GetPlayerInfoString() + GuessNumber + TEXT(" -> ") +
							ResultString;
						ChatGameState->MulticastRPCPrintChatMessageString(CombinedMessageString);
					}
				}
			}
		}
	}
	else
	{
		//유효하지 않다면 원본 메세지만 출력
		if (AGameStateBase* GameStateBase = GetWorld()->GetGameState())
		{
			if (AChatGameState* ChatGameState = Cast<AChatGameState>(GameStateBase))
			{
				if (HasAuthority())
				{
					ChatGameState->MulticastRPCPrintChatMessageString(MessageString);
				}
			}
		}
	}
}

void AChatGameMode::IncreaseGuessCount(AChatPlayerController* ChatPC)
{
	if (AChatPlayerState* ChatPS = ChatPC->GetPlayerState<AChatPlayerState>())
	{
		ChatPS->CurrentGuessCount++;
	}
}

void AChatGameMode::ResetGame()
{
	//새로운 숫자 생성
	SecretNumberString = SecretNumber();

	//모든 플레이어 상태 초기화
	for (const auto& ChatPC : AllPlayerControllers)
	{
		if (AChatPlayerState* ChatPS = ChatPC->GetPlayerState<AChatPlayerState>())
		{
			ChatPS->CurrentGuessCount = 0;
			FString Restart = TEXT("Restart");
			ChatPC->NotificationText = FText::FromString(Restart);
		}
	}
}

void AChatGameMode::ResetGameDelay()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	FTimerHandle TimerHandle_ResetGame;
	const float DelayTime = 3.0f;
	TimerManager.SetTimer(TimerHandle_ResetGame, this, &AChatGameMode::ResetGame, DelayTime, false);
}

void AChatGameMode::JudgeGame(AChatPlayerController* ChatPC, int InStrikeCount)
{
	//스트라이크가 3이 나왔을 때 Player""Winner가 뜨고 게임 리셋
	if (InStrikeCount == 3)
	{
		AChatPlayerState* ChatPS = ChatPC->GetPlayerState<AChatPlayerState>();
		for (const auto& ChatPlayerController : AllPlayerControllers)
		{
			if (ChatPC)
			{
				FString CombinedMessageString = ChatPS->PlayerName + TEXT(" Winner");
				ChatPlayerController->NotificationText = FText::FromString(CombinedMessageString);
			}
		}
		ResetGameDelay();
	}
	else
	{
		//아니라면 게임 진행, 모든 플레이어가 카운트가 3이 넘지 않으면 진행
		bool bIsDraw = true;
		for (const auto& ChatPlayerController : AllPlayerControllers)
		{
			if (AChatPlayerState* ChatPS = ChatPlayerController->GetPlayerState<AChatPlayerState>())
			{
				if (ChatPS->CurrentGuessCount <= ChatPS->MaxGuessCount)
				{
					bIsDraw = false;
				}
				else
				{
					FString FailMessageString = ChatPS->PlayerName + TEXT("Fail");
					ChatPlayerController->NotificationText = FText::FromString(FailMessageString);
					//패배로직
				}
			}
		}
		if (bIsDraw == true)
		{
			for (const auto& ChatPlayerController : AllPlayerControllers)
			{
				ChatPlayerController->NotificationText = FText::FromString(TEXT("Game Over"));
			}
			ResetGameDelay();
		}
	}
}
