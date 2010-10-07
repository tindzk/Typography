#import <Bit.h>
#import <String.h>

#import "Utils.h"

#undef self
#define self Body

#ifndef Body_DefaultLength
#define Body_DefaultLength 128
#endif

typedef enum {
	ref(Styles_None)     = 0,
	ref(Styles_Bold)     = Bit(0),
	ref(Styles_Italic)   = Bit(1),
	ref(Styles_Class)    = Bit(2),
	ref(Styles_Function) = Bit(3),
	ref(Styles_Variable) = Bit(4),
	ref(Styles_Macro)    = Bit(5),
	ref(Styles_Term)     = Bit(6),
	ref(Styles_Keyword)  = Bit(7),
	ref(Styles_Path)     = Bit(8),
	ref(Styles_Number)   = Bit(9)
} ref(Style);

typedef enum {
	ref(BlockType_None),
	ref(BlockType_Note),
	ref(BlockType_Warning)
} ref(BlockType);

typedef enum {
	ref(EntryType_Collection),
	ref(EntryType_Block),
	ref(EntryType_Paragraph),
	ref(EntryType_List),
	ref(EntryType_ListItem),
	ref(EntryType_Url),
	ref(EntryType_Text),
	ref(EntryType_Image),
	ref(EntryType_Command),
	ref(EntryType_Code),
	ref(EntryType_Mail),
	ref(EntryType_Anchor),
	ref(EntryType_Jump)
} ref(EntryType);

typedef struct {
	String value;
	int style;
} ref(Text);

typedef struct {
	ref(BlockType) type;
} ref(Block);

typedef struct {
	String path;
} ref(Image);

typedef struct {
	String value;
} ref(Command);

typedef struct {
	String value;
} ref(Code);

typedef struct {
	String addr;
} ref(Mail);

typedef struct {
	String name;
} ref(Anchor);

typedef struct {
	String anchor;
} ref(Jump);

typedef struct {
	String url;
} ref(Url);

typedef struct ref(Entry) {
	ref(EntryType) type;

	union {
		ref(Url)     url;
		ref(Code)    code;
		ref(Mail)    mail;
		ref(Text)    text;
		ref(Jump)    jump;
		ref(Image)   image;
		ref(Block)   block;
		ref(Anchor)  anchor;
		ref(Command) command;
	} u;

	Array(struct ref(Entry) *, *entries);
	struct ref(Entry) *parent;
} ref(Entry);

typedef struct {
	ref(Entry) root;
	ref(Entry) *curEntry;
} self;

def(void, Init);
def(void, Enter);
def(void, Return);
def(void, SetBlock, ref(BlockType) type);
def(void, SetParagraph);
def(void, SetUrl, String url);
def(void, SetList);
def(void, SetListItem);
overload def(void, SetText, String text, int style);
overload def(void, SetText, String text);
def(void, SetCommand, String value);
def(void, SetCode, String value);
def(void, SetMail, String addr);
def(void, SetImage, String path);
def(void, SetAnchor, String name);
def(void, SetJump, String anchor);
