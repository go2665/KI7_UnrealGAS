// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Resource.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UResource : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KI7_UNREALGAS_API IResource
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Resource")
	void UpdateCurrent(float InValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Resource")
	void UpdateMax(float InValue);
};
