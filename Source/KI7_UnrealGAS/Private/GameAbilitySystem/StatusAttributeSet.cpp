// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/StatusAttributeSet.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UStatusAttributeSet::UStatusAttributeSet()
{
	InitMoveSpeed(500.0f);
	InitJumpPower(700.0f);
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
