#ifndef _SFX_MANAGER
#define _SFX_MANAGER


class SFXManagerNode {
public:
	Mix_Chunk *m_sfx;
	Symbol *m_name;
};


class SFXManager {
public:
	SFXManager();
	~SFXManager();

	Mix_Chunk *get(char *name);
	Mix_Chunk *get(Symbol *name);

	void next_cycle(void);	// clears the list of already played SFX
	void cache(char *dirname);

protected:

	List<SFXManagerNode> *m_hash;

	List<SFXManagerNode> m_already_played;
	char *remove_extension(char *filename);
};

#endif


