#ifndef __UTILS_H
	#define __UTILS_H

	typedef enum {
		ARRIVAL_EVENT=0,
		DEPARTURE_EVENT,
	} EVENT_TYPE;

	typedef struct event {
		int terminal;
		int server;
		double time;
		double arr_time;
		double depart_time;
		double service_time;
		EVENT_TYPE event_type;
	} event_st;

	typedef struct heap {
		int size;
		void *list[10000];
	} heap_st;

	typedef struct event_holder {
		event_st *e;
		struct event_holder *next;
	} queue_elem_st;

	typedef struct queue {
		queue_elem_st *head;
		queue_elem_st *tail;
		int size;
	} queue_st;

	void *extract_heap(heap_st *h);

	int insert_heap(heap_st *h, void *data);

	double exponential_rv(double lambda, double *seed);

	event_st *generate_event(int terminal, double time, EVENT_TYPE event);

	void add_event_to_queue(queue_st *q, event_st *data);

	event_st *get_event_from_queue(queue_st *q);

	void debug_event(event_st *e);
#endif
