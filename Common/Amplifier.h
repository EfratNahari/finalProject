#pragma once
#include "../Common/Component.h"
class Amplifier :public Component
{
public:
	Amplifier(typeDevice device) :Component(device, AMPLIFIER) {}
};

