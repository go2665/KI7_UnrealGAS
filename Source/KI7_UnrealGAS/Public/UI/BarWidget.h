// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/Resource.h"
#include "BarWidget.generated.h"

/**
 * 
 */

class UProgressBar;
class UTextBlock;

UCLASS()
class KI7_UNREALGAS_API UBarWidget : public UUserWidget, public IResource
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetProgressColor(FLinearColor InColor);

	virtual void UpdateCurrent_Implementation(float InValue) override;
	virtual void UpdateMax_Implementation(float InValue) override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> BackgroundProgressBar = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Current = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Max = nullptr;

private:
	float MaxValue = 1.0f;
};
