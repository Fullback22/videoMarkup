#pragma once
#include "Classifier/Classifier.h"
#include "Figure/FigureRectangle.h"

class MarkupObject
{
public:
	int objectClass{};
	int activClass{};
	FigureRectangle* position{};
	
	MarkupObject();
};

