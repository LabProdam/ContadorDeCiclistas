#include "ObjectCounter.hpp"
#include "Utils.hpp"

ObjectCounter::ObjectCounter(cv::Rect referenceBox) {
	Config configuration;
	memset(this->countedPoint, -1, sizeof(this->countedPoint));
	this->pos = 0;
	this->totalLeftCount = configuration.GetLeftCounter();
	this->totalRightCount = configuration.GetRightCounter();
	this->referenceBox = referenceBox;	
}

bool ObjectCounter::AccountPoint(TrackedObject point) {
	bool newElement = true;
	bool addedNew = false;
	for (auto it : this->countedPoint) {
		if (it == point.id) {
			newElement = false;
			break;
		}
	}
	
		printf("id: %d, Width: %d, Traj: %f\n", point.id, point.rect.width, fabs(point.pt.x - point.pt0.x) );
	
	if (newElement && 
		point.framesAlive > 2 &&
		abs(point.pt.x - point.pt0.x) > (1.0 * this->referenceBox.width) &&
		point.rect.width > (0.7 * this->referenceBox.width) && 				
		point.rect.width < (1.25 * this->referenceBox.width)) {				
		if (point.pt.x > this->referenceBox.tl().x &&
			point.pt.x < this->referenceBox.br().x &&
			point.pt.y > this->referenceBox.tl().y &&
			point.pt.y < this->referenceBox.br().y)
		{
			Config configuration;
			
			if (point.ltr) {
				this->totalLeftCount++;
				configuration.SetLeftCounter(this->totalLeftCount);
			}
			else {
				this->totalRightCount++;
				configuration.SetRightCounter(this->totalRightCount);
			}
			
			this->countedPoint[pos++] = point.id;
			if (pos >= sizeof(countedPoint)/sizeof(countedPoint[0])) {
				pos = 0;
			}
			
			addedNew = true;
		}
	}
	return addedNew;
}

unsigned int ObjectCounter::GetTotalPoints() {
	return this->totalLeftCount + this->totalRightCount;
}	

unsigned int ObjectCounter::GetLTRPoints() {
	return this->totalLeftCount;
}

unsigned int ObjectCounter::GetRTLPoints() {
	return this->totalRightCount;
}

void ObjectCounter::ZeroCounters() {
	Config configuration;
	
	this->totalLeftCount = 0;
	this->totalRightCount = 0;
	
	configuration.SetLeftCounter(this->totalLeftCount);
	configuration.SetLeftCounter(this->totalRightCount);
}

void ObjectCounter::SetReferenceBox(cv::Rect referenceBox) {
	this->referenceBox = referenceBox;
}
