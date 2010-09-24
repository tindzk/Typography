#import <String.h>

#import "Body.h"
#import "Section.h"

#undef self
#define self Chapter

typedef struct {
	String title;
	Body body;
	Array(Section *, *sections);
} self;

def(void, Init, String title);
def(Section *, AddSection, String title);
