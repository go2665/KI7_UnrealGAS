// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BarWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UBarWidget::SetProgressColor(FLinearColor InColor)
{
	BackgroundProgressBar->SetFillColorAndOpacity(InColor);
}

void UBarWidget::UpdateCurrent_Implementation(float InValue)
{
	UE_LOG(LogTemp, Log, TEXT("Current : %.1f"), InValue);
	Current->SetText(FText::AsNumber(FMath::RoundToInt(InValue)));
	BackgroundProgressBar->SetPercent(InValue/MaxValue);
}

void UBarWidget::UpdateMax_Implementation(float InValue)
{
	UE_LOG(LogTemp, Log, TEXT("Max : %.1f"), InValue);
	Max->SetText(FText::AsNumber(FMath::RoundToInt(InValue)));
	MaxValue = InValue;
}
