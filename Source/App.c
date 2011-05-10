#import "App.h"

#define self App

rsdef(self, New, Logger *logger) {
	return (self) {
		.ch     = NULL,
		.logger = logger,
		.parser = Parser_New(logger)
	};
}

def(void, Destroy) {
	each(ch, this->doc.chapters) {
		Chapter_Destroy(*ch);
	}

	each(sect, this->doc.sections) {
		Section_Destroy(*sect);
	}

	ChapterArray_Free(this->doc.chapters);
	SectionArray_Free(this->doc.sections);

	Parser_Destroy(&this->parser);
}

static def(void, OnSection, Parser_Node *node) {
	Section *sect = Section_New(node->options);
	Section_SetBody(sect, Parser_ProcessBody(&this->parser, node->node, NULL));

	if (this->ch == NULL) {
		SectionArray_Push(&this->doc.sections, sect);
	} else {
		SectionArray_Push(&this->ch->sections, sect);
	}
}

static def(void, OnChapter, Parser_Node *node) {
	this->ch = Chapter_New(node->options);

	Chapter_SetBody(this->ch,
		Parser_ProcessBody(&this->parser, node->node, (Parser_Handler[]) {
			{ $("section"), Parser_OnNode_For(this, ref(OnSection)) },
			{ .name = $("") } /* Sentinel. */
		})
	);

	ChapterArray_Push(&this->doc.chapters, this->ch);
}

def(void, Parse, RdString base, RdString filename) {
	Parser_SetAutoDetectParagraphs(&this->parser, true);
	Parser_Parse(&this->parser, filename);

	this->doc = (Document) {
		.chapters  = ChapterArray_New(0),
		.sections  = SectionArray_New(0),
		.title     = Parser_GetMeta(&this->parser, $("title")),
		.subtitle  = Parser_GetMeta(&this->parser, $("subtitle")),
		.author    = Parser_GetMeta(&this->parser, $("author")),
		.toc       = Parser_GetMeta(&this->parser, $("toc")),
		.math      = String_Equals(Parser_GetMeta(&this->parser, $("math")), $("yes")),
		.footnotes = Parser_GetFootnotes(&this->parser)
	};

	if (this->doc.title.len == 0) {
		this->doc.title = filename;
	}

	Parser_ProcessNodes(&this->parser, Parser_GetRoot(&this->parser), (Parser_Handler[]) {
		{ $("chapter"), Parser_OnNode_For(this, ref(OnChapter)) },
		{ $("section"), Parser_OnNode_For(this, ref(OnSection)) },
		{ .name = $("") }
	});

	Plugins_HTML(base, &this->doc, Channel_StdOut);
}
