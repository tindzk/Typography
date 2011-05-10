#import <Main.h>
#import <Path.h>
#import <File.h>

#import "App.h"

#define self Application

/* Use our own method for printing out log messages (less verbose). */
def(void, OnLogMessage, FmtString msg, Logger_Level level, __unused RdString file, __unused int line) {
	Terminal_SetOutput(&this->term,
		level == Logger_Level_Error
			? Channel_StdErr
			: Channel_StdOut);

	Terminal_FmtPrint(&this->term, $("[%] $\n"),
		Logger_ResolveLevel(level), msg);

	Terminal_SetOutput(&this->term, Channel_StdOut);
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

	App app = App_New(&this->logger);
	App_Parse(&app, base, filename);
	App_Destroy(&app);

	return true;
}
