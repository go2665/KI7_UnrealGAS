// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/Que/GCNS_Test.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

UGCNS_Test::UGCNS_Test()
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Test"));
}

bool UGCNS_Test::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	if (MyTarget)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			TestVFX,
			MyTarget->GetActorLocation(),
			MyTarget->GetActorRotation()
		);
		return true;
	}
	return false;
}
