// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/SmallGame/SmallGame1.h"
#include "Components/Button.h"

void USmallGame1::NativeConstruct()
{
	
}

bool USmallGame1::CheckCurrentButtonAngle()
{
	if (
		Button1->GetRenderTransformAngle() == 0 &&
		Button2->GetRenderTransformAngle() == 0 &&
		Button4->GetRenderTransformAngle() == 0 &&
		(Button5->GetRenderTransformAngle() == 90 || Button5->GetRenderTransformAngle() == 180) &&
		Button7->GetRenderTransformAngle() == 0 &&
		(Button8->GetRenderTransformAngle() == 0 || Button8->GetRenderTransformAngle() == 180) &&
		Button9->GetRenderTransformAngle() == 0
		)
	{
		return true;
	}

	return false;
}
