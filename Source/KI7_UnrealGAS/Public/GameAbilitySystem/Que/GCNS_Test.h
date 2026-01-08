// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GCNS_Test.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALGAS_API UGCNS_Test : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UGCNS_Test();
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<class UNiagaraSystem> TestVFX = nullptr;
};
