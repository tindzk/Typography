#import <String.h>

#import "Body.h"
#import "Section.h"

class(Chapter) {
	String title;
	Body body;
	Array(Section *, *sections);
};
