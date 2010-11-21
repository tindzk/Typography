#import <String.h>
#import <docslib/Body.h>

#import "Section.h"

Array_Define(Section *, SectionArray);

record(Chapter) {
	String title;
	Body body;
	SectionArray *sections;
};

Array_Define(Chapter *, ChapterArray);
