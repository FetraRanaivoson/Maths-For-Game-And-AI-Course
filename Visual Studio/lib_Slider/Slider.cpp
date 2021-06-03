#include "Slider.h"

Slider::Slider(int x, int y, int width, double min, double max, double value) {
	this->x = x;
	this->y = y;
	this->width = width;

	this->min = min;
	this->max = max;
	this->value = min;
	this->setValue(value);

	isDragMode = false;
}

void Slider::setValue(double value) {
	if (value >= this->min && value <= this->max) {
		this->value = value;
		this->computePos();
	}
}

void Slider::setMin(double min) {
	if (min <= this->max) {
		this->min = min;
	}
}

void Slider::setMax(double max) {
	if (max >= this->min) {
		this->max = max;
	}
}

void Slider::setCursorPosition(int pos) {
	if (pos >= this->x && pos <= this->x + this->width) {
		this->pos = pos;
		this->computeValue();
	}
}

double Slider::getValue() {
	return this->value;
}

void Slider::computeValue() {
	this->value = this->min + ((double)this->pos - this->x)*(this->max - this->min) / this->width;
}

void Slider::computePos() {
	this->pos = (int)(this->x + (this->value - this->min)* this->width / (this->max - this->min));
}

bool Slider::isNear(int mouseX, int mouseY) {
	return (abs(mouseY - this->y) < Slider::cursorHeight / 2)
		&& (mouseX >= this->x - Slider::nearXOffset) && (mouseX <= this->x + this->width + Slider::nearXOffset);
}

double Slider::draw(SDL_Renderer *renderer, SDL_Event &evt) {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	update(evt);

	//	draw the line
	SDL_Rect rect;
	rect.w = this->width;
	rect.h = Slider::lineThickness;
	rect.x = this->x;
	rect.y = this->y - Slider::lineThickness / 2;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);

	//	draw the cursor
	rect.w = Slider::cursorWidth;
	rect.h = Slider::cursorHeight;
	rect.x = this->pos - Slider::cursorWidth / 2;
	rect.y = this->y - Slider::cursorHeight / 2;

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 200);
	SDL_RenderFillRect(renderer, &rect);

	return this->value;
}

void Slider::update(SDL_Event &evt) {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);	//	getting mouse information

	//	start of drag and drop
	if (evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_LEFT && this->isNear(mouseX, mouseY)) {
		this->setCursorPosition(mouseX);
		isDragMode = true;
		evt.type = 0;	//	set event to nothing
	}

	//	move point
	else if (evt.type == SDL_MOUSEMOTION && isDragMode) {
		this->setCursorPosition(mouseX);
		evt.type = 0;	//	set event to nothing
	}

	//	end of drag and drop
	else if (evt.type == SDL_MOUSEBUTTONUP && isDragMode) {
		isDragMode = false;
		evt.type = 0;	//	set event to nothing
	}
}
