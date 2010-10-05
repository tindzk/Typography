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
	ref(ItemType_Text),
	ref(ItemType_Image),
	ref(ItemType_Command),
	ref(ItemType_Code),
	ref(ItemType_Mail),
	ref(ItemType_Anchor),
	ref(ItemType_Jump),
	ref(ItemType_Url)
} ref(ItemType);

typedef enum {
	ref(EntryType_Items),
	ref(EntryType_Block),
	ref(EntryType_Paragraph),
	ref(EntryType_List),
} ref(EntryType);

typedef struct {
	String value;
	int style;
} ref(Text);

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
	String caption;
	String addr;
} ref(Mail);

typedef struct {
	String name;
} ref(Anchor);

typedef struct {
	String caption;
	String anchor;
} ref(Jump);

typedef struct {
	String caption;
	String url;
} ref(Url);

typedef struct {
	ref(ItemType) type;

	union {
		ref(Url)     url;
		ref(Text)    text;
		ref(Image)   image;
		ref(Code)    code;
		ref(Mail)    mail;
		ref(Anchor)  anchor;
		ref(Jump)    jump;
		ref(Command) cmd;
	} u;
} ref(Item);

typedef struct {
	Array(ref(Item), *items);
} ref(ListItem);

typedef struct {
	ref(EntryType) type;

	Array(ref(Item), *items);

	union {
		ref(BlockType)       block;
		Array(ref(ListItem), *list);
	} u;
} ref(Entry);

typedef struct {
	Array(ref(Entry), *items);
} self;

def(void, Init);
def(void, EnterBlock, ref(BlockType) type);
def(void, EnterParagraph);
def(void, EnterList);
def(void, EnterListItem);
def(void, Return);
overload def(void, AddText, String text, int style);
overload def(void, AddText, String text);
def(void, AddCommand, String value);
def(void, AddCode, String value);
def(void, AddMail, String caption, String addr);
def(void, AddImage, String path);
def(void, AddAnchor, String name);
def(void, AddJump, String caption, String anchor);
def(void, AddUrl, String caption, String url);
