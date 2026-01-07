// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/AttributeSet/StatusAttributeSet.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UStatusAttributeSet::UStatusAttributeSet()
{
	InitAttackPower(10.0f);
	InitCriticalRate(0.2f);
	InitMoveSpeed(500.0f);
	InitJumpPower(700.0f);
}

void UStatusAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetCriticalRateAttribute())
	{
		// 0 ~ 1
		NewValue = FMath::Clamp(NewValue, 0.0f, 1.0f);
	}
}

void UStatusAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMoveSpeedAttribute())
	{
		AActor* OwningActor = GetOwningActor(); // 이 어트리뷰트를 가지고 있는 액터
		ACharacter* OwningChar = Cast<ACharacter>(OwningActor);
		if (OwningChar)
		{
			OwningChar->GetCharacterMovement()->MaxWalkSpeed = NewValue;
		}
	}

	if (Attribute == GetJumpPowerAttribute())
	{
		AActor* OwningActor = GetOwningActor(); // 이 어트리뷰트를 가지고 있는 액터
		ACharacter* OwningChar = Cast<ACharacter>(OwningActor);
		if (OwningChar)
		{
			OwningChar->GetCharacterMovement()->JumpZVelocity = NewValue;
		}
	}
}
