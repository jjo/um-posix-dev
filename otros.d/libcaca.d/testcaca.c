/* $Id: testcaca.c,v 1.2 2004/10/22 19:50:51 jjo Exp $ */
/* 
 * vim: ts=2:aw
 */
#include <stdlib.h>
#include <caca.h>

int main(void)
{
	int event;
	int ch;
	struct caca_sprite *carita=NULL;

	if (caca_init()) 
		return 1;
	caca_set_feature(CACA_ANTIALIASING_PREFILTER);
	caca_set_delay(40000);
	caca_set_color(CACA_COLOR_YELLOW, CACA_COLOR_RED);
	caca_draw_thin_box(0,0,10,2);
	caca_refresh();
	carita=caca_load_sprite("carita.txt");
	while(1) {
		while((event = caca_get_event(CACA_EVENT_ANY))) {
			caca_set_color(CACA_COLOR_BLACK, CACA_COLOR_WHITE);
			caca_printf(1,1, "%x\n", event);
			if(event & CACA_EVENT_KEY_PRESS)
			{
				ch=event & 0xffff;
				switch(ch)
				{
					case 'q':
					case 'Q':
						goto fin;
					case '0'...'9':  /* extension gcc */
						caca_set_color(caca_rand(2,14), CACA_COLOR_BLACK);
						caca_draw_thin_line(0,3,caca_get_width(),3+(ch-'0')*10);
					default:
						if (carita)
							caca_draw_sprite(caca_rand(4,caca_get_width()), caca_rand(4,caca_get_height()), carita, 0);
				}
			}
			caca_refresh();
		}
	}
fin:
	caca_end();
	return 0;
}

