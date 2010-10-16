#import <Bit.h>
#import <String.h>

#undef self
#define self Body

#ifndef Body_DefaultLength
#define Body_DefaultLength 128
#endif

set {
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

set {
	ref(BlockType_None),
	ref(BlockType_Note),
	ref(BlockType_Warning)
} ref(BlockType);

record {
	String value;
	int style;
} ref(Text);

record {
	ref(BlockType) type;
} ref(Block);

record {
	String path;
} ref(Image);

record {
	String value;
} ref(Command);

record {
	String value;
} ref(Code);

record {
	String addr;
} ref(Mail);

record {
	String name;
} ref(Anchor);

record {
	String anchor;
} ref(Jump);

record {
	String url;
} ref(Url);

set {
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

record Body {
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
