#pragma once

#include <cassert>
#include <vector>
#include "Lerper.h"

class Timeline;

class TimelineNode
{
public:
	TimelineNode(BaseLerper* animation, float durationTime);
	~TimelineNode();

	void update(float enlapsedTime);
	float getTotalDurationTime();
	TimelineNode* addChild(TimelineNode* child);
	void setTimeline(Timeline* timeline);

private:
	float currentTime = -1;
	float durationTime;
	Timeline* timeline;
	BaseLerper *animation;
	std::vector<TimelineNode*> timelineChildren;
};

class Timeline
{
public:
	Timeline();
	~Timeline();

	void start();
	void update(float deltaTime);
	void pause();
	void stop();

	//get info
	TimelineNode* getRoot();
	bool isRunning();
	bool isReversed();
	void setIsReversed(bool newReversed);

	float getTimeSinceStart();
	float getMaxTime();

private:
	TimelineNode* root;

	float  timeSinceStart = -1;
	float  maxTime = -1;
	bool _isRunning = false;
	bool _isReversed = false;
};