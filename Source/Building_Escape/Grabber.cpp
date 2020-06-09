// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::HandleCheck()
{
		
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(PhysicsHandle){

		//Physics Handle found

	}
	else{

		UE_LOG(LogTemp,Error,TEXT("Physics Handle MUST be added. It is currently not present on %s."), *GetOwner()->GetName());

	}
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	HandleCheck();
	InputCapture();
}

void UGrabber::InputCapture()
{

GrabberComp = GetOwner()->FindComponentByClass<UInputComponent>();

	if(GrabberComp){
		UE_LOG(LogTemp,Warning,TEXT("Grabber input present on %s"), *GetOwner()->GetName());
		GrabberComp->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		GrabberComp->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}

}

void UGrabber::Grab()
{

UE_LOG(LogTemp, Warning, TEXT("Grabber Button pressed"));

FVector CurrentLocation;
FRotator CurrentRotation;

GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT CurrentLocation, OUT CurrentRotation);


FVector LineTraceEnd = CurrentLocation + CurrentRotation.Vector() * Reach ;
	
FHitResult HitResult = GetFirstPhsyicsBodyInReach();
UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

if (HitResult.GetActor())
{

	PhysicsHandle->GrabComponentAtLocation(ComponentToGrab,NAME_None,LineTraceEnd);

}

}

void UGrabber::Release()
{

UE_LOG(LogTemp, Warning, TEXT("Grabber Button released"));


if (PhysicsHandle)
{

	PhysicsHandle->ReleaseComponent();

}

}
FHitResult UGrabber::GetFirstPhsyicsBodyInReach() const
{

	
	FVector CurrentLocation;
	FRotator CurrentRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT CurrentLocation, OUT CurrentRotation);

	FVector LineTraceEnd = CurrentLocation + CurrentRotation.Vector() * Reach ;

	// Ray-cast to a certain distance
	
	FHitResult PlayerHit;
	
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());


	GetWorld()->LineTraceSingleByObjectType(OUT PlayerHit, CurrentLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),TraceParams);
	// See what it hits

  AActor* ActorHit = PlayerHit.GetActor();

  if(ActorHit){

  UE_LOG(LogTemp,Error,TEXT("You are in grabbing range of %s"), *ActorHit->GetName());

  }

	return PlayerHit;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if(PhysicsHandle->GrabbedComponent)
	{
		FVector CurrentLocation;
	    FRotator CurrentRotation;
	    

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT CurrentLocation, OUT CurrentRotation);

	    FVector LineTraceEnd = CurrentLocation + CurrentRotation.Vector() * Reach ;

		PhysicsHandle->SetTargetLocation(LineTraceEnd);

	}
	
}

