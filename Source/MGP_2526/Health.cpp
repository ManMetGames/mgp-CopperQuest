// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"

// Sets default values for this component's properties
UHealth::UHealth()
{
	//PrimaryComponentTick.bCanEverTick = true;
	HP = MaxHealth;
	// ...
}


// Called when the game starts
void UHealth::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UHealth::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealth::TakeDamage(int Damage)
{
	HP -= Damage;
	if (HP < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("I am hit"));
		HP = 0;
		// Tells interested classes we have died
		Died.Broadcast();
	}
}
//allows player to heal
void UHealth::Heal(int Heal)
{
	HP += Heal;
	if (HP > MaxHealth)
	{
		HP = MaxHealth;
	}
}