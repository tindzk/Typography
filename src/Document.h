#import <Array.h>
#import <String.h>

#import "Chapter.h"

class(Document) {
	String title;
	Array(Chapter *, *chapters);
};
