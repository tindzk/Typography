#import <Path.h>
#import <File.h>
#import <Signal.h>
#import <Exception.h>
#import <Typography.h>
#import <FileStream.h>
#import <BufferedStream.h>

#import "Parser.h"
#import "Plugins/HTML.h"

Logger logger;
ExceptionManager exc;

void OnLogMessage(__unused void *ptr, String msg, Logger_Level level, __unused String file, __unused int line) {
	String fmt = String_Format($("[%] %\n"),
		Logger_LevelToString(level), msg);

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

	Logger_Init(&logger, &OnLogMessage, NULL,
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

	Typography tyo;

	File file;
	BufferedStream stream;

	FileStream_Open(&file, filename, FileStatus_ReadOnly);

	BufferedStream_Init(&stream, &FileStream_Methods, &file);
	BufferedStream_SetInputBuffer(&stream, 1024, 128);

	Typography_Init(&tyo, &BufferedStream_Methods, &stream);

	ParserInstance parser = Parser_NewStack();
	Parser_Init(parser);

	try (&exc) {
		Typography_Parse(&tyo);
		Parser_Parse(parser, Typography_GetRoot(&tyo));
		Plugins_HTML(base, Parser_GetDocument(parser), File_StdOut);
	} clean catchAny(e) {
		Exception_Print(e);

#if Exception_SaveTrace
		Backtrace_PrintTrace(e->trace, e->traceItems);
#endif

		excReturn ExitStatus_Failure;
	} finally {
		Parser_Destroy(parser);
		Typography_Destroy(&tyo);
	} tryEnd;

	return ExitStatus_Success;
}
