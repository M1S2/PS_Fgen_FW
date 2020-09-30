/*
 * UserControlBool.h
 *
 * Created: 28.09.2020 19:23:06
 *  Author: V17
 */ 


#ifndef USERCONTROLBOOL_H_
#define USERCONTROLBOOL_H_

#include "UserControlBase.h"

class UserControlBool : public UserControlBase<uint8_t>
{
private:
	
public:
	UserControlBool(uint8_t locx, uint8_t locy, char* label, uint8_t* controlValue) : UserControlBase(locx, locy, label, controlValue)
	{}
	
	void KeyInput(Keys_t key);
	void EncoderInput(EncoderDirection_t encDir);
	void Draw(u8g_t *u8g);
};

#endif /* USERCONTROLBOOL_H_ */