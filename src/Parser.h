#import <String.h>
#import <Logger.h>
#import <Typography.h>

#import "Utils.h"

#import "Body.h"
#import "Chapter.h"
#import "Section.h"
#import "Document.h"

#undef self
#define self Parser

typedef struct {
	Document document;

	struct {
		Chapter *chapter;
		Section *section;
		Body    *body;
	} cur;
} Class(self);

def(void, Init);
def(void, Destroy);
def(Document *, GetDocument);
def(void, Parse, Typography_Node *node);
