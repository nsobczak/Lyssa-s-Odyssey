// Fill out your copyright notice in the Description page of Project Settings.

#include "StructButtonLinks.h"

FStructButtonLinks::FStructButtonLinks()
{
	ButtonGroupUniqueID = -1;
	ButtonUniqueID = -1;
	NextButtonID_Up = -1;
	NextButtonID_Right = -1;
	NextButtonID_Bottom = -1;
	NextButtonID_Left = -1;
}

FStructButtonLinks::FStructButtonLinks(int32 buttonGroupUniqueID, int32 buttonUniqueID,
	int32 nextButtonID_Up, int32 nextButtonID_Right, int32 nextButtonID_Bottom, int32 nextButtonID_Left)
{
	ButtonGroupUniqueID = buttonGroupUniqueID;
	ButtonUniqueID = buttonUniqueID;
	NextButtonID_Up = nextButtonID_Up;
	NextButtonID_Right = nextButtonID_Right;
	NextButtonID_Bottom = nextButtonID_Bottom;
	NextButtonID_Left = nextButtonID_Left;
}

void FStructButtonLinks::SetButtonLinks(int32 buttonGroupUniqueID, int32 buttonUniqueID,
	int32 nextButtonID_Up, int32 nextButtonID_Right, int32 nextButtonID_Bottom, int32 nextButtonID_Left)
{
	ButtonGroupUniqueID = buttonGroupUniqueID;
	ButtonUniqueID = buttonUniqueID;
	NextButtonID_Up = nextButtonID_Up;
	NextButtonID_Right = nextButtonID_Right;
	NextButtonID_Bottom = nextButtonID_Bottom;
	NextButtonID_Left = nextButtonID_Left;
}
