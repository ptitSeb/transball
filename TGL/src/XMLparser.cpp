#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif

#ifdef WIN32
#include "windows.h"
#else
#include <sys/time.h>
#include <time.h>
#endif
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "math.h"

#include "List.h"

#include "symbol.h"

#include "XMLparser.h"


XMLNode *XMLNode::from_file(FILE *fp)
{
	char buffer[1024];
	List<XMLNode> stack;
	XMLNode *node,*first_node=0;
	bool open;
	char c;
	bool look_for_first_character=true;

	do {
		node=read_tag_from_file(fp,&open,look_for_first_character);

		if (first_node==0) first_node=node;

		if (node!=0) {
			if (open) {
				if (!stack.EmptyP()) stack[0]->m_children.Add(node);
				stack.Insert(node);

				// get the value of the XML node:
				{
					int i=0;

					do{
						buffer[i++]=c=fgetc(fp);
					}while(c!='<' && !feof(fp));

					if (i>0) i--;
					buffer[i]=0;
					Symbol::arrange_string(buffer);
					if (strlen(buffer)>0) node->m_value=new Symbol(buffer);
					if (c=='<') look_for_first_character=false;
						   else look_for_first_character=true;
				}
			
			} else {
				if (!stack[0]->m_type->cmp(node->m_type)) printf("XMLNode::from_file: Warning! XML mismatch: %s -> %s\n",
																stack[0]->m_type->get(),
																node->m_type->get());
				stack.ExtractIni();
				delete node;
				look_for_first_character=true;
			} // if 
		
		} // if 

	}while(node!=0);

	return first_node;
} /* XMLNode::from_file */ 


XMLNode *XMLNode::read_tag_from_file(FILE *fp,bool *open,bool look_for_first_character)
{
	XMLNode *tmp;
	char buffer[1024];
	int i;
	char c;
	bool end_of_tag=false;

	*open=true;

	if (look_for_first_character) {
		// Look for the '<':
		do{
			c=fgetc(fp);
		}while(c!='<' && !feof(fp));
		if (feof(fp)) return 0;
	} // if 
	
	i=0;
	c=fgetc(fp);
	if (c=='/') {
		*open=false; 
	} else {
		buffer[i++]=c;
	} // if 

	tmp=new XMLNode();

	do{
		c=fgetc(fp);
		if (!end_of_tag) {
			buffer[i++]=c;
			if (c==' ') {
				end_of_tag=true;
				// look for attributes:

				int j=0;
				char buffer2[1024];				
				while(c==' ') c=fgetc(fp);
				buffer2[j++]=c;
				while(c!=' ' && c!='=') buffer2[j++]=c=fgetc(fp);

				if (j>0) j--;
				buffer2[j]=0;
				tmp->m_attribute_names.Add(new Symbol(buffer2));
				j=0;

				while(c!='\"') c=fgetc(fp);
				buffer2[j++]=c=fgetc(fp);
				while(c!='\"') buffer2[j++]=c=fgetc(fp);
				if (j>0) j--;
				buffer2[j]=0;
				tmp->m_attribute_values.Add(new Symbol(buffer2));
			} // if 
		} else {
		} // if 	
	}while(c!='>' && !feof(fp));
	if (feof(fp)) return 0;
	if (i>0) i--;
	buffer[i]=0;

	tmp->m_type=new Symbol(buffer);

	return tmp;
} /* XMLNode::read_tag_from_file */ 



XMLNode::XMLNode(void)
{
	m_type=0;
	m_value=0;

} /* XMLNode::XMLNode */ 


XMLNode::~XMLNode()
{
	delete m_type;
	delete m_value;

} /* XMLNode::~XMLNode */ 


Symbol *XMLNode::get_type(void)
{
	return m_type;
} /* XMLNode::get_type */ 


Symbol *XMLNode::get_value(void)
{
	return m_value;
} /* XMLNode::get_value */ 


Symbol *XMLNode::get_attribute(Symbol *name)
{
	int pos=m_attribute_names.Position(name);

	if (pos==-1) return 0;

	return m_attribute_values[pos];
} /* XMLNode::get_attribute */ 


Symbol *XMLNode::get_attribute(char *name)
{
	Symbol *n=new Symbol(name);
	Symbol *res=get_attribute(n);
	delete n;
	return res;
} /* XMLNode::get_attribute */ 


List<XMLNode> *XMLNode::get_children(void)
{
	List<XMLNode> *l=new List<XMLNode>;

	l->Instance(m_children);

	return l;
} /* XMLNode::get_children */ 


void XMLNode::print(int tabs)
{
	int i;
	List<Symbol> l1,l2;
	Symbol *s1,*s2;
	List<XMLNode> l3;
	XMLNode *n;

	for(i=0;i<tabs;i++) printf("  ");
	printf("<%s",m_type->get());

	l1.Instance(m_attribute_names);
	l2.Instance(m_attribute_values);
	l1.Rewind();
	l2.Rewind();
	while(l1.Iterate(s1) && l2.Iterate(s2)) {
		printf(" %s = \"%s\"",s1->get(),s2->get());
	} // while 
	printf(">\n");

	if (m_value!=0) {
		for(i=0;i<tabs+1;i++) printf("  ");
		printf("%s\n",m_value->get());
	} // if 

	l3.Instance(m_children);
	l3.Rewind();
	while(l3.Iterate(n)) n->print(tabs+1);

	for(i=0;i<tabs;i++) printf("  ");
	printf("</%s>\n",m_type->get());
} /* XMLNode::print */ 


XMLNode *XMLNode::get_children(Symbol *type)
{
	List<XMLNode> l;
	XMLNode *c;

	l.Instance(m_children);
	l.Rewind();
	while(l.Iterate(c)) {
		if (c->get_type()->cmp(type)) return c;
	} // while 

	return 0;
} /* XMLNode::get_children */ 


XMLNode *XMLNode::get_children(char *type)
{
	List<XMLNode> l;
	XMLNode *c;

	l.Instance(m_children);
	l.Rewind();
	while(l.Iterate(c)) {
		if (c->get_type()->cmp(type)) return c;
	} // while 

	return 0;
} /* XMLNode::get_children */ 
