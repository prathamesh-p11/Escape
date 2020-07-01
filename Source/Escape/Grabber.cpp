// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"	//to draw the reach vector
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

//macro to mark out parameters, OUT does nothing, its only function is to mark out parameters for readability
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting on duty!"));
	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);	

	//Draw a line from player to show reach
	FVector LineTraceEndRotation = PlayerViewPointRotation.Vector();
	FVector LineTraceEnd = PlayerViewPointLocation + (LineTraceEndRotation * Reach);
	DrawDebugLine(
		GetWorld(),		//world
		PlayerViewPointLocation,		//starting point
		LineTraceEnd,					//end point
		FColor(0, 255, 0),				//color of line
		false,							//does line persists? false - we want to update the line every tick
		0.f,							//lifetime - no persistence - no life - 0
		0,								//Depthpriority
		5								//line thickness
	);

	//Reach out from player(RayCasting) to a certain distance
	FHitResult ObjectHit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT ObjectHit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	//Loggin out hit objects as actors
	AActor* ActorHit = ObjectHit.GetActor();
	if(ActorHit)
		UE_LOG(LogTemp, Warning, TEXT("Line trace has hit %s"), *(ActorHit->GetName()));
	//What objects rays are hitting and are they grabable 
}