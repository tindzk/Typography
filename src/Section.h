#import <String.h>

#import "Body.h"

#undef self
#define self Section

typedef struct {
	String title;
	Body body;
} self;

def(void, Init, String title);
