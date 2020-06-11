// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	bool WasOpenSoundPlayed = false;
	bool WasCloseSoundPlayed = true;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
float InitialYaw = 0.0f;
float CurrentYaw = 0.0f;
float DoorLastOpened = 0.0f;


UPROPERTY(EditAnywhere)
float DoorCloseDelay = 0.0f;

UPROPERTY(EditAnywhere)
float TargetYaw = 90.0f;

UPROPERTY(EditAnywhere)
ATriggerVolume* PressurePlate = nullptr;

UPROPERTY(EditAnywhere)
UAudioComponent* DoorSound = nullptr;

UPROPERTY(EditAnywhere)
float OpeningSpeed = .6f;

UPROPERTY(EditAnywhere)
float ClosingSpeed = 4.0f;

UPROPERTY(EditAnywhere)
float MassToOpenDoor = 60.f;

};
