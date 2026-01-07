// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_FireDamage.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALGAS_API UGEEC_FireDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UGEEC_FireDamage();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CustomData")
	float MinimumDamage = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CustomData")
	float DamageMultiplier = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CustomData")
	TObjectPtr<UCurveTable> DamageTable = nullptr;


private:
	FGameplayTag Tag_DebuffBurn;
	FGameplayTag Tag_ElementFire;
	//FGameplayTag Tag_EffectDamage;	// SetByCaller용
};
