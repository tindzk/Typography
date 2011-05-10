#import <String.h>
#import <docslib/Body.h>

#import "Section.h"

#define self Chapter

Array(Section *, SectionArray);

class {
	Body         body;
	RdString     title;
	SectionArray *sections;
};

Array(Chapter *, ChapterArray);

rsdef(self *, New, RdString title);
def(void, Destroy);
def(void, SetBody, Body body);

#undef self
