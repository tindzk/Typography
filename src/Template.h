#import <String.h>
#import <StringArray.h>

#import "Document.h"
#import "TemplateHelper.h"

#undef self
#define self Template

record(MainTemplate) {
	String base;
	Document *doc;
};

void Template_Main(MainTemplate *, String *);
