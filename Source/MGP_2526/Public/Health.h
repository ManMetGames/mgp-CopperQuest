// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FYouDied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShieldBroke);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MGP_2526_API UHealth : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealth();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Maximum health value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Stats") 
	int32 MaxHealth = 75;

	// Current health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health|Stats") 
	int32 Health = 75;

	// Maximum shield value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Shield") 
	int32 MaxShield = 100;

	// Current shield
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health|Shield")
	int32 Shield = 100;

	//Event broadcast when health reaches zero
	UPROPERTY(BlueprintAssignable, Category = "Health|Events") 
	FYouDied OnDied;

	//Event broadcast when shield breaks
	UPROPERTY(BlueprintAssignable, Category = "Health|Events") 
	FShieldBroke OnShieldBroke;
		
	//Damage is applied to shield first
	UFUNCTION(BlueprintCallable, Category = "Health") 
	void TakeDamage(int32 DamageAmount);

	//Heal health by amount (does not affect shield)
	UFUNCTION(BlueprintCallable, Category = "Health") 
	void Heal(int32 HealAmount);

	//Restore shield by amount
	UFUNCTION(BlueprintCallable, Category = "Health") 
	void RestoreShield(int32 ShieldAmount);

	//Fully restore both health and shield to their max values
	UFUNCTION(BlueprintCallable, Category = "Health") 
	void RestoreFully();

	//Set max values and optionally reset current values to the new maxes
	UFUNCTION(BlueprintCallable, Category = "Health") 
	void SetMaxValues(int32 NewMaxHealth, int32 NewMaxShield, bool bResetCurrent = false);

	//Utility Getters
	UFUNCTION(BlueprintPure, Category = "Health") 
	FORCEINLINE int32 GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure, Category = "Health") 
	FORCEINLINE int32 GetShield() const { return Shield; }

protected: 
	// Internal helper to clamp values and handle death/shield-break logic 
	void ClampAndBroadcast();
};
