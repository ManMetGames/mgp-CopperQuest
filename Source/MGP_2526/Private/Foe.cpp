// Fill out your copyright notice in the Description page of Project Settings.


#include "Foe.h"
#include "Components/StaticMeshComponent.h" 
#include "Health.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFoe::AFoe()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh")); 
	RootComponent = CubeMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));

	if (CubeAsset.Succeeded())
	{
		CubeMesh->SetStaticMesh(CubeAsset.Object);
	}

	AutoPossessPlayer = EAutoReceiveInput::Disabled;

	CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CubeMesh->SetCollisionObjectType(ECC_WorldDynamic);
	CubeMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	CubeMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CubeMesh->SetGenerateOverlapEvents(true);

	CubeMesh->OnComponentBeginOverlap.AddDynamic(this, &AFoe::OnCubeOverlap);
}
void AFoe::OnCubeOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	if (UHealth* Health = OtherActor->FindComponentByClass<UHealth>())
	{
		Health->TakeDamage(DamageAmount);
		return;
	}

	UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, nullptr, this, nullptr);
}