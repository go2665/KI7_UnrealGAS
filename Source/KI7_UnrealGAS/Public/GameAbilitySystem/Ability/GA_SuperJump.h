// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_SuperJump.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALGAS_API UGA_SuperJump : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_SuperJump();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|SuperJump")
	float JumpPower = 1000.0f;
};
