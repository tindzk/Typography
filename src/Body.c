#import "Body.h"

def(void, Init) {
	this->root.type   = ref(EntryType_Collection);
	this->root.parent = NULL;
	Array_Init(this->root.entries, Body_DefaultLength);

	this->curEntry = &this->root;
}

def(void, Enter) {
	ref(Entry) *entry = New(ref(Entry));

	entry->type   = ref(EntryType_Collection);
	entry->parent = this->curEntry;
	Array_Init(entry->entries, Body_DefaultLength);

	Array_Push(this->curEntry->entries, entry);

	this->curEntry = this->curEntry->entries->buf[this->curEntry->entries->len - 1];
}

def(void, Return) {
	this->curEntry = this->curEntry->parent;
}

def(void, SetBlock, ref(BlockType) type) {
	this->curEntry->type         = ref(EntryType_Block);
	this->curEntry->u.block.type = type;
}

def(void, SetParagraph) {
	this->curEntry->type = ref(EntryType_Paragraph);
}

def(void, SetUrl, String url) {
	this->curEntry->type      = ref(EntryType_Url);
	this->curEntry->u.url.url = String_Clone(url);
}

def(void, SetList) {
	this->curEntry->type = ref(EntryType_List);
}

def(void, SetListItem) {
	this->curEntry->type = ref(EntryType_ListItem);
}

overload def(void, SetText, String text, int style) {
	this->curEntry->type         = ref(EntryType_Text);
	this->curEntry->u.text.value = String_Clone(text);
	this->curEntry->u.text.style = style;
}

overload def(void, SetText, String text) {
	ref(SetText)(this, text, 0);
}

def(void, SetCommand, String value) {
	this->curEntry->type            = ref(EntryType_Command);
	this->curEntry->u.command.value = String_Clone(value);
}

def(void, SetCode, String value) {
	this->curEntry->type         = ref(EntryType_Code);
	this->curEntry->u.code.value = String_Clone(value);
}

def(void, SetMail, String addr) {
	this->curEntry->type        = ref(EntryType_Mail);
	this->curEntry->u.mail.addr = String_Clone(addr);
}

def(void, SetImage, String path) {
	this->curEntry->type         = ref(EntryType_Image);
	this->curEntry->u.image.path = String_Clone(path);
}

def(void, SetAnchor, String name) {
	this->curEntry->type          = ref(EntryType_Anchor);
	this->curEntry->u.anchor.name = String_Clone(name);
}

def(void, SetJump, String anchor) {
	this->curEntry->type           = ref(EntryType_Jump);
	this->curEntry->u.jump.anchor  = String_Clone(anchor);
}
