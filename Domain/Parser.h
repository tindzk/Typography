#import <File.h>
#import <Path.h>
#import <YAML.h>
#import <String.h>
#import <Logger.h>
#import <Integer.h>
#import <StringStream.h>
#import <Ecriture/Tree.h>
#import <Ecriture/Parser.h>

#import "Body.h"

#define self Parser

record(ref(Node)) {
	RdString name;
	RdString options;
	DocumentTree_Node *node;
};

Callback(ref(OnNode), void, ref(Node) *node);

record(ref(Handler)) {
	RdString    name;
	ref(OnNode) onNode;
};

class {
	String        file;
	YAML          yml;
	Ecriture_Tree ecr;
	size_t        ofsLine;
	Logger        *logger;
	BodyArray     *footnotes;
	bool          autoDetectParagraphs;
};

rsdef(self, New);
def(void, Destroy);
def(void, SetAutoDetectParagraphs, bool value);
def(BodyArray *, GetFootnotes);
def(DocumentTree_Node *, GetRoot);
def(void, Parse, RdString path);
def(RdString, GetMeta, RdString name);
def(RdStringArray *, GetMultiMeta, RdString name);
def(void, ProcessNodes, DocumentTree_Node *node, ref(Handler) *handlers);
def(Body, ProcessBody, DocumentTree_Node *node, ref(Handler) *handlers);
def(ref(Node), GetNodeByName, RdString name);

#undef self
