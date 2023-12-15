// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InGamePlayerController.generated.h"

class UGameOverWidget;
class IInteractInterface;
/**
 * 
 */
class UInGameWidget;
class UPlayerInventoryComponent;
UCLASS()
class MARINECRAFT_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void SetChargePercent( float NewChargePercent );
	void UpdateInteractActor( IInteractInterface* NewInteractActor );
	void UpdateInventoryWidget( UPlayerInventoryComponent* PlayerInventoryComponent );
	void SetCurrentItem(int32 NewItemIndex);
	void ToggleInventory();
	void Impact( float DamageAmount );
	void Die();

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UGameOverWidget> GameOverWidgetClass;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UInGameWidget> InGameWidgetClass;
	UPROPERTY(Meta = ( AllowPrivateAccess ) )
	UInGameWidget* InGameWidget;
};