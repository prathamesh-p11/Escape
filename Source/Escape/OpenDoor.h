// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float OpenAngle;
	float CurrentYaw;
	float InitialYaw;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	//Property to select actor which would trigger the pressure plate (manually set to default pawn every time in UE editor Open Door component)
	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpensDoor; 

	float DoorOpenedAt = 0.f;
	constexpr static float DoorCloseDelay = 0.5f;

	float DoorOpenSpeed = 0.8f;
	float DoorCloseSpeed = 2.f;
};
