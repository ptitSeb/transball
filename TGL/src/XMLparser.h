#ifndef XML_PARSER
#define XML_PARSER

class XMLNode {
public:
	static XMLNode *from_file(FILE *fp);
	static XMLNode *from_string(char *str,int *pos);
	
	~XMLNode();

	Symbol *get_type(void);
	Symbol *get_value(void);

	Symbol *get_attribute(Symbol *name);
	Symbol *get_attribute(char *name);
	List<XMLNode> *get_children(void);
	XMLNode *get_children(Symbol *type);
	XMLNode *get_children(char *type);

	void print(int tabs);

private:
	XMLNode(void);

	static XMLNode *read_tag_from_file(FILE *fp,bool *open,bool look_for_first_character);
	static XMLNode *read_tag_from_string(char *str,int *pos,bool *open);

	Symbol *m_type;
	Symbol *m_value;
	List<Symbol> m_attribute_names;
	List<Symbol> m_attribute_values;
	List<XMLNode> m_children;
};

#endif