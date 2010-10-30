#import <File.h>
#import <String.h>
#import <Logger.h>
#import <Integer.h>
#import <Typography.h>
#import <FileStream.h>
#import <BufferedStream.h>

#import "Body.h"
#import "Chapter.h"
#import "Section.h"

#undef self
#define self Parser

class(self) {
	Typography tyo;

	struct {
		Chapter *chapter;
		Section *section;
		Body    *body;
	} cur;
};

ExtendClass(self);

def(void, Init, String path);
def(void, Destroy);
def(String, GetMeta, String name);
def(ChapterArray *, GetChapters);
