#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void getInput(double *pH, double *r11L,
		double *mu1, double *mu2, double *r2d) {
	scanf("%lf", pH);
	scanf("%lf", r11L);
	scanf("%lf", r2d);
	scanf("%lf", mu1);
	scanf("%lf", mu2);
}

int processArrivalEvent(queue_st *q, event_st *e, heap_st *h,
		double *numH, double *numL, double mu, double *s_mu,
		double clock, QUEUE_TYPE queue) {
	int addedToQueue = 0;
	if (((*numH) + (*numL)) < 1) {
		// queue1 is empty
		// generate departure event with mu1 from queue1
		event_st *dev = generate_event(queue, clock + exponential_rv(mu, s_mu),
			DEPARTURE_EVENT, e->priority);
		insert_heap(h, dev);
		// free current event
	} else {
		add_event_to_queue(q, e);
		addedToQueue = 1;
	}
	if (e->priority == PRIORITY_HIGH) {
		(*numH)+=1;
	} else {
		(*numL)+=1;
	}
	return addedToQueue;
}

event_st *generate_lambda_event(double *s_pH, double pH, queue_st *q1,
		queue_st *q2, double lambda, double *s_Lambda, double clock) {
	double rv = uniform_rv(s_pH);
	event_st *ev = NULL;
	if (rv <= pH) {
		ev = generate_event(QUEUE_2, clock + exponential_rv(lambda, s_Lambda),
				ARRIVAL_EVENT, PRIORITY_HIGH);
	} else {
		ev = generate_event(QUEUE_1, clock + exponential_rv(lambda, s_Lambda),
				ARRIVAL_EVENT, PRIORITY_LOW);
	}
	ev->toSystem = 1;
	return ev;
}

void runSimulation(double total, double lambda, double pH,
		double mu1, double mu2, double r11L, double r2d) {
	int done = 0;

	double s_pH = 1111;
	double s_r11L = 1111;
	double s_r2d = 1111;
	double s_mu1 = 1111;
	double s_mu2 = 1111;
	double s_Lambda = 1111;
	double nQ1H = 0;
	double nQ1L = 0;
	double nQ2H = 0;
	double nQ2L = 0;
	double clock = 0;
	double NDep = 0;
	double NDepH = 0;
	double NDepL = 0;
	double NDepQ1H = 0;
	double NDepQ2H = 0;
	double NDepQ1L = 0;
	double NDepQ2L = 0;

	// data structures to hold events
	heap_st h;
	queue_st q1;
	queue_st q2;

	// generate first event
	event_st *ev = generate_lambda_event(&s_pH, pH, &q1, &q2, lambda, &s_Lambda, clock);
	insert_heap(&h, ev);

	while (!done) {
		// remove event from heap
		event_st *e = extract_heap(&h);
		clock = e->time;
		switch (e->event_type) {
		case ARRIVAL_EVENT:
			switch (e->queue) {
			case QUEUE_1:
				if (!processArrivalEvent(&q1, e, &h,
					&nQ1H, &nQ1L, mu1, &s_mu1, clock, QUEUE_1)) {
					// free current arrival event
					free(e);
				} else {
				}
				break;
			case QUEUE_2:
				if (!processArrivalEvent(&q2, e, &h,
					&nQ2H, &nQ2L, mu2, &s_mu2, clock, QUEUE_2)) {
					free(e);
				} else {
				}
				break;
			default:
				printf("Arrival Event not assigned to queue ???\n");
			}
			if (e->toSystem == 1) {
				ev = generate_lambda_event(&s_pH, pH, &q1, &q2, lambda, &s_Lambda, clock);
				insert_heap(&h, ev);
			}
			break;

		case DEPARTURE_EVENT:
			switch(e->queue) {
			case QUEUE_1:
			{
				// if event is high priority, generate arrival event to q2
				if (e->priority == PRIORITY_HIGH) {
					event_st *arrEq2 = generate_event(QUEUE_2, clock,
						ARRIVAL_EVENT, PRIORITY_HIGH);
					NDepQ1H+=1;
					nQ1H--;
					insert_heap(&h, arrEq2);
				}
				// else get uniform random value for r11L
				else {
					double urv = uniform_rv(&s_r11L);
					if (urv <= r11L) {
						// if urv is less than or equal to r11L, generate arrival event to q1
						event_st *arrEq1 = generate_event(QUEUE_1, clock,
								ARRIVAL_EVENT, PRIORITY_LOW);
						insert_heap(&h, arrEq1);
					} else {
						// else generate arrival event to q2
						event_st *arrEq2 = generate_event(QUEUE_2, clock,
								ARRIVAL_EVENT, PRIORITY_LOW);
						insert_heap(&h, arrEq2);
					}
					NDepQ1L+=1;
					nQ1L--;
				}
				// get next event from queue1, generate departure event and add to heap
				event_st *eventQ1 = get_event_from_queue(&q1);
				if (eventQ1) {
					event_st *eventQ1Dep = generate_event(QUEUE_1, clock + exponential_rv(mu1, &s_mu1),
							DEPARTURE_EVENT, eventQ1->priority);
					insert_heap(&h, eventQ1Dep);
					free(eventQ1);
				}
				free(e);
				break;
			}
			case QUEUE_2:
			{
				// get urv for r2d
				double urv = uniform_rv(&s_r2d);
				// if urv is less than or equal to (1-r2d), generate arrival event to q1
				if (urv > r2d) {
					event_st *arr2Q1 = generate_event(QUEUE_1, clock, ARRIVAL_EVENT,
							e->priority);
					insert_heap(&h, arr2Q1);
				} else {
					NDep+=1;
					if (e->priority == PRIORITY_HIGH) {
						NDepH += 1;
					} else {
						NDepL += 1;
					}
				}
				// get next event from queue2, generate departure event and add to heap
				event_st *eventQ2 = get_event_from_queue(&q2);
				if (eventQ2) {
					event_st *eventQ2Dep = generate_event(QUEUE_2, clock + exponential_rv(mu2, &s_mu2),
							DEPARTURE_EVENT, eventQ2->priority);
					insert_heap(&h, eventQ2Dep);
				}
				if (e->priority == PRIORITY_HIGH) {
					nQ2H--;
					NDepQ2H+=1;
				} else {
					nQ2L--;
					NDepQ2L+=1;
				}
				free(e);
				break;
			}

			default:
				printf("Departure Event not assigned to queue ???\n");
			}
			break;

		default:
			printf("No matching case found\n");
			break;
		}

		if (NDep == total) {
			done = 1;
		}
	}

	printf("NDep: %lf NDepH: %lf NDepL: %lf clock: %lf Hthroughput: %lf Lthroughput: %lf\n",
			NDep, NDepH, NDepL, clock, NDepH/clock, NDepL/clock);
	printf("Queue1: NDepQ1H: %lf NDepQ1L: %lf nQ1H: %lf nQ1L: %lf\n", NDepQ1H, NDepQ1L, nQ1H, nQ1L);
	printf("Queue2: NDepQ2H: %lf NDepQ2L: %lf nQ2H: %lf nQ2L: %lf\n", NDepQ2H, NDepQ2L, nQ2H, nQ2L);
	printf("\n");

	event_st *toFree = extract_heap(&h);
	while (toFree) {
		free(toFree);
		toFree = extract_heap(&h);
	}
	toFree = get_event_from_queue(&q1);
	while (toFree) {
		free(toFree);
		toFree = get_event_from_queue(&q1);
	}
	toFree = get_event_from_queue(&q2);
	while (toFree) {
		free(toFree);
		toFree = get_event_from_queue(&q2);
	}
}

int main() {
	// input values
	double pH = 0;
	double lambda = 0;
	double r11L = 0;
	double mu1 = 0;
	double mu2 = 0;
	double r2d = 0;
	double total = 500000;

	getInput(&pH, &r11L, &mu1, &mu2, &r2d);

	for (lambda = 1; lambda <= 10; lambda ++) {
		runSimulation(total, lambda, pH, mu1, mu2,
				r11L, r2d);
	}

	return 1;
}
