#include "touch_shell.h"

#define HEIGHT_TITLE_BAR 31.0f

bool touch_press = false;
SceTouchData touch;	
int countTouch = 0;
int pre_touch_y = 0;
int pre_touch_x = 0;
bool touch_nothing = false;

// State_Touch is global value to certain function TOUCH_FRONT alway return absolute one value for one time.
int State_Touch = -1;
float slide_value_hold = 0;
float slide_value = 0;

bool saved_loc = false;
bool have_slide = false;
bool have_touch_hold = false;
bool have_touch = true;
int MIN_SLIDE_VERTTICAL = 40.0f;
int MIN_SLIDE_HORIZONTAL = 100.0f;
int AREA_TOUCH = 40.0f;
bool disable_touch = false;

clock_t time_on_touch_start = 0;

// Output:
//	0 : Touch down select
//	1 : Touch up select
//	2 : Double touch (temporary disable)
//  3 : Hold Touch
//  4 : Empty slot
//	5 : Empty slot
//	6 : Touch slide up
//	7 : Touch slide down
//  8 : Touch slide left
//  9 : Touch slide right
// 10 : Two finger slide right
// 11 : Empty slot
// 12 : Empty slot
// 13 : Empty slot
// 14 : Three finger slide left
// 15 : Three finger slide right
int TOUCH_FRONT() {	
	sceTouchPeek(0, &touch, 1);
		
	if(touch_press & (touch.reportNum == 0) ) {
		saved_loc = false;			
		time_on_touch_start = 0;		

		//Touch up Select Event
		//prevent event slide
		if (!have_slide) {			
			State_Touch = 1;					

		//Touch Double Event		
		//prevent event slide
		//if (!have_slide) 
			//if ((touch.report[0].x < pre_touch_x + AREA_TOUCH) & (touch.report[0].x > pre_touch_x - AREA_TOUCH) & (touch.report[0].y < pre_touch_y + AREA_TOUCH) & (touch.report[0].y > pre_touch_y - AREA_TOUCH)) {
				//countTouch++;			
			//}						
		}
		else {
			touch_nothing = false;			
		}


		if (countTouch >= 2) {			
			countTouch = 0;
			//State_Touch = 2;			
		}
				
		//reset listen event slide
		have_slide = false;	
	}

	if (touch.reportNum > 0) {
		touch_press = true;		
		
		if (!saved_loc) {
			if (touch.report[0].x > pre_touch_x + AREA_TOUCH || touch.report[0].x < pre_touch_x - AREA_TOUCH) {			 
				pre_touch_x = touch.report[0].x;
				countTouch = 0;
			}
			if (touch.report[0].y > pre_touch_y + AREA_TOUCH || touch.report[0].y < pre_touch_y - AREA_TOUCH) {
				pre_touch_y = touch.report[0].y;				
				countTouch = 0;
			}
			time_on_touch_start = clock();
			saved_loc = true;
			slide_value_hold = slide_value;

			//Touch down Select Event				
			State_Touch = 0;					
			
		}

		//Touch Slide Up Event
		if (((touch.report[0].y < pre_touch_y - MIN_SLIDE_VERTTICAL)  || (pre_touch_y > SCREEN_HEIGHT*2 - HEIGHT_TITLE_BAR)) & (touch.reportNum == 1)) {			
			State_Touch = 6;							
			have_slide = true;									
			return State_Touch;
		}

		//Touch Slide Down Event
		if (((touch.report[0].y > pre_touch_y + MIN_SLIDE_VERTTICAL) || (pre_touch_y < HEIGHT_TITLE_BAR)) & (touch.reportNum == 1)) {			
			State_Touch = 7;												
			have_slide = true;				
			return State_Touch;
		}

		//Touch Slide Left Event
		if ((touch.report[0].x < pre_touch_x - MIN_SLIDE_HORIZONTAL) & (touch.reportNum == 1)) {			
			State_Touch = 8;						
			have_slide = true;									
			return State_Touch;
		}

		//Touch Slide Right Event
		if ((touch.report[0].x > pre_touch_x + MIN_SLIDE_HORIZONTAL) & (touch.reportNum == 1)) {			
			State_Touch = 9;										
			have_slide = true;				
			return State_Touch;
		} else

		//Two finger slide right
		if ((touch.report[0].x > pre_touch_x + MIN_SLIDE_HORIZONTAL) & (touch.report[1].x > pre_touch_x + MIN_SLIDE_HORIZONTAL) & (touch.reportNum == 2)) {			
			State_Touch = 10;										
			have_slide = true;				
			return State_Touch;
		}

		if (!have_slide) {												
			if (((float)(clock() - time_on_touch_start)/CLOCKS_PER_SEC  > 0.4f) & (time_on_touch_start > 0)) {
				if ((touch.report[0].x < pre_touch_x + AREA_TOUCH) & (touch.report[0].x > pre_touch_x - AREA_TOUCH) & (touch.report[0].y < pre_touch_y + AREA_TOUCH) & (touch.report[0].y > pre_touch_y - AREA_TOUCH)) {
					State_Touch = 3;							
					return State_Touch;			
				}
			}																
		}
	}
	else touch_press = false;
	return State_Touch;
}
