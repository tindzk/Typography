#import <Array.h>
#import <String.h>

#import "Chapter.h"

typedef struct {
	String title;
	Array(Chapter *, *chapters);
} Document;
