#include <pthread.h>
#include <sys/socket.h>
#include "request.h"
#include "thread_pool.h"


typedef struct _request
{
    int socket;
    _request* next;
}request, *p_request;

typedef struct
{
    pthread_mutex_t queue_lock;
    pthread_cond_t queue_ready;

    /*链表结构，线程池中所有等待任务*/
    p_request queue_head;
    p_request queue_tail;

    /*是否销毁线程池*/
    int shutdown;
    pthread_t *threadid;
    /*线程池中允许的活动线程数目*/
    int max_thread_num;
    /*当前等待队列的任务数目*/
    int cur_queue_size;

} thread_pool;

int pool_add_worker (int socket);
void *thread_routine (void *arg);

static thread_pool *pool = NULL;
void pool_init (int max_thread_num)
{
    pool = (thread_pool *) malloc (sizeof (thread_pool));
    memset(pool, 0, sizeof(thread_pool));

    pthread_mutex_init (&(pool->queue_lock), NULL);
    pthread_cond_init (&(pool->queue_ready), NULL);

    pool->max_thread_num = max_thread_num;

    pool->threadid = (pthread_t *) malloc (max_thread_num * sizeof (pthread_t));
    for (int i = 0; i < max_thread_num; i++)
    {
        pthread_create (&(pool->threadid[i]), NULL, thread_routine,NULL);
    }
}

/*向线程池中加入任务*/
int pool_add_worker (int socket)
{
    /*构造一个新任务*/
    request *newworker = (request *) malloc (sizeof (request));
    newworker->socket = socket;
    newworker->next = NULL;/*别忘置空*/

    pthread_mutex_lock (&(pool->queue_lock));
    /*将任务加入到等待队列中*/
    request *member = pool->queue_head;
    if (member != NULL)
    {
        pool->queue_tail->next = newworker;
        pool->queue_tail = newworker;
    }
    else
    {
        pool->queue_head = newworker;
        pool->queue_tail = newworker;
    }

    pool->cur_queue_size++;
    pthread_mutex_unlock (&(pool->queue_lock));
    /*好了，等待队列中有任务了，唤醒一个等待线程； 
    注意如果所有线程都在忙碌，这句没有任何作用*/
    pthread_cond_signal (&(pool->queue_ready));
    return 0;
}

/*销毁线程池，等待队列中的任务不会再被执行，但是正在运行的线程会一直 
把任务运行完后再退出*/
int pool_destroy ()
{
    if (pool->shutdown)
        return -1;/*防止两次调用*/
    pool->shutdown = 1;
    /*唤醒所有等待线程，线程池要销毁了*/
    pthread_cond_broadcast (&(pool->queue_ready));
    /*阻塞等待线程退出，否则就成僵尸了*/
    for (int i = 0; i < pool->max_thread_num; i++)
        pthread_join (pool->threadid[i], NULL);
    free (pool->threadid);

    /*销毁等待队列*/
    request *head = NULL;
    while (pool->queue_head != NULL)
    {
        head = pool->queue_head;
        pool->queue_head = pool->queue_head->next;
        free (head);
    }
    /*条件变量和互斥量也别忘了销毁*/
    pthread_mutex_destroy(&(pool->queue_lock));
    pthread_cond_destroy(&(pool->queue_ready));
    free (pool);
    /*销毁后指针置空是个好习惯*/
    pool=NULL;
    return 0;
}
void *thread_routine (void *arg)
{
    printf ("starting thread 0x%lld\n", (long long)pthread_self ());
    while (1)
    {
        pthread_mutex_lock (&(pool->queue_lock));
        /*如果等待队列为0并且不销毁线程池，则处于阻塞状态; 注意 
        pthread_cond_wait是一个原子操作，等待前会解锁，唤醒后会加锁*/
        while (pool->cur_queue_size == 0 && !pool->shutdown)
        {
            printf ("thread 0x%lld is waiting\n", (long long)pthread_self ());
            pthread_cond_wait (&(pool->queue_ready), &(pool->queue_lock));
        }
        /*线程池要销毁了*/
        if (pool->shutdown)
        {
            /*遇到break,continue,return等跳转语句，千万不要忘记先解锁*/
            pthread_mutex_unlock (&(pool->queue_lock));
            printf ("thread 0x%lld will exit\n", (long long)pthread_self());
            pthread_exit (NULL);
        }
	//printf("************************************************************\n");
        printf("thread 0x%lld is starting to work\n", (long long)pthread_self ());
        /*等待队列长度减去1，并取出链表中的头元素*/
        pool->cur_queue_size--;
        request *worker = pool->queue_head;
        pool->queue_head = worker->next;
        if(pool->queue_head == NULL)
            pool->queue_tail = NULL;
        pthread_mutex_unlock (&(pool->queue_lock));

        /*调用回调函数，执行任务*/
        do_request(worker->socket);
        free (worker);
    }
}
void main_loop(int socket)
{
    pool_init(5);
    while(1){
        /* take a call and buffer it */
        int fd = accept( socket, NULL, NULL );
        if(fd == -1)
            break;
        pool_add_worker(fd);
    }
}
