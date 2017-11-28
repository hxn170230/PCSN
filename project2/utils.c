/*
 * Harshavardhan Nalajala 
 * hxn170230
 */
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void debug_event(event_st *ev) {
	char *s = ev->event_type==ARRIVAL_EVENT?"ARRIVAL":"DEPARTURE";
	printf("[Queue: %u]: [%s] [clock: %lf] \n", ev->queue, s, ev->time);
}

void debug_heap(heap_st *h) {
	printf("HEAP: size[%d]\n", h->size);
	int i = 0;
	for (i = 0; i < h->size; i++) {
		printf("%d[%lf] ", i, ((event_st *)(h->list[i]))->time);
	}
	printf("\n");
}

static int min_heapify(heap_st *h) {
	int pos = h->size - 1;
	
	while (pos > 0) {
		int child = pos;
		int parent = pos/2;
		event_st *c = (event_st *)(h->list[child]);
		event_st *p = (event_st *)(h->list[parent]);
		if (c->time < p->time) {
			h->list[child] = p;
			h->list[parent] = c;
			pos = parent;
		} else {
			break;
		}
	}
	// debug_heap(h);
	return 0;
}

int insert_heap(heap_st *h, void *data) {
	if (!h) {
		h = (heap_st *)malloc(sizeof(heap_st));
		h->size = 1;
		h->list[0] = data;
		return 0;
	} else {
		int pos = h->size;
		h->list[pos] = data;
		h->size = pos + 1;
		return min_heapify(h);
	}
}

void *extract_heap(heap_st *h) {
	if (!h || h->size == 0) {
		return NULL;
	} else if (h->size == 1) {
		h->size = 0;
		return h->list[0];
	}

	void *data = h->list[0];
	// debug_event((event_st *)data);
	h->list[0] = h->list[h->size-1];
	h->size -= 1;
	int pos = 0;
	while (pos < h->size) {
		int lc = 2 *pos + 1;
		int rc = 2*(pos+1);
		event_st *pdata = (event_st *)(h->list[pos]);
		event_st *ldata = NULL;
		if (lc < h->size)
			ldata = (event_st *)(h->list[lc]);
		event_st *rdata = NULL;
		if (rc < h->size)
			rdata = (event_st *)(h->list[rc]);
		if (ldata && rdata && 
			(pdata->time > ldata->time || pdata->time > rdata->time)) {
			//printf("parent(%lf) left(%lf) right(%lf)\n", pdata->time, ldata->time, rdata->time);
			if (ldata->time < rdata->time) {
				// swap ldata and data
				h->list[lc] = pdata;
				h->list[pos] = ldata;
				pos = lc;
			} else {
				// swap rdata and data
				h->list[rc] = pdata;
				h->list[pos] = rdata;
				pos = rc;
			}
		} else if (ldata && pdata->time > ldata->time) {
			//printf("parent(%lf) left(%lf)\n", pdata->time, ldata->time);
			h->list[lc] = pdata;
			h->list[pos] = ldata;
			pos = lc;
		} else if (rdata && pdata->time > rdata->time) {
			//printf("parent(%lf) right(%lf)\n", pdata->time, rdata->time);
			h->list[rc] = pdata;
			h->list[pos] = rdata;
			pos = rc;
		} else {
			break;
		}
	}
	//debug_heap(h);
	return data;
}

double uniform_rv(double *s) {
	double k = 16807;
	double m = 2147483647;
	*s = fmod(k*(*s), m);
	double r = (*s)/m;
	return r;
}

double exponential_rv(double lambda, double *s) {
	double urv = uniform_rv(s);
	double exprv = (-1/lambda)*log(urv);
	return exprv;
}

event_st *generate_event(QUEUE_TYPE queue, double time, EVENT_TYPE event, PRIORITY_TYPE priority) {
	event_st *e = (event_st *)malloc(sizeof(event_st));
	e->queue = queue;
	e->time = time;
	e->event_type = event;
	e->priority = priority;
	e->toSystem = 0;
	return e;
}

void add_event_to_queue(queue_st *q, event_st *e) {
	// allocate event holder to e
	queue_elem_st *elem = (queue_elem_st *)malloc(sizeof(queue_elem_st));
	elem->e = e;
	elem->next = NULL;
	// if priority is high, add to Hlist
	if (e->priority == PRIORITY_HIGH) {
		if (q->Hhead == NULL) {
			q->Hhead = elem;
			q->Htail = elem;
		} else {
			(q->Htail)->next = elem;
			q->Htail = elem;
		}
	} else {
		// else add to Llist
		if (q->Lhead == NULL) {
			q->Lhead = elem;
			q->Ltail = elem;
		} else {
			(q->Ltail)->next = elem;
			q->Ltail = elem;
		}
	}
	q->size += 1;
}

event_st *get_event_from_queue(queue_st *q) {
	queue_elem_st *elem = NULL;
	if (q->size == 0) {
		return NULL;
	} else if (q->size == 1) {
		q->size = 0;
		if (q->Hhead) {
			elem = q->Hhead;
			q->Hhead = NULL;
			q->Htail = NULL;
		} else {
			elem = q->Lhead;
			q->Lhead = NULL;
			q->Ltail = NULL;
		}
	} else {
		q->size -= 1;
		if (q->Hhead) {
			elem = q->Hhead;
			q->Hhead = (q->Hhead)->next;
		} else {
			elem = q->Lhead;
			q->Lhead = (q->Lhead)->next;
		}
	}
	return elem->e;
}