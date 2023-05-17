// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectableInterface.h"

// Add default functionality here for any ISelectableInterface functions that are not pure virtual.
void ISelectableInterface::Select()
{
	GLog->Log("> Error!: This class should not be instantiated!");
}

void ISelectableInterface::Deselect()
{
	GLog->Log("> Error!: This class should not be instantiated!");
}
