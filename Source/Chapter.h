#import <String.h>
#import <docslib/Body.h>

#import "Section.h"

Array(Section *, SectionArray);

record(Chapter) {
	Body         body;
	RdString     title;
	SectionArray *sections;
};

Array(Chapter *, ChapterArray);
