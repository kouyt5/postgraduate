void pool_init (int max_thread_num);
int pool_add_worker (int socket);
int pool_destroy ();
void *thread_routine (void *arg);
void main_loop(int socket);

