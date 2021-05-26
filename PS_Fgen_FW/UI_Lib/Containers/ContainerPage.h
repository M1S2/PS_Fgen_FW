/*
 * ContainerPage.h
 *
 * Created: 12.03.2021 18:32:49
 *  Author: V17
 */ 


#ifndef CONTAINERPAGE_H_
#define CONTAINERPAGE_H_

#include "Container.h"

class ContainerPage : public Container
{
	public:
		ContainerPage();
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		virtual bool KeyInput(Keys_t key) override;
		
		void InitItems();
		bool NextItem();
		bool PreviousItem();
		bool NextControlItem();
		bool PreviousControlItem();
};

#endif /* CONTAINERPAGE_H_ */