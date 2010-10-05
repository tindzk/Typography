#import "Parser.h"

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
	for (size_t i = 0; i < sizeof(styles) / sizeof(styles[0]); i++) {
		if (String_Equals(name, styles[i].name)) {
			return styles[i].style;
		}
	}

	return Body_Styles_None;
}

Body_BlockType ref(ResolveBlock)(String name) {
	for (size_t i = 0; i < sizeof(blocks) / sizeof(blocks[0]); i++) {
		if (String_Equals(name, blocks[i].name)) {
			return blocks[i].block;
		}
	}

	return Body_BlockType_None;
}

def(void, Init) {
	Document_Init(&this->document);
}

def(void, Destroy) {

}

def(Document *, GetDocument) {
	return &this->document;
}

static String ref(GetValue)(Typography_Node *node) {
	if (node->len == 0) {
		Logger_LogFmt(&logger, Logger_Level_Error,
			$("line %: value expected."),
			Integer_ToString(node->line));

		return $("");
	}

	if (node->len > 1) {
		Logger_LogFmt(&logger, Logger_Level_Error,
			$("line %: too many nodes."),
			Integer_ToString(node->line));

		return $("");
	}

	Typography_Node *child = node->nodes[0];

	if (child->type != Typography_NodeType_Text) {
		Logger_LogFmt(&logger, Logger_Level_Error,
			$("line %: node given, text expected."),
			Integer_ToString(node->line));

		return $("");
	}

	return Typography_Text(child)->value;
}

static def(void, ParseMetaBlock, Typography_Node *node) {
	for (size_t i = 0; i < node->len; i++) {
		Typography_Node *child = node->nodes[i];

		if (child->type == Typography_NodeType_Item) {
			if (String_Equals(Typography_Item(child)->name, $("title"))) {
				Document_SetTitle(&this->document, ref(GetValue)(child));
			} else {
				Logger_LogFmt(&logger, Logger_Level_Error,
					$("line %: unknown meta tag '%'."),
					Integer_ToString(child->line),
					Typography_Item(child)->name);
			}
		}
	}
}

static def(void, ParseStyleBlock, Typography_Node *node, int style, Body *body);

static def(void, ParseList, Typography_Node *node, Body *body) {
	for (size_t i = 0; i < node->len; i++) {
		Typography_Node *child = node->nodes[i];

		if (child->type == Typography_NodeType_Item) {
			if (!String_Equals(Typography_Item(child)->name, $("item"))) {
				Logger_LogFmt(&logger, Logger_Level_Error,
					$("line %: got '%', 'item' expected."),
					Integer_ToString(child->line),
					Typography_Item(child)->name);

				continue;
			}

			Body_EnterListItem(body);

			/* TODO allow sub-lists */
			ref(ParseStyleBlock)(this, child, 0, body);
		}
	}
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

static def(void, ParseCommand, Typography_Node *child, Body *body) {
	String value = ref(GetValue)(child);
	Body_AddCommand(body, ref(CleanValue)(this, value));
}

static def(void, ParseCode, Typography_Node *child, Body *body) {
	String value = ref(GetValue)(child);
	Body_AddCode(body, ref(CleanValue)(this, value));
}

static def(void, ParseMail, Typography_Node *child, Body *body) {
	String caption = Typography_Item(child)->options;
	String addr    = String_Trim(ref(GetValue)(child));

	if (caption.len == 0) {
		caption = addr;
	}

	Body_AddMail(body, caption, addr);
}

static def(void, ParseAnchor, Typography_Node *child, Body *body) {
	Body_AddAnchor(body, String_Trim(ref(GetValue)(child)));
}

static def(void, ParseJump, Typography_Node *child, Body *body) {
	String caption = Typography_Item(child)->options;
	String anchor  = String_Trim(ref(GetValue)(child));

	if (caption.len == 0) {
		caption = anchor;
	}

	Body_AddJump(body, caption, anchor);
}

static def(void, ParseUrl, Typography_Node *child, Body *body) {
	String caption = Typography_Item(child)->options;
	String url     = String_Trim(ref(GetValue)(child));

	if (caption.len == 0) {
		caption = url;
	}

	Body_AddUrl(body, caption, url);
}

static def(void, ParseImage, Typography_Node *child, Body *body) {
	String path = String_Trim(ref(GetValue)(child));
	Body_AddImage(body, path);
}

static def(void, ParseItem, Typography_Node *child, int style, Body *body) {
	String name;
	Body_BlockType block;

	name = Typography_Item(child)->name;

	Body_Style _style;

	if ((_style = ref(ResolveStyle)(name)) != Body_Styles_None) {
		BitMask_Set(style, _style);
		ref(ParseStyleBlock)(this, child, style, body);
	} else if ((block = ref(ResolveBlock)(name)) != Body_BlockType_None) {
		Body_EnterBlock(body, block);
		ref(ParseStyleBlock)(this, child, style, body);
		Body_Return(body);
	} else if (String_Equals(name, $("list"))) {
		Body_EnterList(body);
		ref(ParseList)(this, child, body);
		Body_Return(body);
	} else if (String_Equals(name, $("p"))) {
		Body_EnterParagraph(body);
		ref(ParseStyleBlock)(this, child, style, body);
		Body_Return(body);
	} else if (String_Equals(name, $("command"))) {
		ref(ParseCommand)(this, child, body);
	} else if (String_Equals(name, $("code"))) {
		ref(ParseCode)(this, child, body);
	} else if (String_Equals(name, $("mail"))) {
		ref(ParseMail)(this, child, body);
	} else if (String_Equals(name, $("anchor"))) {
		ref(ParseAnchor)(this, child, body);
	} else if (String_Equals(name, $("jump"))) {
		ref(ParseJump)(this, child, body);
	} else if (String_Equals(name, $("url"))) {
		ref(ParseUrl)(this, child, body);
	} else if (String_Equals(name, $("image"))) {
		ref(ParseImage)(this, child, body);
	} else {
		Logger_LogFmt(&logger, Logger_Level_Error,
			$("line %: '%' not understood."),
			Integer_ToString(child->line),
			Typography_Item(child)->name);
	}
}

static def(void, AddText, String text, int style, Body *body) {
	if (style != 0) {
		Body_AddText(body, text, style);
		return;
	}

	size_t last = 0;

	for (;;) {
		ssize_t pos = String_Find(text, last, $("\n\n"));

		if (pos == String_NotFound) {
			Body_AddText(body, String_Slice(text, last));
			break;
		} else {
			Body_AddText(body, String_Slice(text, last, pos - last));
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
		Typography_Node *child = node->nodes[i];

		if (child->type == Typography_NodeType_Text) {
			String text = ref(CleanText)(this,
				Typography_Text(child)->value);

			if (i == 0) {
				text = String_Trim(text, String_TrimLeft);
			} else if (i == node->len - 1) {
				text = String_Trim(text, String_TrimRight);
			}

			ref(AddText)(this, text, style, body);
		} else if (child->type == Typography_NodeType_Item) {
			ref(ParseItem)(this, child, style, body);
		}
	}
}

static def(void, ParseSectionBlock, String title, Typography_Node *node) {
	Section *sect = Chapter_AddSection(this->cur.chapter, title);

	this->cur.section = sect;

	for (size_t i = 0; i < node->len; i++) {
		Typography_Node *child = node->nodes[i];

		if (child->type == Typography_NodeType_Text) {
			String text = ref(CleanText)(this,
				Typography_Text(child)->value);

			if (i == 0) {
				text = String_Trim(text, String_TrimLeft);
			} else if (i == node->len - 1) {
				text = String_Trim(text, String_TrimRight);
			}

			ref(AddText)(this, text, 0, &sect->body);
		} else if (child->type == Typography_NodeType_Item) {
			ref(ParseItem)(this, child, 0, &sect->body);
		}
	}
}

static def(void, ParseChapterBlock, String title, Typography_Node *node) {
	Chapter *ch = Document_AddChapter(&this->document, title);

	this->cur.chapter = ch;

	bool introductoryText = true;

	for (size_t i = 0; i < node->len; i++) {
		Typography_Node *child = node->nodes[i];

		if (child->type == Typography_NodeType_Text) {
			if (!introductoryText) {
				if (String_Trim(Typography_Text(child)->value).len > 0) {
					Logger_LogFmt(&logger, Logger_Level_Error,
						$("line %: text between sections is ignored."),
						Integer_ToString(child->line));
				}

				continue;
			}

			String text = ref(CleanText)(this,
				Typography_Text(child)->value);

			if (i == 0) {
				text = String_Trim(text, String_TrimLeft);
			} else if (i == node->len - 1) {
				text = String_Trim(text, String_TrimRight);
			}

			ref(AddText)(this, text, 0, &ch->body);
		} else if (child->type == Typography_NodeType_Item) {
			if (String_Equals(Typography_Item(child)->name, $("section"))) {
				ref(ParseSectionBlock)(this, Typography_Item(child)->options, child);
				introductoryText = false;
			} else {
				ref(ParseItem)(this, child, 0, &ch->body);
			}
		}
	}
}

def(void, Parse, Typography_Node *node) {
	for (size_t i = 0; i < node->len; i++) {
		Typography_Node *child = node->nodes[i];

		if (child->type == Typography_NodeType_Item) {
			if (String_Equals(Typography_Item(child)->name, $("meta"))) {
				ref(ParseMetaBlock)(this, child);
			} else if (String_Equals(Typography_Item(child)->name, $("chapter"))) {
				ref(ParseChapterBlock)(this, Typography_Item(child)->options, child);
			} else {
				Logger_LogFmt(&logger, Logger_Level_Error,
					$("line %: '%' not understood."),
					Integer_ToString(child->line),
					Typography_Item(child)->name);
			}
		}
	}
}
