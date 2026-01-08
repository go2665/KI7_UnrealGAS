// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/Ability/GA_SuperJump.h"
#include "GameFramework/Character.h"

UGA_SuperJump::UGA_SuperJump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_SuperJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);	// 쿨타임이 안되었거나 코스트가 부족하다.
		return;
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);	// 어빌리티를 사용하는 것이 캐릭터가 아니면 실패
		return;
	}

	FVector Direction(1, 0, 1);
	Direction.Normalize();	// 앞쪽 위 45도 방향	
	Direction = Character->GetActorRotation().RotateVector(Direction);
	Character->LaunchCharacter(Direction * JumpPower, false, false);// 캐릭터 날리기

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);		// 어빌리티 종료
}
