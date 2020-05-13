#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

/* 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}
*/

/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called.
    // To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 
    std::random_device rd;
    std::default_random_engine gen = std::default_random_engine(rd());
    std::uniform_int_distribution<unsigned> cycle_duration(4000, 6000);

    
    static std::chrono::steady_clock::time_point now;
    static std::chrono::steady_clock::time_point last;
    static unsigned int time_threshold;
    timer_state state = Init; 
    

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));        

        switch (state)
        {
        case Init:
            /* std::cout << "Init " << std::endl; */ /* for debug */
            last = std::chrono::steady_clock::now();
            time_threshold = cycle_duration(gen);
            state = Run;
            break;
        case Run:
            now = std::chrono::steady_clock::now();
            if(std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count() > time_threshold)
            {
                /* std::cout << "loop take " << std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count() << "ms -> toggle" << std::endl; */ /* for debug */
                _currentPhase = _currentPhase? red:green;
                state = Reset;
            }
            break;
        case Reset:
            /* std::cout << "Reset " << std::endl; */ /* for debug */
            time_threshold = cycle_duration(gen);
            last = now;
            state = Run;
            break;       
        }
    }
}

