// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameAbilitySystem/StatusAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Interface/TwinResource.h"

// Sets default values
ATestCharacter::ATestCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	BarWigetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	BarWigetComponent->SetupAttachment(RootComponent);

	// 컴포넌트 생성
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// 어트리뷰트 셋 생성
	StatusAttributeSet = CreateDefaultSubobject<UStatusAttributeSet>(TEXT("Status"));
}

void ATestCharacter::TestHealthChange(float Amount)
{
	if (StatusAttributeSet)
	{
		float CurrentValue = StatusAttributeSet->GetHealth();
		StatusAttributeSet->SetHealth(CurrentValue + Amount);
	}
}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);	// 어빌리티 시스템 컴포넌트 초기화

		// 초기화 이후에만 가능
		FOnGameplayAttributeValueChange& onHealthChange =
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStatusAttributeSet::GetHealthAttribute());

		onHealthChange.AddUObject(this, &ATestCharacter::OnHealthChange);	// Health가 변경되었을 때 실행될 함수 바인딩
			
	}

	if (StatusAttributeSet)
	{
		if (BarWigetComponent && BarWigetComponent->GetWidget())
		{
			if (BarWigetComponent->GetWidget()->Implements<UTwinResource>())
			{
				ITwinResource::Execute_UpdateMaxHealth(BarWigetComponent->GetWidget(), StatusAttributeSet->GetMaxHealth());
				ITwinResource::Execute_UpdateCurrentHealth(BarWigetComponent->GetWidget(), StatusAttributeSet->GetHealth());

				ITwinResource::Execute_UpdateMaxMana(BarWigetComponent->GetWidget(), StatusAttributeSet->GetMaxMana());
				ITwinResource::Execute_UpdateCurrentMana(BarWigetComponent->GetWidget(), StatusAttributeSet->GetMana());
			}
		}
		//StatusAttributeSet->Health = 50.0f;	// 절대 안됨
		//StatusAttributeSet->SetHealth(50.0f);	// 무조건 Setter로 변경해야 한다.
	}
}

// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString healthString = FString::Printf(TEXT("%.1f / %.1f"), 
		StatusAttributeSet->GetHealth(), StatusAttributeSet->GetMaxHealth());	
	DrawDebugString(GetWorld(), GetActorLocation(), healthString, nullptr, FColor::White, 0, true);
}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATestCharacter::OnHealthChange(const FOnAttributeChangeData& InData)
{
	UE_LOG(LogTemp, Log, TEXT("On Health Change : %.1f -> %.1f"), InData.OldValue, InData.NewValue);
	ITwinResource::Execute_UpdateCurrentHealth(BarWigetComponent->GetWidget(), StatusAttributeSet->GetHealth());
}

void ATestCharacter::OnManaChange(const FOnAttributeChangeData& InData)
{
	UE_LOG(LogTemp, Log, TEXT("On Mana Change : %.1f -> %.1f"), InData.OldValue, InData.NewValue);
	ITwinResource::Execute_UpdateCurrentMana(BarWigetComponent->GetWidget(), StatusAttributeSet->GetMana());
}

