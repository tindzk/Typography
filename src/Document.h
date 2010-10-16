#import <Array.h>
#import <String.h>

#import "Chapter.h"

record(Document) {
	String title;
	Array(Chapter *, *chapters);
};
