// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "TestCharacter.generated.h"

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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnHealthChange(const FOnAttributeChangeData& InData);
	void OnManaChange(const FOnAttributeChangeData& InData);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	float TestValue = 10.0f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UWidgetComponent> BarWigetComponent = nullptr;
	
private:
	UPROPERTY()
	TObjectPtr<UStatusAttributeSet> StatusAttributeSet = nullptr;
};
