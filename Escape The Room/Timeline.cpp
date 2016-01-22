#include "Timeline.h"

TimelineNode::TimelineNode(BaseLerper* animation, float durationTime)
{
	this->animation = animation;
	this->durationTime = durationTime;
}

TimelineNode::~TimelineNode()
{
	if(this->animation != NULL)
		delete animation;
	timelineChildren.clear();
}

void TimelineNode::update(float enlapsedTime)
{
	if (timeline == NULL)
		return;

	else if (!timeline->isRunning())
		return;

	float rest = enlapsedTime - this->durationTime;

	if(animation != NULL && ((currentTime >= 0.0f || enlapsedTime > 0.0f) && (currentTime < durationTime || enlapsedTime < durationTime))){
		animation->update(enlapsedTime / this->durationTime);
		currentTime = enlapsedTime;
	}

	for (std::vector<TimelineNode*>::iterator child = timelineChildren.begin(); child != timelineChildren.end(); ++child) {
		(*child)->update(rest);
	}

}

float TimelineNode::getTotalDurationTime()
{
	float childrenTime = 0.0f;

	for (std::vector<TimelineNode*>::iterator child = timelineChildren.begin(); child != timelineChildren.end(); ++child) {
		float newTime = (*child)->getTotalDurationTime();
		childrenTime = (newTime > childrenTime) ? newTime : childrenTime;
	}

	return this->durationTime + childrenTime;
}

TimelineNode* TimelineNode::addChild(TimelineNode * child)
{
	if(this->timeline != NULL)
		assert(("A child cannot be added while the timeline is running!", !this->timeline->isRunning()));

	timelineChildren.push_back(child);
	child->setTimeline(this->timeline);

	return child;
}

void TimelineNode::setTimeline(Timeline * timeline)
{
	if (timeline != NULL)
		assert(("A timeline cannot be set it it is running!", !timeline->isRunning()));

	this->timeline = timeline;
}

Timeline::Timeline()
{
	this->root = new TimelineNode(NULL, 0.0f);
	this->root->setTimeline(this);
}

Timeline::~Timeline()
{
	if(this->root != NULL)
		delete this->root;
}

void Timeline::start()
{
	assert(("Cannot start the timeline twice!", !isRunning()));

	this->_isRunning = true;
	this->maxTime = root->getTotalDurationTime();

	if (this->timeSinceStart == -1) {
		if (this->_isReversed)
			this->timeSinceStart = this->maxTime;
		else
			this->timeSinceStart = 0.0f;
	}
}

void Timeline::update(float deltaTime)
{
	if (!isRunning())
		return;

	bool reachedEnd = false;
	if (isReversed()) {
		this->timeSinceStart -= deltaTime;
		if (this->timeSinceStart < 0.0f) {
			timeSinceStart = 0.0f;
			reachedEnd = true;
		}
	}
	else {
		this->timeSinceStart += deltaTime;
		if (this->timeSinceStart > this->maxTime) {
			timeSinceStart = maxTime;
			reachedEnd = true;
		}
	}

	root->update(timeSinceStart);

	if (reachedEnd) {
		this->timeSinceStart = -1.0f;
		this->_isRunning = false;
	}
}

void Timeline::pause()
{
	this->_isRunning = false;
}

void Timeline::stop()
{
	this->_isRunning = false;
	this->timeSinceStart = -1.0f;
}

TimelineNode* Timeline::getRoot()
{
	return this->root;
}

bool Timeline::isRunning()
{
	return _isRunning;
}

bool Timeline::isReversed()
{
	return _isReversed;
}

void Timeline::setIsReversed(bool newReversed)
{
	this->_isReversed = newReversed;
}

float Timeline::getTimeSinceStart()
{
	return this->timeSinceStart;
}

float Timeline::getMaxTime()
{
	return this->maxTime;
}