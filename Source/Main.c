#import <Main.h>
#import <Path.h>
#import <File.h>
#import <docslib/Parser.h>

#import "Document.h"
#import "Plugins/HTML.h"

#define self Application

/* Use our own method for printing out log messages (less verbose). */
def(void, OnLogMessage, FmtString msg, Logger_Level level, __unused RdString file, __unused int line) {
	Terminal_SetOutput(&this->term,
		level == Logger_Level_Error
			? File_StdErr
			: File_StdOut);

	Terminal_FmtPrint(&this->term, $("[%] $\n"),
		Logger_ResolveLevel(level), msg);

	Terminal_SetOutput(&this->term, File_StdOut);
}

def(void, Parse, RdString base, RdString filename) {
	Parser parser = Parser_New(&this->logger);
	Parser_Parse(&parser, filename);

	Document doc = {
		.chapters  = ChapterArray_New(0),
		.title     = Parser_GetMeta(&parser, $("title")),
		.subtitle  = Parser_GetMeta(&parser, $("subtitle")),
		.author    = Parser_GetMeta(&parser, $("author")),
		.toc       = Parser_GetMeta(&parser, $("toc")),
		.footnotes = Parser_GetFootnotes(&parser)
	};

	if (doc.title.len == 0) {
		doc.title = filename;
	}

	Parser_Nodes *nodes = Parser_GetNodes(&parser, Parser_GetRoot(&parser));

	each(node, nodes) {
		if (!String_Equals(node->name, $("chapter"))) {
			continue;
		}

		Chapter *ch = Chapter_Alloc();

		ch->title    = node->options;
		ch->sections = SectionArray_New(0);
		ch->body     = Parser_GetBody(&parser, node->node, $("section"));

		Parser_Nodes *children = Parser_GetNodes(&parser, node->node);

		each(child, children) {
			if (!String_Equals(child->name, $("section"))) {
				continue;
			}

			Section *sect = Section_Alloc();

			sect->title = child->options;
			sect->body  = Parser_GetBody(&parser, child->node, $(""));

			SectionArray_Push(&ch->sections, sect);
		}

		Parser_Nodes_Free(children);

		ChapterArray_Push(&doc.chapters, ch);
	}

	Parser_Nodes_Free(nodes);

	Plugins_HTML(base, &doc, File_StdOut);

	each(ch, doc.chapters) {
		each(sect, (*ch)->sections) {
			Body_Destroy(&(*sect)->body);
			Section_Free(*sect);
		}

		SectionArray_Free((*ch)->sections);
		Body_Destroy(&(*ch)->body);
		Chapter_Free(*ch);
	}

	ChapterArray_Free(doc.chapters);

	Parser_Destroy(&parser);
}

def(bool, Run) {
	if (this->args->len == 0) {
		Logger_Error(&this->logger, $("No parameters specified."));
		return false;
	}

	RdString filename = this->args->buf[0];

	if (!Path_Exists(filename)) {
		Logger_Error(&this->logger, $("Path '%' does not exist."), filename);
		return false;
	}

	RdString base =
		(this->args->len > 1)
			? this->args->buf[1]
			: $(".");

	call(Parse, base, filename);

	return true;
}
