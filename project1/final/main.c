 /*
 * Harshavardhan Nalajala 
 * hxn170230
 */
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static void getInput(int *L, int *K, int *m, double *mu, int *runs) {
	scanf("%d", runs);
	scanf("%d", L);
	scanf("%d", K);
	scanf("%d", m);
	scanf("%lf", mu);
}

static int get_next_free_server(int servers, int *busy) {
	int i = 0;
	for (i = 0; i < servers; i++) {
		if (busy[i] != 1) {
			return i;
		}
	}
	return -1;
}

static void runSimulation(int terms, int qsize, int servers, double mu, double lambda, int TOTAL) {
	int N = 0;
	int Ndep = 0;
	double clock = 0;

	double EN = 0;
	double ET = 0;
	double total_arrival = 0;
	double accept_arrival = 0;
	double dropped = 0;
	double system_busy_time = 0;

	// data structure used--heap and queue
	// heap to hold requests in chronological order
	// queue to hold requests in the system
	heap_st h;
	h.size = 0;
	queue_st q;
	q.size = 0;

	int i = 0;
	double seed[terms];
	double server_seed[servers];
	int server_busy[servers];
	for (i = 0; i < servers; i++) {
		server_busy[i] = 0;
		server_seed[i] = 1111;
	}
	// generate L arrival events and add to heap
	for (i = 0; i < terms; i++) {
		seed[i] = 1111;
		event_st *ev = generate_event(i+1, clock+exponential_rv(lambda, &seed[i]), ARRIVAL_EVENT);
		insert_heap(&h, (void *)ev);
	}
	// while !done
	int done = 0;
	while (!done) {
		// event = extract min
		event_st *ev = extract_heap(&h);
		// debug_event(ev);
		// increment clock to event clock
		//printf("clock: %lf\n", clock);
		// if event is arrival type
		double prev = clock;
		clock = ev->time;
		EN += N*(clock-prev);
		if (N >= servers) {
			system_busy_time += servers*(clock-prev);
		} else {
			system_busy_time += N*(clock-prev);
		}
		if (ev->event_type == ARRIVAL_EVENT) {
			total_arrival++;
			// if N >= K 
			if (N == qsize) {
				// discard this event, increment blocking
				// generate new arrival event for the terminal and add to heap
				event_st *e = generate_event(ev->terminal, clock+exponential_rv(lambda, &seed[ev->terminal -1]), ARRIVAL_EVENT);
				//debug_event(e);
				insert_heap(&h, (void *)e);
				free(ev);
				dropped++;
			}
			// else if N < m
			  else if (N < servers) {
				// generate departure event for the terminal and add to heap
				int server_id = get_next_free_server(servers, server_busy);
				event_st *e = generate_event(ev->terminal, clock+exponential_rv(mu, &server_seed[server_id]), DEPARTURE_EVENT);
				e->server = server_id;
				e->arr_time = clock;
				e->service_time = e->time - clock;
				server_busy[server_id] = 1;
				//debug_event(e);
				insert_heap(&h, (void*)e);
				free(ev);
				// N++
				N++;
				accept_arrival++;
			}
			// else
	 		  else {
				// this is a queued event in case K > m
				// add to queued events
				ev->arr_time = clock;
				add_event_to_queue(&q, ev);
				// N++
				N++;
				accept_arrival++;
			}
		}
		// if event is departure type
		  else if (ev->event_type == DEPARTURE_EVENT) {
			// Ndep ++
			ev->depart_time = clock;
			//printf("Depart TIME: %lf arr time: %lf time_diff: %lf\n", ev->depart_time, ev->arr_time, ev->depart_time - ev->arr_time);
			Ndep++;
			ET += (ev->depart_time - ev->arr_time);
			N--;

			server_busy[ev->server] = 0;
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
					int server_id = get_next_free_server(servers, server_busy);
					event_st *de = generate_event(e->terminal, clock+exponential_rv(mu, &server_seed[server_id]), DEPARTURE_EVENT);
					de->server = server_id;
					de->arr_time = e->arr_time;
					de->service_time = de->time - clock;
					insert_heap(&h, de);
					free(e);
				}
				// generate arrival event for the terminal and add to heap
				event_st *ae = generate_event(ev->terminal, clock+exponential_rv(lambda, &seed[ev->terminal -1]), ARRIVAL_EVENT);
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
		//printf("%d,%lf\n",N,clock);
	}

	double val = 1;
	for (i = 1; i <= qsize; i++) {
		int j = 0;
		double v = 1;
		for (j = 0; j < i; j++) {
			v = v*(terms-j)*(lambda/mu);
	}
		for (j = 1; j <= i; j++) {
			if (j < servers) {
				v = v/j;
			} else {
				v = v/servers;
			}
		}
		val = val + v;
	}
	double probs[qsize];
	probs[0] = 1/val;
	for (i = 1; i <= qsize; i++) {
		if (i < servers) {
			probs[i] = (lambda/(i*mu))*(terms-i+1)*(probs[i-1]);
		} else {
			probs[i] = (lambda/(servers*mu))*(terms-i+1)*(probs[i-1]);
		}
	}
	double expVal = 0;
	double avglambda = 0;
	double util = 0;
	val = 0;
	for (i = 0; i <= qsize; i++) {
		expVal += i*probs[i];
		if (i != qsize)
			avglambda += (terms-i)*lambda*probs[i];
		val = val + (terms-i)*lambda*probs[i];
		if (i < servers) {
			util += ((double)i/(double)servers)*probs[i];
		} else {
			util += probs[i];
		}
	}
	double blocked = ((terms-qsize)*lambda*probs[qsize])/val;
	system_busy_time = system_busy_time/servers;

	printf("%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", lambda, EN/clock, expVal, ET/Ndep,
			expVal/avglambda, dropped/total_arrival, blocked, util, system_busy_time/clock);
}

int main() {
	// L, K, m, lambda, mu
	int L;
	int K;
	int m;
	double lambda;
	double mu;

	int runs = 0;

	getInput(&L, &K, &m, &mu, &runs);
	
	double i = 0;
	for (i = 1; i < 101; i++) {
		lambda = (i/100)*(m*mu);
		runSimulation(L, K, m, mu, lambda, runs);
	}

	/*
	lambda = 0.2 * m * mu;
	runSimulation(L, K, m, mu, lambda, runs);
	*/
	return 0;
}
