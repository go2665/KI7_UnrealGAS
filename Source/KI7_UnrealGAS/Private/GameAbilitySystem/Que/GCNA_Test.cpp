// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/Que/GCNA_Test.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AGCNA_Test::AGCNA_Test()
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.TestActor"));
	bAutoDestroyOnRemove = true;	// 재사용시 파티클 재생이 끝나지 않는 문제 해결을 위해 필요
}

bool AGCNA_Test::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (SpawnedVFX.IsValid())		// 안전장치(혹시 만들어진게 있으면 제거하고 진행하라)
	{
		SpawnedVFX->Deactivate();
		SpawnedVFX = nullptr;
	}

	if (MyTarget)
	{
		//Parameters.EffectContext.GetHitResult : 히트 정보를 가져올 수 있다(=부딪친 위치나 노멀 벡터를 구할 수 있다.)
		// 중요 : GetHitResult는 Context에서 값을 설정해 줬어야 쓸 수 있다.
		SpawnedVFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(	// 파티글 만들어서 저장해 놓기
			GetWorld(),
			TestVFX,
			MyTarget->GetActorLocation(),	// 생성 위치
			MyTarget->GetActorRotation()	// 생성할 때 회전
		);

		//UNiagaraFunctionLibrary::SpawnSystemAttached()
		// 붙인채로 움직이고 싶을 때
		return true;
	}
	return false;
}

bool AGCNA_Test::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (SpawnedVFX.IsValid())		// 끝날 때 제거하기
	{
		SpawnedVFX->Deactivate();
		SpawnedVFX = nullptr;
	}
	return true;
}
