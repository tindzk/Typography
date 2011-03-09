#import <Path.h>
#import <File.h>
#import <Signal.h>
#import <Terminal.h>
#import <Exception.h>
#import <Typography.h>
#import <docslib/Parser.h>

#import "Document.h"
#import "Plugins/HTML.h"

Logger logger;
Terminal term;

void OnLogMessage(__unused void *ptr, FmtString msg, Logger_Level level, __unused String file, __unused int line) {
	Terminal_FmtPrint(&term, $("[%] $\n"),
		Logger_ResolveLevel(level), msg);
}

int main(int argc, char *argv[]) {
	Signal0();

	term = Terminal_New(File_StdIn, File_StdOut, false);

	Logger_Init(&logger, Callback(NULL, &OnLogMessage),
		Logger_Level_Fatal |
		Logger_Level_Crit  |
		Logger_Level_Error |
		Logger_Level_Warn  |
		Logger_Level_Info  |
		Logger_Level_Trace);

	if (argc <= 1) {
		Logger_Error(&logger, $("No parameters specified."));
		return ExitStatus_Failure;
	}

	ProtString filename = String_FromNul(argv[1]);

	if (!Path_Exists(filename)) {
		Logger_Error(&logger, $("Path '%' does not exist."),
			filename);

		return ExitStatus_Failure;
	}

	ProtString base = $(".");

	if (argc > 2) {
		base = String_FromNul(argv[2]);
	}

	Parser parser = Parser_New();
	Parser_Parse(&parser, filename);

	try {
		Document doc = {
			.chapters = ChapterArray_New(0),
			.title    = Parser_GetMeta(&parser, $("title")),
			.subtitle = Parser_GetMeta(&parser, $("subtitle")),
			.author   = Parser_GetMeta(&parser, $("author")),
			.toc      = Parser_GetMeta(&parser, $("toc"))
		};

		Parser_Nodes *nodes = Parser_GetNodes(&parser, Parser_GetRoot(&parser));

		foreach (node, nodes) {
			if (!String_Equals(node->name, $("chapter"))) {
				continue;
			}

			Chapter *ch = Pool_Alloc(Pool_GetInstance(), sizeof(Chapter));

			ch->title    = node->options;
			ch->sections = SectionArray_New(0);
			ch->body     = Parser_GetBody(&parser, node->node, $("section"));

			Parser_Nodes *children = Parser_GetNodes(&parser, node->node);

			foreach (child, children) {
				if (!String_Equals(child->name, $("section"))) {
					continue;
				}

				Section *sect = Pool_Alloc(Pool_GetInstance(), sizeof(Section));

				sect->title = child->options;
				sect->body  = Parser_GetBody(&parser, child->node, $(""));

				SectionArray_Push(&ch->sections, sect);
			}

			Parser_Nodes_Free(children);

			ChapterArray_Push(&doc.chapters, ch);
		}

		Parser_Nodes_Free(nodes);

		Plugins_HTML(base, &doc, File_StdOut);

		foreach (ch, doc.chapters) {
			foreach (sect, (*ch)->sections) {
				Body_Destroy(&(*sect)->body);
				Pool_Free(Pool_GetInstance(), *sect);
			}

			SectionArray_Free((*ch)->sections);
			Body_Destroy(&(*ch)->body);
			Pool_Free(Pool_GetInstance(), *ch);
		}

		ChapterArray_Free(doc.chapters);
	} catchAny {
		Exception_Print(e);
		excReturn ExitStatus_Failure;
	} finally {
		Parser_Destroy(&parser);
		Terminal_Destroy(&term);
	} tryEnd;

	return ExitStatus_Success;
}
