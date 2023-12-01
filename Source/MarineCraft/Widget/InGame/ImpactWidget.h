// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ImpactWidget.generated.h"

/**
 * 
 */
class UImage;
UCLASS()
class MARINECRAFT_API UImpactWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void Impact();

	void ImpactOnDeath();

private:
	UPROPERTY(Meta = ( AllowPrivateAccess, BindWidget) )
	UImage* Img_Blood;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	float DisapearSpeed;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	float OpacityIncreaseAmount;

	bool bIsDead;
};