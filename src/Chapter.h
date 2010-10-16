#import <String.h>

#import "Body.h"
#import "Section.h"

record {
	String title;
	Body body;
	Array(Section *, *sections);
} Chapter;
