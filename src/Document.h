#import <Array.h>
#import <String.h>

#import "Chapter.h"

typedef struct {
	String title;
	Array(Chapter *, *chapters);
} Document;

void Document_Init(Document *this);
void Document_SetTitle(Document *this, String title);
Chapter* Document_AddChapter(Document *this, String title);
