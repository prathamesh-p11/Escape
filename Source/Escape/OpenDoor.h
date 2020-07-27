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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float OpenAngle;
	float CurrentYaw;
	float InitialYaw;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY()
	UAudioComponent* DoorAudioComponent;

	float DoorOpenedAt = 0.f;
	constexpr static float DoorCloseDelay = 0.5f;

	float DoorOpenSpeed = 0.8f;
	float DoorCloseSpeed = 2.f;

	float MinMassToOpenDoor = 50.f;

	bool OpenDoorSound = false;
	bool CloseDoorSound = true;
};
