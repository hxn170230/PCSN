#include "utils.h"
#include <stdio.h>

int main() {
	// L, K, m, lambda, mu
	// generate L arrival events and add to heap
	// while !done
	// 	event = extract min
	// 	increment clock to event clock
	// 	if event is arrival type
	// 		if N >= K 
	// 			discard this event, increment blocking
	// 			generate new arrival event for the terminal and add to heap
	// 		else if N < m
	// 			generate departure event for the terminal and add to heap
	// 			N++
	// 		else
	// 			this is a queued event in case K > m
	// 			add to queued events
	// 			// what to do here ?
	//
	// 	if event is departure type
	// 		Ndep ++
	// 		if (Ndep < TOTAL)
	// 			get queued event
	// 			if queued event
	// 				generate departure event for the queued event and add to heap
	// 			generate arrival event for the terminal and add to heap
	// 		else 
	// 			done = true
	return 0;
}
