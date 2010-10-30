#import "Parser.h"
#import <App.h>

extern Logger logger;

static struct {
	String     name;
	Body_Style style;
} styles[] = {
	{ $("b"),       Body_Styles_Bold     },
	{ $("i"),       Body_Styles_Italic   },
	{ $("class"),   Body_Styles_Class    },
	{ $("func"),    Body_Styles_Function },
	{ $("var"),     Body_Styles_Variable },
	{ $("macro"),   Body_Styles_Macro    },
	{ $("term"),    Body_Styles_Term     },
	{ $("keyword"), Body_Styles_Keyword  },
	{ $("path"),    Body_Styles_Path     },
	{ $("number"),  Body_Styles_Number   }
};

static struct {
	String         name;
	Body_BlockType block;
} blocks[] = {
	{ $("note"),    Body_BlockType_Note    },
	{ $("warning"), Body_BlockType_Warning }
};

Body_Style ref(ResolveStyle)(String name) {
	for (size_t i = 0; i < nElems(styles); i++) {
		if (String_Equals(name, styles[i].name)) {
			return styles[i].style;
		}
	}

	return Body_Styles_None;
}

Body_BlockType ref(ResolveBlock)(String name) {
	for (size_t i = 0; i < nElems(blocks); i++) {
		if (String_Equals(name, blocks[i].name)) {
			return blocks[i].block;
		}
	}

	return Body_BlockType_None;
}

def(void, Init) {
	this->document.chapters = ChapterArray_New(0);
	this->document.title    = HeapString(0);
}

def(void, Destroy) {

}

def(Document *, GetDocument) {
	return &this->document;
}

static def(String, GetValue, Typography_Node *node) {
	if (node->len == 0) {
		Logger_Error(&logger, $("line %: value expected."),
			Integer_ToString(node->line));

		return $("");
	}

	if (node->len > 1) {
		Logger_Error(&logger, $("line %: too many nodes."),
			Integer_ToString(node->line));

		return $("");
	}

	Typography_Node *child = node->buf[0];

	if (child->type != Typography_NodeType_Text) {
		Logger_Error(&logger,
			$("line %: node given, text expected."),
			Integer_ToString(node->line));

		return $("");
	}

	return Typography_Text(child)->value;
}

static def(void, ParseMetaBlock, Typography_Node *node) {
	for (size_t i = 0; i < node->len; i++) {
		Typography_Node *child = node->buf[i];

		if (child->type == Typography_NodeType_Item) {
			if (String_Equals(Typography_Item(child)->name, $("title"))) {
				String_Copy(&this->document.title, call(GetValue, child));
			} else {
				Logger_Error(&logger,
					$("line %: unknown meta tag '%'."),
					Integer_ToString(child->line),
					Typography_Item(child)->name);
			}
		}
	}
}

static def(void, InitBody, Body *body) {
	body->type   = Body_Type_Collection;
	body->parent = NULL;
	body->nodes  = BodyArray_New(Body_DefaultLength);

	this->cur.body = body;
}

static def(void, Enter) {
	Body *body = New(Body);

	body->type   = Body_Type_Collection;
	body->parent = this->cur.body;
	body->nodes  = BodyArray_New(Body_DefaultLength);

	BodyArray_Push(&this->cur.body->nodes, body);

	this->cur.body =
		this->cur.body->nodes->buf[this->cur.body->nodes->len - 1];
}

static def(void, Return) {
	this->cur.body = this->cur.body->parent;
}

static def(void, SetBlock, Body_BlockType type) {
	this->cur.body->type       = Body_Type_Block;
	this->cur.body->block.type = type;
}

static def(void, SetParagraph) {
	this->cur.body->type = Body_Type_Paragraph;
}

static def(void, SetUrl, String url) {
	this->cur.body->type    = Body_Type_Url;
	this->cur.body->url.url = String_Clone(url);
}

static def(void, SetList) {
	this->cur.body->type = Body_Type_List;
}

static def(void, SetListItem) {
	this->cur.body->type = Body_Type_ListItem;
}

static def(void, SetText, String text, int style) {
	this->cur.body->type       = Body_Type_Text;
	this->cur.body->text.value = String_Clone(text);
	this->cur.body->text.style = style;
}

static def(void, SetCommand, String value) {
	this->cur.body->type          = Body_Type_Command;
	this->cur.body->command.value = String_Clone(value);
}

static def(void, SetCode, String value) {
	this->cur.body->type       = Body_Type_Code;
	this->cur.body->code.value = String_Clone(value);
}

static def(void, SetMail, String addr) {
	this->cur.body->type      = Body_Type_Mail;
	this->cur.body->mail.addr = String_Clone(addr);
}

static def(void, SetImage, String path) {
	this->cur.body->type       = Body_Type_Image;
	this->cur.body->image.path = String_Clone(path);
}

static def(void, SetAnchor, String name) {
	this->cur.body->type        = Body_Type_Anchor;
	this->cur.body->anchor.name = String_Clone(name);
}

static def(void, SetJump, String anchor) {
	this->cur.body->type        = Body_Type_Jump;
	this->cur.body->jump.anchor = String_Clone(anchor);
}

static def(void, ParseStyleBlock, Typography_Node *node, int style, Body *body);

static def(void, ParseList, Typography_Node *node, Body *body) {
	call(Enter);
	call(SetList);

	for (size_t i = 0; i < node->len; i++) {
		Typography_Node *child = node->buf[i];

		if (child->type == Typography_NodeType_Item) {
			if (!String_Equals(Typography_Item(child)->name, $("item"))) {
				Logger_Error(&logger,
					$("line %: got '%', 'item' expected."),
					Integer_ToString(child->line),
					Typography_Item(child)->name);

				continue;
			}

			call(Enter);
			call(SetListItem);

			call(ParseStyleBlock, child, 0, body);

			call(Return);
		}
	}

	call(Return);
}

static def(String, CleanValue, String value) {
	String out = value;

	ssize_t pos = String_Find(value, '\n');

	if (pos != String_NotFound) {
		String firstLine = String_Slice(value, 0, pos);

		/* Skip first line if empty. */
		if (String_Trim(firstLine).len == 0) {
			out = String_Slice(value, pos + 1);
		}
	}

	return String_Trim(out, String_TrimRight);
}

static def(void, ParseCommand, Typography_Node *child) {
	String value   = call(GetValue,   child);
	String cleaned = call(CleanValue, value);

	call(Enter);
	call(SetCommand, cleaned);
	call(Return);
}

static def(void, ParseCode, Typography_Node *child) {
	String value   = call(GetValue,   child);
	String cleaned = call(CleanValue, value);

	call(Enter);
	call(SetCode, cleaned);
	call(Return);
}

static def(void, ParseMail, Typography_Node *child, Body *body) {
	String addr = String_Trim(Typography_Item(child)->options);

	call(Enter);
	call(SetMail, addr);

	call(ParseStyleBlock, child, 0, body);

	call(Return);
}

static def(void, ParseAnchor, Typography_Node *child) {
	String value = String_Trim(call(GetValue, child));

	call(Enter);
	call(SetAnchor, value);
	call(Return);
}

static def(void, ParseJump, Typography_Node *child, Body *body) {
	String anchor = String_Trim(Typography_Item(child)->options);

	call(Enter);
	call(SetJump, anchor);

	call(ParseStyleBlock, child, 0, body);

	call(Return);
}

static def(void, ParseUrl, Typography_Node *child, Body *body) {
	String url = String_Trim(Typography_Item(child)->options);

	call(Enter);
	call(SetUrl, url);

	call(ParseStyleBlock, child, 0, body);

	call(Return);
}

static def(void, ParseImage, Typography_Node *child) {
	String path = String_Trim(call(GetValue, child));

	call(Enter);
	call(SetImage, path);
	call(Return);
}

static def(void, ParseParagraph, Typography_Node *child, Body *body) {
	call(Enter);
	call(SetParagraph);

	call(ParseStyleBlock, child, 0, body);

	call(Return);
}

static def(void, ParseBlock, Body_BlockType type, Typography_Node *child, Body *body) {
	call(Enter);
	call(SetBlock, type);

	call(ParseStyleBlock, child, 0, body);

	call(Return);
}

static def(void, ParseItem, Typography_Node *child, int style, Body *body) {
	Body_Style _style;
	Body_BlockType type;

	String name = Typography_Item(child)->name;

	if ((_style = ref(ResolveStyle)(name)) != Body_Styles_None) {
		BitMask_Set(style, _style);
		call(ParseStyleBlock, child, style, body);
	} else if ((type = ref(ResolveBlock)(name)) != Body_BlockType_None) {
		call(ParseBlock, type, child, body);
	} else if (String_Equals(name, $("list"))) {
		call(ParseList, child, body);
	} else if (String_Equals(name, $("p"))) {
		call(ParseParagraph, child, body);
	} else if (String_Equals(name, $("url"))) {
		call(ParseUrl, child, body);
	} else if (String_Equals(name, $("command"))) {
		call(ParseCommand, child);
	} else if (String_Equals(name, $("code"))) {
		call(ParseCode, child);
	} else if (String_Equals(name, $("mail"))) {
		call(ParseMail, child, body);
	} else if (String_Equals(name, $("anchor"))) {
		call(ParseAnchor, child);
	} else if (String_Equals(name, $("jump"))) {
		call(ParseJump, child, body);
	} else if (String_Equals(name, $("image"))) {
		call(ParseImage, child);
	} else {
		Logger_Error(&logger,
			$("line %: '%' not understood."),
			Integer_ToString(child->line),
			Typography_Item(child)->name);
	}
}

static def(void, AddText, String text, int style) {
	if (style != 0) {
		call(Enter);
		call(SetText, text, style);
		call(Return);

		return;
	}

	size_t last = 0;

	for (;;) {
		ssize_t pos = String_Find(text, last, $("\n\n"));

		if (pos == String_NotFound) {
			call(Enter);
			call(SetText, String_Slice(text, last), 0);
			call(Return);

			break;
		} else {
			call(Enter);
			call(SetText, String_Slice(text, last, pos - last), 0);
			call(Return);
		}

		last = pos + 1;
	}
}

static def(String, CleanText, String value) {
	String text = String_ReplaceAll(value, $("\n"), $(" "));

	while(String_ReplaceAll(&text, $("\t"), $(" ")));
	while(String_ReplaceAll(&text, $("  "), $(" ")));

	return text;
}

static def(void, ParseStyleBlock, Typography_Node *node, int style, Body *body) {
	for (size_t i = 0; i < node->len; i++) {
		Typography_Node *child = node->buf[i];

		if (child->type == Typography_NodeType_Text) {
			String text = call(CleanText,
				Typography_Text(child)->value);

			if (i == 0) {
				text = String_Trim(text, String_TrimLeft);
			} else if (i == node->len - 1) {
				text = String_Trim(text, String_TrimRight);
			}

			call(AddText, text, style);
		} else if (child->type == Typography_NodeType_Item) {
			call(ParseItem, child, style, body);
		}
	}
}

static def(void, ParseSectionBlock, String title, Typography_Node *node) {
	Section *sect = New(Section);
	sect->title = String_Clone(title);
	call(InitBody, &sect->body);
	SectionArray_Push(&this->cur.chapter->sections, sect);

	this->cur.section = sect;

	for (size_t i = 0; i < node->len; i++) {
		Typography_Node *child = node->buf[i];

		if (child->type == Typography_NodeType_Text) {
			String text = call(CleanText,
				Typography_Text(child)->value);

			if (i == 0) {
				text = String_Trim(text, String_TrimLeft);
			} else if (i == node->len - 1) {
				text = String_Trim(text, String_TrimRight);
			}

			call(AddText, text, 0);
		} else if (child->type == Typography_NodeType_Item) {
			call(ParseItem, child, 0, &sect->body);
		}
	}
}

static def(void, ParseChapterBlock, String title, Typography_Node *node) {
	Chapter *ch = New(Chapter);

	ch->title    = String_Clone(title);
	ch->sections = SectionArray_New(0);

	call(InitBody, &ch->body);

	ChapterArray_Push(&this->document.chapters, ch);

	this->cur.chapter = ch;

	bool introductoryText = true;

	for (size_t i = 0; i < node->len; i++) {
		Typography_Node *child = node->buf[i];

		if (child->type == Typography_NodeType_Text) {
			if (!introductoryText) {
				if (String_Trim(Typography_Text(child)->value).len > 0) {
					Logger_Error(&logger,
						$("line %: text between sections is ignored."),
						Integer_ToString(child->line));
				}

				continue;
			}

			String text = call(CleanText,
				Typography_Text(child)->value);

			if (i == 0) {
				text = String_Trim(text, String_TrimLeft);
			} else if (i == node->len - 1) {
				text = String_Trim(text, String_TrimRight);
			}

			call(AddText, text, 0);
		} else if (child->type == Typography_NodeType_Item) {
			if (String_Equals(Typography_Item(child)->name, $("section"))) {
				call(ParseSectionBlock, Typography_Item(child)->options, child);
				introductoryText = false;
			} else {
				call(ParseItem, child, 0, &ch->body);
			}
		}
	}
}

def(void, Parse, Typography_Node *node) {
	for (size_t i = 0; i < node->len; i++) {
		Typography_Node *child = node->buf[i];

		if (child->type == Typography_NodeType_Item) {
			if (String_Equals(Typography_Item(child)->name, $("meta"))) {
				call(ParseMetaBlock, child);
			} else if (String_Equals(Typography_Item(child)->name, $("chapter"))) {
				call(ParseChapterBlock, Typography_Item(child)->options, child);
			} else {
				Logger_Error(&logger,
					$("line %: '%' not understood."),
					Integer_ToString(child->line),
					Typography_Item(child)->name);
			}
		}
	}
}
