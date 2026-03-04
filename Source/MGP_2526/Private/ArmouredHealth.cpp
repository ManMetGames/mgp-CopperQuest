// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmouredHealth.h"

void UArmouredHealth::TakeDamage(int Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Armoured Health took no damage! you might want to change that"));
	if (Armour > 0)
	{
		Armour -= Damage;
	}
	else
	{
		Super::TakeDamage(Damage);
	}

}