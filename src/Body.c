#import "Body.h"

def(void, Init) {
	Array_Init(this->items, Body_DefaultLength);

	ref(Entry) entry;

	entry.type = ref(EntryType_Items);
	Array_Init(entry.items, Body_DefaultLength);

	Array_Push(this->items, entry);
}

static def(void, Push, ref(Item) item) {
	ref(Entry) *curEntry = &this->items->buf[this->items->len - 1];

	switch (curEntry->type) {
		case ref(EntryType_Items):
			Array_Push(curEntry->items, item);
			break;

		case ref(EntryType_Block):
			Array_Push(curEntry->items, item);
			break;

		case ref(EntryType_Paragraph):
			Array_Push(curEntry->items, item);
			break;

		case ref(EntryType_List): {
			ref(ListItem) *listItem =
				&curEntry->u.list->buf[curEntry->u.list->len - 1];

			Array_Push(listItem->items, item);

			break;
		}
	}
}

def(void, EnterBlock, ref(BlockType) type) {
	ref(Entry) entry;

	entry.type = ref(EntryType_Block);
	Array_Init(entry.items, Body_DefaultLength);
	entry.u.block = type;

	Array_Push(this->items, entry);
}

def(void, EnterParagraph) {
	ref(Entry) entry;
	entry.type = ref(EntryType_Paragraph);
	Array_Init(entry.items, Body_DefaultLength);

	Array_Push(this->items, entry);
}

def(void, EnterList) {
	ref(Entry) entry;
	entry.type  = ref(EntryType_List);
	entry.items = NULL; /* unused */
	Array_Init(entry.u.list, Body_DefaultLength);

	Array_Push(this->items, entry);
}

def(void, EnterListItem) {
	ref(ListItem) item;
	Array_Init(item.items, Body_DefaultLength);

	ref(Entry) *curEntry = &this->items->buf[this->items->len - 1];
	Array_Push(curEntry->u.list, item);
}

def(void, Return) {
	ref(Entry) entry;
	entry.type = ref(EntryType_Items);
	Array_Init(entry.items, Body_DefaultLength);

	Array_Push(this->items, entry);
}

overload def(void, AddText, String text, int style) {
	ref(Item) item;

	item.type = ref(ItemType_Text);

	item.u.text.value = String_Clone(text);
	item.u.text.style = style;

	ref(Push)(this, item);
}

overload def(void, AddText, String text) {
	ref(AddText)(this, text, 0);
}

def(void, AddCommand, String value) {
	ref(Item) item;

	item.type        = ref(ItemType_Command);
	item.u.cmd.value = String_Clone(value);

	ref(Push)(this, item);
}

def(void, AddCode, String value) {
	ref(Item) item;

	item.type        = ref(ItemType_Code);
	item.u.cmd.value = String_Clone(value);

	ref(Push)(this, item);
}

def(void, AddMail, String caption, String addr) {
	ref(Item) item;

	item.type           = ref(ItemType_Mail);
	item.u.mail.caption = String_Clone(caption);
	item.u.mail.addr    = String_Clone(addr);

	ref(Push)(this, item);
}

def(void, AddAnchor, String name) {
	ref(Item) item;

	item.type          = ref(ItemType_Anchor);
	item.u.anchor.name = String_Clone(name);

	ref(Push)(this, item);
}

def(void, AddJump, String caption, String anchor) {
	ref(Item) item;

	item.type           = ref(ItemType_Jump);
	item.u.jump.caption = String_Clone(caption);
	item.u.jump.anchor  = String_Clone(anchor);

	ref(Push)(this, item);
}

def(void, AddUrl, String caption, String url) {
	ref(Item) item;

	item.type          = ref(ItemType_Url);
	item.u.url.caption = String_Clone(caption);
	item.u.url.url     = String_Clone(url);

	ref(Push)(this, item);
}
