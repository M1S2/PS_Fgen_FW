/*
 * DevSettings.h
 *
 * Created: 15.09.2020 19:33:01
 *  Author: V17
 */ 


#ifndef DEVSETTINGS_H_
#define DEVSETTINGS_H_

struct DevSettings_t
{
	float PS_Voltage;
	bool PS_Output_Enabled;
};

extern DevSettings_t DevSettings;		// defined in main.cpp

#endif /* DEVSETTINGS_H_ */