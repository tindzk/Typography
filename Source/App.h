#import <Logger.h>
#import <Domain/Parser.h>

#import "Document.h"
#import "Plugins/HTML.h"

#define self App

class {
	Chapter  *ch;
	Document doc;
	Logger   *logger;
	Parser   parser;
};

rsdef(self, New, Logger *logger);
def(void, Destroy);
def(void, Parse, RdString base, RdString filename);

#undef self
