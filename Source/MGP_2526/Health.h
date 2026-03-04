// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FYouDied);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MGP_2526_API UHealth : public UActorComponent
{
	GENERATED_BODY()

private:
	// Makes variables private so only health script deals with health values
	UPROPERTY(VisibleAnywhere, Category = "Health|MaxHealth")
	int MaxHealth = 75;

	UPROPERTY(VisibleAnywhere, Category = "Health|HP")
	int HP;

public:	
	// Sets default values for this component's properties
	UHealth();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)

	// function to take damage
	virtual void TakeDamage(int damage);
	UFUNCTION(BlueprintCallable)

	virtual void Heal(int Heal);
	UPROPERTY(BlueprintAssignable, Category = "Health")

	FYouDied Died;
		
};
