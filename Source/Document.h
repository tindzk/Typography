#import <Array.h>
#import <String.h>

#import "Chapter.h"

record(Document) {
	ProtString   title;
	ChapterArray *chapters;
};
