#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static void getInput(int *L, int *K, int *m, double *mu) {
	scanf("%d", L);
	scanf("%d", K);
	scanf("%d", m);
	scanf("%lf", mu);
}

static void runSimulation(int terms, int qsize, int servers, double mu, double lambda, int TOTAL) {
	int N = 0;
	int Ndep = 0;
	double clock = 0;

	double EN = 0;
	int total_arrival = 0;
	int dropped = 0;

	heap_st h;
	h.size = 0;
	queue_st q;
	q.size = 0;

	// generate L arrival events and add to heap
	int i = 0;
	for (i = 0; i < terms; i++) {
		event_st *ev = generate_event(i+1, clock+exponential_rv(lambda), ARRIVAL_EVENT);
		insert_heap(&h, (void *)ev);
	}
	// while !done
	int done = 0;
	while (!done) {
		// event = extract min
		event_st *ev = extract_heap(&h);
		// debug_event(ev);
		// increment clock to event clock
		double prev = clock;
		if (clock < ev->time)
			clock = ev->time;
		printf("clock: %lf\n", clock);
		// if event is arrival type
		if (ev->event_type == ARRIVAL_EVENT) {
			total_arrival++;
			// if N >= K 
			if (N == qsize) {
				// discard this event, increment blocking
				// generate new arrival event for the terminal and add to heap
				event_st *e = generate_event(ev->terminal, clock+exponential_rv(lambda), ARRIVAL_EVENT);
				//debug_event(e);
				insert_heap(&h, (void *)e);
				free(ev);
				dropped++;
			}
			// else if N < m
			  else if (N < servers) {
				// generate departure event for the terminal and add to heap
				event_st *e = generate_event(ev->terminal, clock+exponential_rv(mu), DEPARTURE_EVENT);
				//debug_event(e);
				insert_heap(&h, (void*)e);
				free(ev);
				// N++
				EN += N*(clock-prev);
				N++;
			}
			// else
	 		  else {
				// this is a queued event in case K > m
				// add to queued events
				add_event_to_queue(&q, ev);
				// N++
				EN += N*(clock-prev);
				N++;
			}
		}
		// if event is departure type
		  else if (ev->event_type == DEPARTURE_EVENT) {
			// Ndep ++
			EN += N*(clock-prev);
			Ndep++;
			N--;
			// if (Ndep < TOTAL)
			if (Ndep < TOTAL) {
				// get queued event
				event_st *e = get_event_from_queue(&q);
				// if queued event
				if (e) {
					// generate departure event for the queued event and add to heap
					//debug_event(e);
					int x = 0;
					if (N >= servers) {
						x = servers;
					} else {
						x = N;
					}
					event_st *de = generate_event(e->terminal, clock+exponential_rv(mu), DEPARTURE_EVENT);
					insert_heap(&h, de);
					free(e);
				}
				// generate arrival event for the terminal and add to heap
				event_st *ae = generate_event(ev->terminal, clock+exponential_rv(lambda), ARRIVAL_EVENT);
				//debug_event(ae);
				insert_heap(&h, ae);
			}
			// else 
			  else {
				// done = true
				done = 1;
			}
			free(ev);
		}
		printf("System: N=%d Ndep=%d Blocked=%d Queued=%d\n", N, Ndep, dropped, q.size);
	}

	printf("Current no. of customers in the system: %d\n", N);
	printf("Expected no. of customers in the system: %lf\n", EN/clock);
	printf("Total Arrived: %d Blocked %d Probability: %lf\n", total_arrival, dropped, (double)dropped/(double)total_arrival);
}

int main() {
	// L, K, m, lambda, mu
	int L;
	int K;
	int m;
	double lambda;
	double mu;

	int runs = 1000000;

	getInput(&L, &K, &m, &mu);
	lambda = 0.1 * m * mu;
	runSimulation(L, K, m, mu, lambda, runs);

	/*
	lambda = 0.2 * m * mu;
	runSimulation(L, K, m, mu, lambda, runs);
	*/
	return 0;
}
