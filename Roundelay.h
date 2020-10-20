#pragma once
#include <iostream>
#include "Hash.h"

const int maxCommandLength = 32;

class Roundelay
{
	Roundelay(const Roundelay&) = delete;
	Roundelay& operator=(const Roundelay&) = delete;

public:
	Roundelay();
	~Roundelay();

	void Hop();

private:
	Hash hs;
};

