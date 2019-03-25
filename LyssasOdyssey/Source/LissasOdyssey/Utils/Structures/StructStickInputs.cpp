// Fill out your copyright notice in the Description page of Project Settings.

#include "StructStickInputs.h"

FStructStickInputs::FStructStickInputs()
{
}

FStructStickInputs::FStructStickInputs(float top, float right, float down, float left)
{
	Inputs = { top, right, down, left };
}
