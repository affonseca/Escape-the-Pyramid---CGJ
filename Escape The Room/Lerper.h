#pragma once

class BaseLerper
{
public:
	BaseLerper(){}
	virtual bool update(float percentage) = 0;

};

template <class T>
class Lerper : public BaseLerper{
public:
	Lerper(T* target, float* startValues, float* endValues, void(*lerpFunction)(T* target, float* startValues, float* endValues, float percentage));
	~Lerper();
	bool update(float delta);

private:
	T* target;
	float* start;
	float* end;
	void(*lerpFunction)(T* target, float* startValues, float* endValues, float percentage);
};

template <class T>
Lerper<T>::Lerper(T* target, float* startValues, float* endValues, void(*lerpFunction)(T* target, float* startValues, float* endValues, float percentage)) {
	this->target = target;
	this->start = startValues;
	this->end = endValues;
	this->lerpFunction = lerpFunction;
}

template <class T>
Lerper<T>::~Lerper() {
	delete start;
	delete end;
}

template <class T>
bool Lerper<T>::update(float percentage) {
	if (percentage < 0.0f) {
		percentage = 0.0f;
	}

	if (percentage > 1.0f) {
		percentage = 1.0f;
	}

	(*lerpFunction)(target, start, end, percentage);

	if (percentage == 1.0f)
		return true;
	return false;
}