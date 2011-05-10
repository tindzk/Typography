#import "Chapter.h"

#define self Chapter

rsdef(self *, New, RdString title) {
	self *res = Memory_New(sizeof(self));

	return *res = (self) {
		.title    = title,
		.sections = SectionArray_New(0)
	}, res;
}

def(void, Destroy) {
	each(sect, this->sections) {
		Section_Destroy(*sect);
	}

	SectionArray_Free(this->sections);
	Body_Destroy(&this->body);

	Memory_Destroy(this);
}

def(void, SetBody, Body body) {
	this->body = body;
}
