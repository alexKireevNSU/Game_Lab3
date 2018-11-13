#include "Robot.h"

using namespace Robots;

void Robot_Collector::move(movement m) {
	switch (m) {
	case right: {
		block b = this->map.get_data(this->my_x + 1, this->my_y);
		if (b == rock) {
			return;
		}
		++this->my_x;
	}
	case left: {
		block b = this->map.get_data(this->my_x - 1, this->my_y);
		if (b == rock) {
			return;
		}
		--this->my_x;
	}
	case down: {
		block b = this->map.get_data(this->my_x, this->my_y - 1);
		if (b == rock) {
			return;
		}
		--this->my_y;
	}
	case up: {
		block b = this->map.get_data(this->my_x, this->my_y + 1);
		if (b == rock) {
			return;
		}
		++this->my_y;
	}
	default: { return; }
	}
}

void Robot_Collector::Grab() {

}