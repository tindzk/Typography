#import <String.h>

#import "Body.h"
#import "Section.h"

record(Chapter) {
	String title;
	Body body;
	Array(Section *, *sections);
};
