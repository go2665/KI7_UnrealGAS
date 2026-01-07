// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Haste.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALGAS_API UGA_Haste : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Haste();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Haste")
	TSubclassOf<UGameplayEffect> EffectClass = nullptr;
};
