// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameAbilitySystem/GameAbilitySystemMacros.h"
#include "StatusAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALGAS_API UStatusAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UStatusAttributeSet();

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UStatusAttributeSet, MoveSpeed)
			
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData JumpPower;
	ATTRIBUTE_ACCESSORS(UStatusAttributeSet, JumpPower)
};
