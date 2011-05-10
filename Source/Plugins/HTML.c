#import "HTML.h"

void Plugins_HTML(RdString base, Document *doc, Channel *ch) {
	String s = String_New(1024);

	MainTemplate tpl = {
		.doc  = doc,
		.base = base
	};

	Template_Main(&tpl, &s);

	for (size_t total = 0; total < s.len; ) {
		size_t written = Channel_Write(ch,
			s.buf + total,
			s.len - total);

		if (written == 0) {
			break;
		}

		total += written;
	}

	String_Destroy(&s);
}
