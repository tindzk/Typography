#import "Section.h"

#define self Section

rsdef(self *, New, RdString title) {
	self *res = Memory_New(sizeof(self));

	return *res = (self) {
		.title = title
	}, res;
}

def(void, Destroy) {
	Body_Destroy(&this->body);
	Memory_Destroy(this);
}

def(void, SetBody, Body body) {
	this->body = body;
}
