#ifndef __TGL_REPLAY_LOADER
#define __TGL_REPLAY_LOADER

#define TGL_RL_MAXTHREADS		4


class TGLreplayLoaderTask {
public:
	~TGLreplayLoaderTask();

	pthread_t m_thread;
	pthread_attr_t m_thread_attr;

	static void *thread(void *task);
	void stop(void);
	
	FILE *m_fp;
	class TGLreplayLoader *m_father;
	TGLreplay **m_destination;
	bool m_running;
	bool m_stopsignal;
};

class TGLreplayLoader {
public:
	TGLreplayLoader(void);
	~TGLreplayLoader();

	void load_replay(FILE *fp,TGLreplay **m_destination);
	void cancel_all(void);

	void launch_threads(void);


	List<TGLreplayLoaderTask> m_pending_tasks;
	List<TGLreplayLoaderTask> m_executing_tasks;
	pthread_mutex_t m_mutex; 

};


#endif

