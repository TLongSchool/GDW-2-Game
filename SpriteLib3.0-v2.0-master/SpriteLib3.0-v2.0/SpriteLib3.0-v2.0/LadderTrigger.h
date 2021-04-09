#pragma once
#include "Trigger.h"
#include <Box2D/Box2D.h>

class LadderTrigger : public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;

	bool climb = false;
protected:
};

