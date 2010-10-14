#import <String.h>

#import "Body.h"
#import "Section.h"

typedef struct {
	String title;
	Body body;
	Array(Section *, *sections);
} Chapter;
