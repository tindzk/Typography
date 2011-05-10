#import <String.h>
#import <docslib/Body.h>

#define self Section

class {
	RdString title;
	Body     body;
};

rsdef(self *, New, RdString title);
def(void, Destroy);
def(void, SetBody, Body body);

#undef self
