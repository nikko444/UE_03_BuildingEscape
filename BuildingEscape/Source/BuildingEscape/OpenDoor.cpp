// this software is a copyright of SGT inc. Ontario, Canada. 2019

#include "OpenDoor.h"
#include "Engine/World.h"
#include "Gameframework/Actor.h"
#include "Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"

#define OUT //Out parameter

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing a pressure plate"), *GetOwner()->GetName())
	}
}

void UOpenDoor::CloseDoor()
{
	if (Owner) {
		Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No owner is set for UOpenDoor"))
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	// If the ActorThatOpens is in the volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) {
		OnOpenRequest.Broadcast();
	} else {
		OnCloseRequest.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	
	//Find all the overlapping actors
	TArray <AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	//Iterate through them adding their masses
	for (const auto* OverlappingActor : OverlappingActors) {
		TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("found %s here"), *OverlappingActor->GetName())
	}
	return TotalMass;
}
