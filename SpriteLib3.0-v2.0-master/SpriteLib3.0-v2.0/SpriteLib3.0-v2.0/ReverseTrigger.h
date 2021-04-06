#pragma once

#include "Trigger.h"

class ReverseTrigger : public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;
protected:

	bool triggered = false;
};