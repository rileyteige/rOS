#include <system.h>
#include <thread.h>

extern thread_t* current_thread;

mutex_t* mutex_create()
{
    assert(current_thread != NULL);
    
    mutex_t* m = (mutex_t*)kmalloc(sizeof(mutex_t));
    m->owner = NULL;
    m->waiting_queue = list_create();
    
    return m;
}

void mutex_lock(mutex_t* m)
{
    assert(m);
    assert(m->waiting_queue);
    
    uint8_t old_status = set_interrupts(DISABLED);

    assert(!m->owner || m->owner != current_thread);
    
    if (m->owner) {
        list_enqueue(m->waiting_queue, current_thread);
        thread_sleep();
    } else {
        m->owner = current_thread;
    }

    set_interrupts(old_status);
}

void mutex_unlock(mutex_t* m)
{
    assert(m);
    assert(m->waiting_queue);
    
    thread_t* t = NULL;
    
    uint8_t old_status = set_interrupts(DISABLED);

    assert(m->owner == current_thread);

    if (m->waiting_queue->length != 0) {
        t = (thread_t*)list_dequeue(m->waiting_queue);
        assert(t);
        m->owner = t;
        make_thread_ready(t);
    } else {
        m->owner = NULL;
    }
    
    set_interrupts(old_status);
}

void mutex_free(mutex_t* m)
{
    assert(m != NULL);
    assert(m->waiting_queue != NULL);
    
    list_free(m->waiting_queue);
    
    m->owner = NULL;
    m->waiting_queue = NULL;
}
