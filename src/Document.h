#import <Array.h>
#import <String.h>

#import "Chapter.h"

Array_Define(Chapter *, ChapterArray);

record(Document) {
	String title;
	ChapterArray *chapters;
};
