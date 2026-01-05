// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/StatusAttributeSet.h"
#include "GameplayEffectExtension.h"

UStatusAttributeSet::UStatusAttributeSet()
{
	InitMaxHealth(100.0f);
	InitHealth(100.0f);
	InitMaxMana(100.0f);
	InitMana(100.0f);
}

void UStatusAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// 값 검증 및 제한(Clamp) 용도. 순수한 수학적 처리 용도.

	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())	// 이 함수가 Health 어트리뷰트가 변경되어서 호출되었는지 확인
	{
		// 최대 체력을 넘지 않게 하기
		// 체력이 음수가 되지 않게 하기
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
		//UE_LOG(LogTemp, Log, TEXT("Health가 변경되었다 (%.1f -> %.1f)"), GetHealth(), NewValue);
	}
}

void UStatusAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// 게임 로직 실행, 이벤트 발생, 다른 시스템과의 상호작용 용도
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())	// Health를 변경시키는 이팩트가 실행된 후에 호출되었는지 확인
	{
		UE_LOG(LogTemp, Log, TEXT("현재 Health : %.1f"), GetHealth());
		// 채력 변화 로직 호출

		if (GetHealth() <= 0.0f)
		{
			UE_LOG(LogTemp, Log, TEXT("사망"));
			// 캐릭터 사망처리 로직 호출
		}
	}
}
