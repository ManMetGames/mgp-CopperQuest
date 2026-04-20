// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Foe.generated.h"

class UStaticMeshComponent;

UCLASS()
class MGP_2526_API AFoe : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFoe();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") 
	UStaticMeshComponent* CubeMesh;

	UPROPERTY(EditAnywhere, Category = "Damage")
	int32 DamageAmount = 10;

	UFUNCTION()
	void OnCubeOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
