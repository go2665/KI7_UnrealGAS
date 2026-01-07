// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/Effect/UGEEC_LightningDamage.h"
#include "GameAbilitySystem/AttributeSet/ResourceAttributeSet.h"
#include "GameAbilitySystem/AttributeSet/StatusAttributeSet.h"

// UUGEEC_LightningDamage 계산에 필요한 어트리뷰트를 캡처하기 위한 구조체(이 계산이 어디에 영향을 줄것인가)
struct FLightningDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);		// Health 어트리뷰트를 캡처할 것이라고 정의
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalRate);

	FLightningDamageStatics()
	{
		// UResourceAttributeSet의 Health를 캡쳐하는데, Target으로 부터 캡쳐, 스냅샷은 사용하지 않음
		DEFINE_ATTRIBUTE_CAPTUREDEF(UResourceAttributeSet, Health, Target, false);

		// UStatusAttributeSet의 AttackPower를 캡쳐, 공격자로 부터 캡쳐, 공격 시점의 값을 가져오기
		DEFINE_ATTRIBUTE_CAPTUREDEF(UStatusAttributeSet, AttackPower, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UStatusAttributeSet, CriticalRate, Source, true);
	}
};

// FLightningDamageStatics을 싱글톤 인스턴스를 반환하는 함수
static FLightningDamageStatics& LightningDamageStatics()
{
	static FLightningDamageStatics Statics;
	return Statics;
}

UUGEEC_LightningDamage::UUGEEC_LightningDamage()
{
	RelevantAttributesToCapture.Add(LightningDamageStatics().HealthDef);	// 캡쳐할 어트리뷰트 목록에 추가
	RelevantAttributesToCapture.Add(LightningDamageStatics().AttackPowerDef);
	RelevantAttributesToCapture.Add(LightningDamageStatics().CriticalRateDef);

	Tag_DebuffShock = FGameplayTag::RequestGameplayTag(FName("State.Debuff.Shock"));	// 태그 미리 저장해 놓기
	Tag_ElementLightning = FGameplayTag::RequestGameplayTag(FName("Element.Lightning"));
}

void UUGEEC_LightningDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// 이팩트를 주고 받는 ASC를 찾아 놓기
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	if (TargetASC && SourceASC)
	{
		const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

		const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();	// 소스의 태그 가져오기
		const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();	// 타겟의 태그 가져오기

		float Damage = MinimumDamage;
		float Multiplier = 1.0f;

		// 커브테이블에서 값 가져오기
		if (DataCurveTable)
		{
			float EffectLevel = Spec.GetLevel();	// 레벨 가져오기
			// 커브 테이블에서 커브 가져오기
			FRealCurve* DamageCurve = DataCurveTable->FindCurve(FName("Damage"), TEXT("UUGEEC_LightningDamage"));
			if (DamageCurve)
			{
				Damage = DamageCurve->Eval(EffectLevel);	// 커브에서 레벨에 해당하는 값 가져오기
			}
			FRealCurve* MultiplierCurve = DataCurveTable->FindCurve(FName("Multiplier"), TEXT("UUGEEC_LightningDamage"));
			if (MultiplierCurve)
			{
				Multiplier = MultiplierCurve->Eval(EffectLevel);
			}
		}

		FAggregatorEvaluateParameters EvaluateParameters;
		EvaluateParameters.SourceTags = SourceTags;
		EvaluateParameters.TargetTags = TargetTags;
		float AttackPower = 0.0f;
		bool Result = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(	// AttackPower 어트리뷰트 값 가져오기
			LightningDamageStatics().AttackPowerDef,
			EvaluateParameters,	// AttackPower에 영향을 끼치는 이팩트 중에 EvaluateParameters에 포함된 태그가 있는 것만 적용한 값을 준다.
			AttackPower);
		//UE_LOG(LogTemp, Log, TEXT("AttackPower Result : %s"), Result ? TEXT("성공") : TEXT("실패"));
		if (Result)
		{
			// 정상적으로 값을 가지고 왔다.
			Damage += AttackPower;
		}
		else
		{
			// 값을 가져오지 못했다.
		}

		//UE_LOG(LogTemp, Log, TEXT("Current Damage : %.1f"), Damage);
		float CriticalRate = 0.0f;
		Result = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			LightningDamageStatics().CriticalRateDef,
			EvaluateParameters,	
			CriticalRate);
		//UE_LOG(LogTemp, Log, TEXT("CriticalRate Result : %s"), Result ? TEXT("성공") : TEXT("실패"));
		//UE_LOG(LogTemp, Log, TEXT("Current CriticalRate : %.1f"), CriticalRate);
		if (Result)
		{
			// 정상적으로 값을 가지고 왔다.(크리티컬 체크 시도)
			if (FMath::FRand() <= CriticalRate)
			{
				Damage *= 2.0f;
				UE_LOG(LogTemp, Log, TEXT("Current Damage : %.1f - Critical"), Damage);
			}
		}
		else
		{
			// 값을 가져오지 못했다.(못받아왔으면 0이니까 무조건 실패)
		}

		if (SourceTags && SourceTags->HasTag(Tag_ElementLightning)		// 공격 데미지가 번개 속성이고
			&& TargetTags && TargetTags->HasTag(Tag_DebuffShock))		// 피격자가 감전 디버프를 가지고 있으면
		{
			Damage *= Multiplier;	// 그러면 데미지 두배
		}

		if (Damage > 0.0f)
		{
			OutExecutionOutput.AddOutputModifier(
				FGameplayModifierEvaluatedData(
					LightningDamageStatics().HealthProperty,	// Health 어트리뷰트를 변경한다.
					EGameplayModOp::Additive,			// 더해 줄거다.
					-Damage								// -Damage만큼 처리
				)
			);
		}
	}
}
