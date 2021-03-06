/*
 * UserControlBool.h
 *
 * Created: 28.09.2020 19:23:06
 *  Author: V17
 */ 

#ifndef USERCONTROLBOOL_H_
#define USERCONTROLBOOL_H_

#include "UserControlBase.h"

class UserControlBool : public UserControlBase
{
private:
	bool* _controlValue;
	bool _controlValueDraw;				// This variable is updated from the _controlValue on each draw of the first page.
	
public:
	
	UserControlBool(uint8_t locx, uint8_t locy, bool* controlValue, void* valueChangedContext, void (*onValueChanged)(void*), const u8g_pgm_uint8_t* iconBits = NULL);
	void KeyInput(Keys_t key);	
	void EncoderInput(EncoderDirection_t encDir);	
	void EncoderPBInput();	
	void Draw(u8g_t *u8g, bool isFirstPage);
};

#endif /* USERCONTROLBOOL_H_ */