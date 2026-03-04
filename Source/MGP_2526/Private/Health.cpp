// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UHealth::UHealth()
{
	//PrimaryComponentTick.bCanEverTick = true;
	Health = MaxHealth; 
	Shield = MaxShield;
}


// Called when the game starts
void UHealth::BeginPlay()
{
	Super::BeginPlay();
	
	// In case Max values were changed in editor after construction, ensure current values are valid
	Health = FMath::Clamp(Health, 0, MaxHealth); 
	Shield = FMath::Clamp(Shield, 0, MaxShield);
}

void UHealth::TakeDamage(int32 DamageAmount)
{
	if (DamageAmount <= 0) 
	{ 
		return; 
	}
	// Track whether shield was >0 before applying damage to detect shield break
	const bool bShieldWasPositive = (Shield > 0);

	ClampAndBroadcast();
}

//allows player to heal
void UHealth::Heal(int Heal)
{
	if (HealAmount <= 0) 
	{ 
		return; 
	}

	Health += HealAmount; 
	Health = FMath::Clamp(Health, 0, MaxHealth);
}

void UHealth::RestoreShield(int32 ShieldAmount) 
{
	if (ShieldAmount <= 0) 
	{ 
		return; 
	}

	Shield += ShieldAmount; 
	Shield = FMath::Clamp(Shield, 0, MaxShield);
}

void UHealth::RestoreFully()
{
	Health = MaxHealth; 
	Shield = MaxShield;
}

void UHealth::SetMaxValues(int32 NewMaxHealth, int32 NewMaxShield, bool bResetCurrent)
{
	MaxHealth = FMath::Max(1, NewMaxHealth); 
	MaxShield = FMath::Max(0, NewMaxShield);

	if (bResetCurrent) 
	{ 
		Health = MaxHealth; 
		Shield = MaxShield; 
	}
	else
	{
		Health = FMath::Clamp(Health, 0, MaxHealth); 
		Shield = FMath::Clamp(Shield, 0, MaxShield);
	}
}

void UHealth::ClampAndBroadcast()
{
	// Clamp values
	const int32 PrevShield = Shield;
	Health = FMath::Clamp(Health, 0, MaxHealth);
	Shield = FMath::Clamp(Shield, 0, MaxShield);

	// Shield broke: previously >0 and now == 0 
	if (PrevShield > 0 && Shield == 0)
	{
		OnShieldBroke.Broadcast();
		UE_LOG(LogTemp, Log, TEXT("%s: Shield broke"), *GetOwner()->GetName());
	}

	// Died: health reached zero
	if (Health == 0)
	{
		OnDied.Broadcast();
		UE_LOG(LogTemp, Log, TEXT("%s: Died"), *GetOwner()->GetName());
	}


}

