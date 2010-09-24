#import "Section.h"

def(void, Init, String title) {
	Body_Init(&this->body);
	this->title = String_Clone(title);
}
