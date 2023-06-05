#pragma once
#include "Component.h"
class Antena :public Component
{
public:
	Antena(typeDevice device) :Component(device, ANTENA) {}
};

