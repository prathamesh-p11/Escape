// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"	
#include "OpenDoor.h"
#include "GameFramework/Actor.h"

#define OUT
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
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle = InitialYaw + 90.f;

	//if no pressure plate set, display the error message and the actor name
	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component attched but no pressureplate property set!"), *GetOwner()->GetName());
	}

	//returns default pawn
	ActorThatOpensDoor = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//ActorThatOpensDoor set to Default pawn
	//If pressure plate property not set, it won't call pressureplate's IsOverlappingActor to avoid nullptr crash
	if(PressurePlate && TotalMassOfActors() >= MinMassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		//returns time from when world was created!!!
		DoorOpenedAt = GetWorld()->GetTimeSeconds();
	}
	else if(PressurePlate && !PressurePlate->IsOverlappingActor(ActorThatOpensDoor))
	{
		if(GetWorld()->GetTimeSeconds() - DoorOpenedAt > DoorCloseDelay)
			CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is %f"), GetOwner()->GetActorRotation().Yaw);
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator OpenDoor(0.f,CurrentYaw,0.f);
	OpenDoor.Yaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);
	GetOwner()->SetActorRotation(OpenDoor);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator CloseDoor(0.f,CurrentYaw,0.f);
	CloseDoor.Yaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	GetOwner()->SetActorRotation(CloseDoor);
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	//Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Calculate total mass
	for(AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}