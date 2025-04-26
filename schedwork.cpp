#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& workerShifts,
    size_t day,
    size_t workerSlot);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    // Get number of days and workers
    size_t n = avail.size(); // number of days
    size_t k = avail[0].size(); // number of workers
    
    // Initialize the schedule with INVALID_ID
    sched.resize(n, vector<Worker_T>(dailyNeed, INVALID_ID));
    
    // Keep track of how many shifts each worker has been assigned
    vector<size_t> workerShifts(k, 0);
    
    // Start the recursive backtracking
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShifts, 0, 0);
}

bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& workerShifts,
    size_t day,
    size_t workerSlot)
{
    size_t n = avail.size(); // number of days
    size_t k = avail[0].size(); // number of workers
    
    // Base case: we've successfully filled the entire schedule
    if (day == n) {
        return true;
    }
    
    // If we've filled all worker slots for the current day, move to the next day
    if (workerSlot == dailyNeed) {
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShifts, day + 1, 0);
    }
    
    // Try assigning each worker to the current slot
    for (Worker_T workerId = 0; workerId < k; workerId++) {
        // Check if this worker is available on this day
        if (!avail[day][workerId]) {
            continue;
        }
        
        // Check if this worker has already been scheduled for this day
        bool alreadyScheduled = false;
        for (size_t i = 0; i < workerSlot; i++) {
            if (sched[day][i] == workerId) {
                alreadyScheduled = true;
                break;
            }
        }
        if (alreadyScheduled) {
            continue;
        }
        
        // Check if this worker has reached the maximum allowed shifts
        if (workerShifts[workerId] >= maxShifts) {
            continue;
        }
        
        // Assign this worker to the current slot
        sched[day][workerSlot] = workerId;
        workerShifts[workerId]++;
        
        // Recursively fill the rest of the schedule
        if (scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShifts, day, workerSlot + 1)) {
            return true;
        }
        
        // Backtrack: undo this assignment and try the next worker
        sched[day][workerSlot] = INVALID_ID;
        workerShifts[workerId]--;
    }
    
    // If we've tried all workers and none worked, backtrack
    return false;
}