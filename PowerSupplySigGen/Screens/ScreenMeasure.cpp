/*
 * ScreenMeasure.cpp
 *
 * Created: 06.04.2021 19:45:48
 *  Author: V17
 */ 

#include "../Device.h"

Icon ico_dmm(40, 3, icon_dmm_width, icon_dmm_height, icon_dmm_bits);
Label lbl_DMM_caption(60, 5, "DMM");
ContainerPage page_DMM;

Icon ico_atx(40, 3, icon_voltage_width, icon_voltage_height, icon_voltage_bits);
Label lbl_ATX_caption(60, 5, "ATX");
ContainerPage page_ATX;

ContainerList list_Measure(40, 0, 240 - 40, 64);

UIElement* uiBuildScreenMeasure()
{
	page_DMM.AddItem(&ico_dmm);
	page_DMM.AddItem(&lbl_DMM_caption);
	page_DMM.InitItems();
	
	page_ATX.AddItem(&ico_atx);
	page_ATX.AddItem(&lbl_ATX_caption);
	page_ATX.InitItems();
	
	list_Measure.AddItem(&page_DMM);
	list_Measure.AddItem(&page_ATX);
	
	return &list_Measure;
}