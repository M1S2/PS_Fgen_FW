/*
 * Parameter.h
 *
 * Created: 03.02.2021 20:38:17
 *  Author: V17
 */ 


#ifndef PARAMETER_H_
#define PARAMETER_H_

template <class T>
class Parameter
{
	public:
	T Val;
	T Min;
	T Max;
	T Def;
	T Step;
	bool Supported;

	Parameter() : Supported(false)
	{ }

	Parameter(T val, T min, T max, T def, T step, bool supported) : Val(val), Min(min), Max(max), Def(def), Step(step), Supported(supported)
	{ }
};

#endif /* PARAMETER_H_ */