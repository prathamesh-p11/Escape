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
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

//Checking for physics handle component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle conponent found on %s"), *GetOwner()->GetName());
	}
}

//Input Component Setup and binding
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>(); 
	if(InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Drop);
	}
}

void UGrabber::Grab()
{
	//TODO : Clean code by either passing LineTraceEnd or by making it a class variable
	//Just for Line trace end
	//---------------Unnecessary redundant copy-----------------------------------------------
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);	

	FVector LineTraceEndRotation = PlayerViewPointRotation.Vector();
	FVector LineTraceEnd = PlayerViewPointLocation + (LineTraceEndRotation * Reach);
	//---------------Unnecessary redundant copy-----------------------------------------------
	

		UE_LOG(LogTemp, Warning, TEXT("***GRAB***"));
		FHitResult HitResult = GetFirstObjectInReach();
		UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
		//GrabComponentAtLocation(Component, BoneName, GrabLocation)
		//GrabLocation = Where to grab the component = set to object's origin
		if(HitResult.GetActor())
		{
			PhysicsHandle->GrabComponentAtLocation(
				ComponentToGrab,
				NAME_None,
				LineTraceEnd
			);
		}
}

void UGrabber::Drop()
{
		PhysicsHandle->ReleaseComponent();

		UE_LOG(LogTemp, Warning, TEXT("***DROPPED***"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

		//TODO : Clean code by either passing LineTraceEnd or by making it a class variable
	//Just for Line trace end
	//---------------Unnecessary redundant copy-----------------------------------------------
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);	

	FVector LineTraceEndRotation = PlayerViewPointRotation.Vector();
	FVector LineTraceEnd = PlayerViewPointLocation + (LineTraceEndRotation * Reach);
	//---------------Unnecessary redundant copy-----------------------------------------------


	//If physics handle is attached
	if(PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

FHitResult UGrabber::GetFirstObjectInReach() const
{
// Get player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);	

	FVector LineTraceEndRotation = PlayerViewPointRotation.Vector();
	FVector LineTraceEnd = PlayerViewPointLocation + (LineTraceEndRotation * Reach);
	
	//Draw a line from player to show reach
	/* Debug code
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
	*/
	//Reach out from player(RayCasting) to a certain distance
	FHitResult ObjectHit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());	//FCollisionQueryParams(TraceTag, bTraceComplex, IgnoreActor)
	//Trace tag = Tag used to provide extra information or filtering for debugging of the trace 
	//TraceComplex = whether to trace against complex collision
	//Actors to ignore - self passed since the trace line's origin begins from inside the actor 
	
	//Outhit = First blocking hit found
	//Start = PlayerViewPointLocation(start location of ray)
	//End = LineTraceEnd(end location of ray)
	//ObjectQueryParams = List of object types it's looking for
	//Params	Additional parameters used for the trace
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

	return ObjectHit; 
}