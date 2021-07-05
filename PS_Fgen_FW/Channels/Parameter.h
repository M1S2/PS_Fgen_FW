/**
 * @file	Parameter.h
 * @date	03.02.2021 20:38:17
 * @author	Markus Scheich
 * @brief	Containing a class used to bundle min, max, default and the current values for parameters.
 */ 

#ifndef PARAMETER_H_
#define PARAMETER_H_

/**
 * Class used to bundle min, max, default and the current values for parameters.
 * @tparam T Type of the parameter
 */
template <class T>
class Parameter
{
	public:
		T Val;			/**< Current value of the parameter */
		T Min;			/**< Minimum value of the parameter */
		T Max;			/**< Maximum value of the parameter */
		T Def;			/**< Default value of the parameter */
		T Step;			/**< Step value of the parameter */

		/**
		 * Empty constructor of the Parameter.
		 */
		Parameter()
		{ }

		/**
		 * Constructor of the Parameter.
		 * @param val Current value of the parameter
		 * @param min Minimum value of the parameter
		 * @param max Maximum value of the parameter
		 * @param def Default value of the parameter
		 * @param step Step value of the parameter
		 */
		Parameter(T val, T min, T max, T def, T step) : Val(val), Min(min), Max(max), Def(def), Step(step)
		{ }
};

#endif /* PARAMETER_H_ */