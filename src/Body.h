#import <Bit.h>
#import <String.h>

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

typedef enum {
	ref(Type_Collection),
	ref(Type_Block),
	ref(Type_Paragraph),
	ref(Type_List),
	ref(Type_ListItem),
	ref(Type_Url),
	ref(Type_Text),
	ref(Type_Image),
	ref(Type_Command),
	ref(Type_Code),
	ref(Type_Mail),
	ref(Type_Anchor),
	ref(Type_Jump)
} ref(Type);

typedef struct Body {
	ref(Type) type;

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

	Array(struct Body *, *nodes);
	struct Body *parent;
} Body;
