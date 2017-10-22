#include "utils.h"
#include <stdlib.h>
#include <math.h>

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

void *extract_min(heap_st *h) {
	if (!h || h->size == 0) {
		return NULL;
	} else if (h->size == 1) {
		h->size = 0;
		return h->list[0];
	}

	void *data = h->list[0];
	h->list[0] = h->list[h->size-1];
	h->size -= 1;
	int pos = 0;
	while (pos < h->size) {
		int lc = 2 *pos + 1;
		int rc = 2*(pos+1);
		event_st *pdata = (event_st *)(h->list[pos]);	
		event_st *ldata = (event_st *)(h->list[lc]);
		event_st *rdata = (event_st *)(h->list[rc]);
		if (pdata->time > ldata->time || pdata->time > rdata->time) {
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
		} else {
			break;
		}
	}
	return data;
}

static double s = 1111;

static double uniform_rv() {
	double k = 16807;
	double m = 2147483647;
	s = fmod(k*s, m);
	double r = s/m;
	return r;
}

double exponential_rv(double lambda) {
	double urv = uniform_rv();
	double exprv = (-1/lambda)*log(urv);
	return exprv;
}

event_st *generate_event(int terminal, double time, EVENT_TYPE event) {
	event_st *e = (event_st *)malloc(sizeof(event_st));
	e->terminal = terminal;
	e->time = time;
	e->event_type = event;
	return e;
}

void add_event_to_queue(queue_st *q, event_st *e) {
	if (q->size == 0) {
		q->head = (queue_elem_st *)malloc(sizeof(queue_elem_st));
		(q->head)->e = e;
		(q->head)->next = NULL;
		q->tail = q->head;
	} else {
		queue_elem_st *elem = (queue_elem_st *)malloc(sizeof(queue_elem_st));
		elem->e = e;
		elem->next = NULL;
		(q->tail)->next = elem;
		q->tail = elem;
		q->size += 1;
	}
}

event_st *get_event_from_queue(queue_st *q) {
	if (q->size == 0) {
		return NULL;
	} else if (q->size == 1) {
		q->size = 0;
		queue_elem_st *elem = q->head;
		q->head = NULL;
		q->tail = NULL;
		event_st *ev = elem->e;
		free(elem);
		return ev;
	} else {
		q->size -= 1;
		queue_elem_st *elem = q->head;
		q->head = elem->next;
		elem->next = NULL;
		event_st *ev = elem->e;
		free(elem);
		return ev;
	}
}