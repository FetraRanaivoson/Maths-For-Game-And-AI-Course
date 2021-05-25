#include "UIKit.h"
#include "time.h"
#include "../lib_Timer/Timer.h"

#include <iostream>
using namespace std;

int main() {
	cout << "CLOCKS_PER_SEC = " << CLOCKS_PER_SEC << endl;

	//	Timer(clock_t duration)
	int cpt1 = 0;			//	first counter
	Timer monTimer1(350);	//	and its timer

	int cpt2 = 0;			//	second counter
	Timer monTimer2(120);	//	and its timer

	//	Ticker : counting time duration
	double x = 0, y = 20;	//	object position on screen
	double vx = 10;			//	10 characters per second
	Ticker ticker;			//	ticker associated to this object

	while (true) {
		//	update first counter
		if (monTimer1.isReady()) {
			UIKit::gotoXY(5, 2);
			cout << cpt1;
			cpt1++;
		}

		//	update second counter
		if (monTimer2.isReady()) {
			UIKit::gotoXY(10, 2);
			cout << cpt2;
			cpt2++;
		}

		//	update point
		double newX = x + vx * ticker.getEllapsedTime();

		//	don't draw if same screen position
		if (abs((int)newX - (int)x) != 0) {

			//	erase character at old position
			UIKit::gotoXY((int)x, (int)y);
			cout << " " << endl;

			//	go back to column 0 if last column has been reached
			if (newX > 50) {
				newX = 0;
			}

			//	draw character at new position
			UIKit::gotoXY((int)newX, (int)y);
			cout << "O";
		}

		//	in any case, update new position
		x = newX;
	}
}