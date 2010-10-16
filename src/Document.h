#import <Array.h>
#import <String.h>

#import "Chapter.h"

record {
	String title;
	Array(Chapter *, *chapters);
} Document;
