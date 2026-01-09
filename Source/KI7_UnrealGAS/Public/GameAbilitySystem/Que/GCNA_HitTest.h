// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "GCNA_HitTest.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALGAS_API AGCNA_HitTest : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AGCNA_HitTest();
	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<class UNiagaraSystem> TestVFX = nullptr;

private:
	UPROPERTY()
	TWeakObjectPtr<class UNiagaraComponent> SpawnedVFX = nullptr;
	
};
