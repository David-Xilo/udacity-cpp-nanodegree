#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{
    // The method receive should use std::unique_lock<std::mutex> and _condition.wait()
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function.

    std::unique_lock<std::mutex> uniqueLock(_mutex);
    // https://en.cppreference.com/w/cpp/thread/condition_variable/wait
    // check lesson 4 > Building a Concurrent Message Queue
    _condition.wait(uniqueLock, [this] { return !_queue.empty(); });

    // https://stackoverflow.com/questions/37173697/is-it-okay-to-move-an-object-from-a-queue-if-youre-about-to-pop-from-it
    T msg = std::move(_queue.front());
    _queue.pop_front();
    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // The method send should use the mechanisms std::lock_guard<std::mutex>
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    std::lock_guard<std::mutex> lockGuard(_mutex);
    _queue.push_back(std::move(msg));
    _condition.notify_one();
}

/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // add the implementation of the method waitForGreen, in which an infinite while-loop
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        TrafficLightPhase tlf = _trafficLightPhaseMQ.receive();
        if (tlf == TrafficLightPhase::green) {
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // Finally, the private method „cycleThroughPhases“ should be started in a thread
    // when the public method „simulate“ is called. To do this, use the thread queue in the base class.

    // like the line threads.emplace_back(std::thread(&Intersection::processVehicleQueue, this));
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // Implement the function with an infinite loop that measures the time between two loop cycles
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.

    // https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(4000, 6000); // values in ms
    int waitTimeMs = distr(gen);
    auto start = std::chrono::high_resolution_clock::now();

    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        auto timeNow = std::chrono::high_resolution_clock::now();
        int time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - start).count();

        if (time_taken >= waitTimeMs) {
            start = std::chrono::high_resolution_clock::now();
            waitTimeMs = distr(gen);
            if (_currentPhase == TrafficLightPhase::red) {
                _currentPhase = TrafficLightPhase::green;
            } else {
                _currentPhase = TrafficLightPhase::red;
            }
            TrafficLightPhase tlp = _currentPhase;
            _trafficLightPhaseMQ.send(std::move(tlp));
        }
    }
}
