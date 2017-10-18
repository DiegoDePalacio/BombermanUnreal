// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeBombModifier.h"

TimeBombModifier::TimeBombModifier (ABombermanBoard* newBombermanBoard, int newCol, int newRow, ABombermanPlayer * newPlayer, int newLinearExtension)
	: RemoteBombModifier( newBombermanBoard, newCol, newRow, newPlayer, newLinearExtension )
{
	type = EModifierType::OWN_TIME_BOMB;
	timer = new TriggerModifierOnProcessTimer(this, Modifier::TIME_BOMB_IN_SECS);

	board->RegisterTimer(timer);
}

TimeBombModifier::~TimeBombModifier()
{
	delete(timer);
}

void TimeBombModifier::Process()
{
	board->UnregisterTimer(timer);
	RemoteBombModifier::Process();
}
