//
// Created by debian on 18.05.15.
//

#include "JobController.h"
#include <signal.h>


void JobController::addJob(Job *job) {
    job->setJobNumber(getJobNumber());
    joblist.push_back(*job);
}

JobController::JobController() {
    JobNumber = 1;
}

int JobController::getJobNumber() {
    return JobNumber++;
}

Job *JobController::getBgJob() {
    joblist.back();
}

pid_t JobController::getFgJob() {
    //joblist.back();
    for (auto it = joblist.rbegin(); it != joblist.rend(); ++it)
    {
       if(it->getState() == 0){
           return it->getPid();
       }
    }
}

pid_t JobController::stopFgJob() {

    for (auto it = joblist.rbegin(); it != joblist.rend(); ++it)
    {
        if(it->getState() == 0){
            killpg(it->getPid(), SIGTSTP);
            it->setState(2);
            return it->getPid();
        }
    }
    return -1;
}

pid_t JobController::contStoppedJob(int state) {

    for (auto it = joblist.rbegin(); it != joblist.rend(); ++it)
    {
        if(it->getState() == 2){
            killpg(it->getPid(), SIGCONT);
            it->setState(state);
            return it->getPid();
        }
    }
    return -1;
}