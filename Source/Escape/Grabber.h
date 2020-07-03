// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	static constexpr float Reach  = 150.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;	//utility object for moving physics objects around
	UInputComponent* InputComponent = nullptr;

	void FindPhysicsHandle();
	void SetupInputComponent();

	void Grab();
	void Drop();

	//Get the first AActor withing reach with physics body
	FHitResult GetFirstObjectInReach() const; 
};
