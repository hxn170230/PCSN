/*
 * Harshavardhan Nalajala 
 * hxn170230
 */
#ifndef __UTILS_H
	#define __UTILS_H

	typedef enum {
		ARRIVAL_EVENT=0,
		DEPARTURE_EVENT,
	} EVENT_TYPE;

	typedef enum {
		PRIORITY_HIGH=0,
		PRIORITY_LOW,
	} PRIORITY_TYPE;

	typedef enum {
		QUEUE_1=0,
		QUEUE_2,
	} QUEUE_TYPE;

	typedef struct event {
		int toSystem;
		double time;
		EVENT_TYPE event_type;
		QUEUE_TYPE queue;
		PRIORITY_TYPE priority;
	} event_st;

	typedef struct heap {
		int size;
		void *list[1000000];
	} heap_st;

	typedef struct event_holder {
		event_st *e;
		struct event_holder *next;
	} queue_elem_st;

	typedef struct queue {
		queue_elem_st *Hhead;
		queue_elem_st *Htail;
		queue_elem_st *Lhead;
		queue_elem_st *Ltail;
		int size;
	} queue_st;

	void *extract_heap(heap_st *h);

	int insert_heap(heap_st *h, void *data);

	double exponential_rv(double lambda, double *seed);

	event_st *generate_event(QUEUE_TYPE queue, double time, EVENT_TYPE event, PRIORITY_TYPE priority);

	void add_event_to_queue(queue_st *q, event_st *data);

	event_st *get_event_from_queue(queue_st *q);

	void debug_event(event_st *e);

	double uniform_rv(double *s);
#endif
