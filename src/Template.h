#import <String.h>
#import <StringArray.h>

#import "Document.h"
#import "TemplateHelper.h"

#undef self
#define self Template

typedef struct {
	String base;
	Document *doc;
} MainTemplate;

void Template_Main(MainTemplate *, String *);
