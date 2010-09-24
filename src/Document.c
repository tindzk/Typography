#import "Document.h"

void Document_Init(Document *this) {
	Array_Init(this->chapters, 0);
	this->title = HeapString(0);
}

void Document_SetTitle(Document *this, String title) {
	String_Copy(&this->title, title);
}

Chapter* Document_AddChapter(Document *this, String title) {
	Chapter *ch = New(Chapter);

	Chapter_Init(ch, title);

	Array_Push(this->chapters, ch);

	return ch;
}
