// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "DamageZone.generated.h"

UCLASS()
class KI7_UNREALGAS_API ADamageZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamageZone();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DamageEffectClass = nullptr;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

private:
	// ASC를 키값으로 한 적용된 이팩트 핸들 모음
	TMap<class UAbilitySystemComponent*, FActiveGameplayEffectHandle> ActiveEffectHandles;
};
