#import <String.h>

#import "Body.h"
#import "Section.h"

Array_Define(Section *, SectionArray);

record(Chapter) {
	String title;
	Body body;
	SectionArray *sections;
};
