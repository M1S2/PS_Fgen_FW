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

	Parameter()
	{ }

	Parameter(T val, T min, T max, T def, T step) : Val(val), Min(min), Max(max), Def(def), Step(step)
	{ }
};

#endif /* PARAMETER_H_ */