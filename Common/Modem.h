#pragma once
#include "Component.h"
class Modem :public Component
{

public:
	Modem(typeDevice device) :Component(device, MODEM) {}
};
