// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "TestCharacter.generated.h"

class UResourceAttributeSet;
class UStatusAttributeSet;
class UWidgetComponent;

UCLASS()
class KI7_UNREALGAS_API ATestCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {
		return AbilitySystemComponent;
	};

	UFUNCTION(BlueprintCallable)
	void TestHealthChange(float Amount);

	UFUNCTION(BlueprintCallable)
	void TestSetByCaller(float Amount);

	UFUNCTION(BlueprintCallable)
	void TestAddInfiniteEffect();

	UFUNCTION(BlueprintCallable)
	void TestRemoveInfiniteEffect();

	UFUNCTION(BlueprintCallable)
	void TestAbility();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnHealthChange(const FOnAttributeChangeData& InData);
	void OnMaxHealthChange(const FOnAttributeChangeData& InData);
	void OnManaChange(const FOnAttributeChangeData& InData);
	void OnMaxManaChange(const FOnAttributeChangeData& InData);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	float TestValue = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	TSubclassOf<class UGameplayEffect> TestEffectClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	TSubclassOf<class UGameplayEffect> TestInfiniteEffectClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialize")
	TSubclassOf<class UGameplayEffect> InitializeEffectClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UGameplayAbility> HasteClass = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UWidgetComponent> BarWigetComponent = nullptr;
	
private:
	UPROPERTY()
	TObjectPtr<UResourceAttributeSet> ResourceAttributeSet = nullptr;

	UPROPERTY()
	TObjectPtr<UStatusAttributeSet> StatusAttributeSet = nullptr;

	FGameplayTag Tag_EffectDamage;

	FActiveGameplayEffectHandle TestInfinite;
};
