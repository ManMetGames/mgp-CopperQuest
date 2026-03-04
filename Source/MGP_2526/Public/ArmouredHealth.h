// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Health.h"
#include "ArmouredHealth.generated.h"
//this is the armoured health
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArmourBroke);
/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MGP_2526_API UArmouredHealth : public UHealth
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Health|HP")

	int Armour;
	// this is an override to the health script
	void TakeDamage(int Damage) override;
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FArmourBroke Broke;
};
