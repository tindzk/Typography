#import "Document.h"
#import <docslib/TemplateHelper.h>

tpl(Main) {
	Document *doc;
	RdString base;
};

#define getNumber(name, arr) \
	(size_t) ((name) - (arr)->buf + 1)
