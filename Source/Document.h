#import <Array.h>
#import <String.h>

#import "Chapter.h"

record(Document) {
	ProtString   title;
	ProtString   subtitle;
	ProtString   author;
	ProtString   toc;
	ChapterArray *chapters;
};
