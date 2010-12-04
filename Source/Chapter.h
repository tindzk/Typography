#import <String.h>
#import <docslib/Body.h>

#import "Section.h"

Array(Section *, SectionArray);

record(Chapter) {
	String title;
	Body body;
	SectionArray *sections;
};

Array(Chapter *, ChapterArray);
