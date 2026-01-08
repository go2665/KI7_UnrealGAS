// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_ChargingJump.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALGAS_API UGA_ChargingJump : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_ChargingJump();

	// 어빌리티 발동시 실행될 함수
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	// 어빌리티를 발동시키는 입력이 끝났을 때 실행될 함수
	virtual void InputReleased(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	// 차징 시간을 업데이트 하는 함수(타이머에서 연결)
	UFUNCTION()
	void UpdateChargingTime();

	// 실제 점프 처리를 하는 함수
	void ExecuteSuperJump(const FGameplayAbilityActorInfo* ActorInfo);

protected:
	// 최소 점프력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|ChargingJump")
	float MinJumpPower = 700.0f;

	// 최대 점프력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|ChargingJump")
	float MaxJumpPower = 3000.0f;

	// 최대 차징 가능한 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|ChargingJump")
	float MaxChargingTime = 3.0f;

	// 차징 중에 적용될 이팩트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability|ChargingJump")
	TSubclassOf<UGameplayEffect> ChargingEffectClass = nullptr;

private:
	// 차징 중에 적용될 이팩트의 핸들(나중에 끄기 위해서)
	FActiveGameplayEffectHandle ChargingEffectHandle;

	// 현재 차징된 시간
	float CurrentChargingTime = 0.0f;

	// 차징 시간 업데이트용 타이머 핸들
	FTimerHandle ChargingTimerHandle;
};
