#import "Chapter.h"

def(void, Init, String title) {
	Body_Init(&this->body);
	this->title = String_Clone(title);
	Array_Init(this->sections, 0);
}

def(Section *, AddSection, String title) {
	Section *sect = New(Section);

	Section_Init(sect, title);

	Array_Push(this->sections, sect);

	return sect;
}
