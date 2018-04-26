// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "Public/Tank.h"
#include "TankPlayerController.h"

void ATankAIController::BeginPlay() {
	Super::BeginPlay();

	if (GetControlledTank() != nullptr) {
		FString TankName = GetControlledTank()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("%s is on the Battlefield (A.I.)!"), *TankName);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("We lost a tank!"));
	}
	GetPlayerTank();
}

void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GetPlayerTank()) {
		auto PlayerLocation = GetPlayerTank()->GetActorLocation();
		GetControlledTank()->AimAt(PlayerLocation);
	}
}

ATank* ATankAIController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const {
	ATank* PlayerTank;
	if ((PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn())) != nullptr) {
		return PlayerTank;
	}
	UE_LOG(LogTemp, Warning, TEXT("AI : I am not seeing shit"));
	return nullptr;
}