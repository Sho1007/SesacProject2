// Fill out your copyright notice in the Description page of Project Settings.


#include "../InGame/ImpactWidget.h"

#include <Components/Image.h>

void UImpactWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry , InDeltaTime);

	if (bIsDead == false) Img_Blood->SetOpacity( FMath::Clamp( Img_Blood->ColorAndOpacity.A - ( ( DisapearSpeed * InDeltaTime ) / ( Img_Blood->ColorAndOpacity.A + 0.001 ) ) , 0.0f , 1.0f ) );
}

void UImpactWidget::Impact()
{
	Img_Blood->SetOpacity( FMath::Clamp( Img_Blood->ColorAndOpacity.A + OpacityIncreaseAmount , 0.0f , 1.0f ) );
}

void UImpactWidget::ImpactOnDeath()
{
	Img_Blood->SetOpacity( 1.0f );
	bIsDead = true;
}