#import <Array.h>
#import <String.h>

#import "Chapter.h"

record(Document) {
	RdString     title;
	RdString     subtitle;
	RdString     author;
	RdString     toc;
	ChapterArray *chapters;
	BodyArray    *footnotes;
};
