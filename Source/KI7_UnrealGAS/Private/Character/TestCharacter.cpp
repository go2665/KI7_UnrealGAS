// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameAbilitySystem/AttributeSet/ResourceAttributeSet.h"
#include "GameAbilitySystem/AttributeSet/StatusAttributeSet.h"
#include "GameAbilitySystem/GameAbilitySystemEnums.h"
#include "Components/WidgetComponent.h"
#include "Interface/TwinResource.h"
#include "EnhancedInputComponent.h"

// Sets default values
ATestCharacter::ATestCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	BarWigetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	BarWigetComponent->SetupAttachment(RootComponent);

	// 컴포넌트 생성
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));	

	// 어트리뷰트 셋 생성
	ResourceAttributeSet = CreateDefaultSubobject<UResourceAttributeSet>(TEXT("ResourceAttributeSet"));
	StatusAttributeSet = CreateDefaultSubobject<UStatusAttributeSet>(TEXT("StatusAttributeSet"));
}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);	// 어빌리티 시스템 컴포넌트 초기화

		if (HasteClass)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(
					HasteClass,									// 어빌리티 클래스
					1,											// 레벨
					static_cast<int32>(EAbilityInputID::Haste),	// 입력 ID
					this										// 소스
				)
			);
		}
		if (SuperJumpClass)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(
					SuperJumpClass,									// 어빌리티 클래스
					1,												// 레벨
					static_cast<int32>(EAbilityInputID::SuperJump),	// 입력 ID
					this											// 소스
				)
			);
		}
		if (ChargingJumpClass)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(
					ChargingJumpClass,									// 어빌리티 클래스
					1,													// 레벨
					static_cast<int32>(EAbilityInputID::ChargingJump),	// 입력 ID
					this												// 소스
				)
			);
		}

		// 초기화 이후에만 가능
		FOnGameplayAttributeValueChange& onHealthChange =
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UResourceAttributeSet::GetHealthAttribute());
		onHealthChange.AddUObject(this, &ATestCharacter::OnHealthChange);	// Health가 변경되었을 때 실행될 함수 바인딩

		FOnGameplayAttributeValueChange& onMaxHealthChange =
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UResourceAttributeSet::GetMaxHealthAttribute());
		onMaxHealthChange.AddUObject(this, &ATestCharacter::OnMaxHealthChange);

		FOnGameplayAttributeValueChange& onManaChange =
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UResourceAttributeSet::GetManaAttribute());
		onManaChange.AddUObject(this, &ATestCharacter::OnManaChange);

		FOnGameplayAttributeValueChange& onMaxManaChange =
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UResourceAttributeSet::GetMaxManaAttribute());
		onMaxManaChange.AddUObject(this, &ATestCharacter::OnMaxManaChange);
			
	}

	if (ResourceAttributeSet)
	{
		if (BarWigetComponent && BarWigetComponent->GetWidget())
		{
			if (BarWigetComponent->GetWidget()->Implements<UTwinResource>())
			{
				ITwinResource::Execute_UpdateMaxHealth(BarWigetComponent->GetWidget(), ResourceAttributeSet->GetMaxHealth());
				ITwinResource::Execute_UpdateCurrentHealth(BarWigetComponent->GetWidget(), ResourceAttributeSet->GetHealth());

				ITwinResource::Execute_UpdateMaxMana(BarWigetComponent->GetWidget(), ResourceAttributeSet->GetMaxMana());
				ITwinResource::Execute_UpdateCurrentMana(BarWigetComponent->GetWidget(), ResourceAttributeSet->GetMana());
			}
		}
		//ResourceAttributeSet->Health = 50.0f;	// 절대 안됨
		//ResourceAttributeSet->SetHealth(50.0f);	// 무조건 Setter로 변경해야 한다.
	}

	Tag_EffectDamage = FGameplayTag::RequestGameplayTag(FName("Effect.Damage"));

	if (InitializeEffectClass && AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(InitializeEffectClass, 0, EffectContext);
		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}

	
}

// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ResourceAttributeSet)
	{
		FString healthString = FString::Printf(TEXT("%.1f / %.1f"), 
			ResourceAttributeSet->GetHealth(), ResourceAttributeSet->GetMaxHealth());	
		DrawDebugString(GetWorld(), GetActorLocation(), healthString, nullptr, FColor::White, 0, true);
	}
}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInput)
	{
		EnhancedInput->BindAction(IA_Ability1, ETriggerEvent::Started, this, &ATestCharacter::OnAbility1Press);
		EnhancedInput->BindAction(IA_Ability2, ETriggerEvent::Started, this, &ATestCharacter::OnAbility2Press);
		EnhancedInput->BindAction(IA_Ability3, ETriggerEvent::Started, this, &ATestCharacter::OnAbility3Press);
		EnhancedInput->BindAction(IA_Ability3, ETriggerEvent::Completed, this, &ATestCharacter::OnAbility3Release);
	}

}

void ATestCharacter::OnHealthChange(const FOnAttributeChangeData& InData)
{
	if (ResourceAttributeSet)
	{
		//UE_LOG(LogTemp, Log, TEXT("On Health Change : %.1f -> %.1f"), InData.OldValue, InData.NewValue);
		ITwinResource::Execute_UpdateCurrentHealth(BarWigetComponent->GetWidget(), ResourceAttributeSet->GetHealth());
	}
}

void ATestCharacter::OnMaxHealthChange(const FOnAttributeChangeData& InData)
{
	if (ResourceAttributeSet)
	{
		ITwinResource::Execute_UpdateMaxHealth(BarWigetComponent->GetWidget(), ResourceAttributeSet->GetMaxHealth());
	}
}

void ATestCharacter::OnManaChange(const FOnAttributeChangeData& InData)
{
	if (ResourceAttributeSet)
	{
		//UE_LOG(LogTemp, Log, TEXT("On Mana Change : %.1f -> %.1f"), InData.OldValue, InData.NewValue);
		ITwinResource::Execute_UpdateCurrentMana(BarWigetComponent->GetWidget(), ResourceAttributeSet->GetMana());
	}
}

void ATestCharacter::OnMaxManaChange(const FOnAttributeChangeData& InData)
{
	if (ResourceAttributeSet)
	{
		ITwinResource::Execute_UpdateMaxMana(BarWigetComponent->GetWidget(), ResourceAttributeSet->GetMaxMana());
	}
}

void ATestCharacter::OnAbility1Press()
{
	UE_LOG(LogTemp, Log, TEXT("OnAbility1Press"));
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::Haste));
	}
}

void ATestCharacter::OnAbility2Press()
{
	UE_LOG(LogTemp, Log, TEXT("OnAbility2Press"));
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::SuperJump));
	}
}

void ATestCharacter::OnAbility3Press()
{
	UE_LOG(LogTemp, Log, TEXT("OnAbility3Press"));
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::ChargingJump));
	}
}

void ATestCharacter::OnAbility3Release()
{
	UE_LOG(LogTemp, Log, TEXT("OnAbility3Release"));
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(EAbilityInputID::ChargingJump));
	}
}

void ATestCharacter::TestHealthChange(float Amount)
{
	if (ResourceAttributeSet)
	{
		float CurrentValue = ResourceAttributeSet->GetHealth();
		ResourceAttributeSet->SetHealth(CurrentValue + Amount);
	}
}

void ATestCharacter::TestSetByCaller(float Amount)
{
	if (AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(TestEffectClass, 0, EffectContext);
		if (SpecHandle.IsValid())
		{
			SpecHandle.Data->SetSetByCallerMagnitude(Tag_EffectDamage, Amount);
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ATestCharacter::TestAddInfiniteEffect()
{
	if (TestInfiniteEffectClass && AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddInstigator(this, this);
		//EffectContext.AddHitResult()

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
			TestInfiniteEffectClass, 0, EffectContext);

		if (SpecHandle.IsValid())
		{
			TestInfinite = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ATestCharacter::TestRemoveInfiniteEffect()
{
	if (TestInfinite.IsValid())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(TestInfinite);
	}
}

void ATestCharacter::TestAbility()
{
	if (AbilitySystemComponent && HasteClass)
	{
		// 클래스로 어빌리티 발동시키기
		AbilitySystemComponent->TryActivateAbilityByClass(HasteClass);
	}
}

void ATestCharacter::TestLineTrace()
{
	// Line : 시작 위치, 도착 위치
	// Ray : 시작 위치, 방향

	FVector Start = GetActorLocation();		// 체크 시작 위치
	FVector End = Start + GetActorForwardVector() * 1000.0f;	// 체크 끝나는 위치

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);	// 자기 자신은 체크 안함

	FHitResult HitResult;	// 충돌 결과를 저장할 구조체
	bool bHit = GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		Start,
		End,
		ECC_Pawn,	// 폰만 체크한다고 설정
		QueryParams
	);

	if (bHit)
	{
		// 선에 걸린 폰이 있다.
		UE_LOG(LogTemp, Log, TEXT("Hit"));
		
		// 라인 트레이스하는 위치 표시
		DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Yellow, false, 0.1f, 0, 1.0f);	
		DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 0.1f);

		AActor* Target = HitResult.GetActor();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		if (TargetASC && TestHitEffectClass)
		{
			UE_LOG(LogTemp, Log, TEXT("ASC와 이팩트 클래스 확인 완료"));
			FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();	// 컨택스트 만들기
			Context.AddHitResult(HitResult);				// 라인트레이스의 히트 결과 전달
			Context.AddInstigator(GetInstigator(), this);	// 인스티게이터 지정

			FGameplayEffectSpecHandle Spec = TargetASC->MakeOutgoingSpec(	// 컨택스트 기반으로 스팩만들기
				TestHitEffectClass,
				1,
				Context
			);
			if (Spec.IsValid())
			{
				TargetASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());	// 이팩트 적용
			}
		}
	}
	else
	{
		// 선에 걸린것이 없다.
		UE_LOG(LogTemp, Log, TEXT("No Hit"));
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f, 0, 1.0f);
	}
}
