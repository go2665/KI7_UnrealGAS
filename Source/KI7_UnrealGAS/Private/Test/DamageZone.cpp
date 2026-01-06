// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/DamageZone.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
ADamageZone::ADamageZone()
{
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADamageZone::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ADamageZone::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ADamageZone::OnEndOverlap);
}

void ADamageZone::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (ASC)
	{
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddInstigator(this, this);

		// Spec 생성
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, ContextHandle);

		if (SpecHandle.IsValid())
		{
			// 이펙트 적용 및 핸들 반환
			FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

			// 나중에 제거하기 위해 맵에 저장
			ActiveEffectHandles.Add(ASC, ActiveGEHandle);

			UE_LOG(LogTemp, Log, TEXT("Entered Damage Zone: %s"), *OtherActor->GetName());
		}
	}
	
}

void ADamageZone::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

	if (TargetASC)
	{
		// 이 ASC가 우리 맵에 있는지 확인 (우리가 건 이펙트가 있는지)
		if (ActiveEffectHandles.Contains(TargetASC))
		{
			// 저장해둔 핸들을 찾아서
			FActiveGameplayEffectHandle HandleToRemove = ActiveEffectHandles[TargetASC];

			// 이펙트 제거
			TargetASC->RemoveActiveGameplayEffect(HandleToRemove);

			// 맵에서 삭제
			ActiveEffectHandles.Remove(TargetASC);

			UE_LOG(LogTemp, Log, TEXT("Exited Damage Zone: %s"), *OtherActor->GetName());
		}
	}
}

