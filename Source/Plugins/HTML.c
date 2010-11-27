#import "HTML.h"

void Plugins_HTML(String base, Document *doc, File *file) {
	String s = HeapString(1024);

	MainTemplate tpl;

	tpl.doc  = doc;
	tpl.base = base;

	Template_Main(&tpl, &s);

	for (size_t total = 0; total < s.len; ) {
		size_t written = File_Write(
			File_FromObject(file),
			s.buf + total,
			s.len - total);

		if (written == 0) {
			break;
		}

		total += written;
	}

	String_Destroy(&s);
}
