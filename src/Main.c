#import <Path.h>
#import <File.h>
#import <Signal.h>
#import <Exception.h>
#import <Typography.h>

#import "Parser.h"
#import "Document.h"
#import "Plugins/HTML.h"

Logger logger;
ExceptionManager exc;

void OnLogMessage(__unused void *ptr, String msg, Logger_Level level, __unused String file, __unused int line) {
	String fmt = String_Format($("[%] %\n"),
		Logger_ResolveLevel(level), msg);

	String_Print(fmt, true);

	String_Destroy(&fmt);
}

int main(int argc, char *argv[]) {
	ExceptionManager_Init(&exc);

	Path0(&exc);
	File0(&exc);
	Memory0(&exc);
	String0(&exc);
	Signal0(&exc);
	Integer0(&exc);
	Typography0(&exc);

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

	String filename = String_FromNul(argv[1]);

	if (!Path_Exists(filename)) {
		Logger_Error(&logger, $("Path '%' does not exist."),
			filename);

		return ExitStatus_Failure;
	}

	String base = $(".");

	if (argc > 2) {
		base = String_FromNul(argv[2]);
	}

	Parser parser;
	Parser_Init(&parser);
	Parser_Parse(&parser, filename);

	try (&exc) {
		Document doc = {
			.chapters = ChapterArray_New(0),
			.title    = Parser_GetMeta(&parser, $("title"))
		};

		Parser_Nodes *nodes = Parser_GetNodes(&parser, Parser_GetRoot(&parser));

		foreach (node, nodes) {
			if (!String_Equals(node->name, $("chapter"))) {
				continue;
			}

			Chapter *ch = New(Chapter);

			ch->title    = node->options;
			ch->sections = SectionArray_New(0);
			ch->body     = Parser_GetBody(&parser, node->node, $("section"));

			Parser_Nodes *children = Parser_GetNodes(&parser, node->node);

			foreach (child, children) {
				if (!String_Equals(child->name, $("section"))) {
					continue;
				}

				Section *sect = New(Section);

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
				Memory_Free(*sect);
			}

			SectionArray_Free((*ch)->sections);
			Body_Destroy(&(*ch)->body);
			Memory_Free(*ch);
		}

		ChapterArray_Free(doc.chapters);
	} clean catchAny {
		ExceptionManager_Print(&exc, e);

#if Exception_SaveTrace
		Backtrace_PrintTrace(exc.e.trace, exc.e.traceItems);
#endif

		excReturn ExitStatus_Failure;
	} finally {
		Parser_Destroy(&parser);
	} tryEnd;

	return ExitStatus_Success;
}
