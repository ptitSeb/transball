#ifndef __TGL_REPLAY_LOADER
#define __TGL_REPLAY_LOADER

#define TGL_RL_MAXTHREADS		2


class TGLreplayLoaderTask {
public:
	~TGLreplayLoaderTask();

	pthread_t m_thread;
	pthread_attr_t m_thread_attr;

	static void *thread(void *task);
	void stop(void);
	
	char *m_name;
	FILE *m_fp;
	class TGLreplayLoader *m_father;
	bool m_running;
	bool m_stopsignal;
	TGLreplay *m_replay;
};

class TGLreplayLoader {
public:
	TGLreplayLoader(void);
	~TGLreplayLoader();

	void load_replay(char *name);
	TGLreplay *is_loaded(char *name);
	bool is_loading(char *name);
	void cancel_all(void);

	void launch_threads(void);


	List<TGLreplayLoaderTask> m_pending_tasks;
	List<TGLreplayLoaderTask> m_executing_tasks;
	List<TGLreplayLoaderTask> m_finished_tasks;
	pthread_mutex_t m_mutex; 

};


#endif

