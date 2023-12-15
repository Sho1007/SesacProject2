// Fill out your copyright notice in the Description page of Project Settings.


#include "../Food/FoodBase.h"

#include "RenderGraphUtils.h"
#include "MarineCraft/Character/CharacterBase.h"
#include "MarineCraft/Character/StatusComponent.h"

void AFoodBase::Use()
{
	Super::Use();

	UStatusComponent* StatusComponent = Cast<UStatusComponent>(PlayerCharacter->GetComponentByClass(UStatusComponent::StaticClass()));
	check(StatusComponent);

	if (HungerAmount != 0.0f)
	{
		StatusComponent->AddHunger(HungerAmount);
	}
	if (ThirstAmount != 0.0f)
	{
		StatusComponent->AddThirsty(ThirstAmount);
	}

	if (--InstanceData.CurrentStack == 0)
	{
		PlayerCharacter->SetQuickSlotItemNull(InventoryIndex);
		Destroy();
	}

	PlayerCharacter->UpdateInventoryWidget();
}

float AFoodBase::GetCookTime() const
{
	return CookTime;
}