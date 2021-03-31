/*
 * ContainerList.h
 *
 * Created: 29.03.2021 18:46:29
 *  Author: V17
 */ 


#ifndef CONTAINERLIST_H_
#define CONTAINERLIST_H_

#include "Container.h"

#define SCROLLBAR_WIDTH			4
#define SCROLLBAR_MARGIN		2

class ContainerList : public Container
{
	public:
		ContainerList(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height);
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
		virtual bool KeyInput(Keys_t key) override;

		bool NextItem();
		bool PreviousItem();
};

#endif /* CONTAINERLIST_H_ */