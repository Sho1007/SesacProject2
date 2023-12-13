// Fill out your copyright notice in the Description page of Project Settings.


#include "../InGame/StatusWidget.h"

#include <Components/ProgressBar.h>

#include "MarineCraft/Character/StatusComponent.h"

void UStatusWidget::InitWidget(UStatusComponent* NewStatusComponent)
{
	NewStatusComponent->OnThirstChange.BindUObject(this, &UStatusWidget::UpdateThirst);
	NewStatusComponent->OnHungerChange.BindUObject(this, &UStatusWidget::UpdateHunger);
	NewStatusComponent->OnHealthChange.BindUObject(this, &UStatusWidget::UpdateHealth);
}

void UStatusWidget::UpdateThirst(float CurrentThirst, float MaxThirst)
{
	ProgressBar_Thirst->SetPercent(CurrentThirst / MaxThirst);
}

void UStatusWidget::UpdateHunger(float CurrentHunger, float MaxHunger)
{
	ProgressBar_Hunger->SetPercent(CurrentHunger / MaxHunger);
}

void UStatusWidget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	ProgressBar_Health->SetPercent(CurrentHealth / MaxHealth);
}