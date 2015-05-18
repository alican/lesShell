//
// Created by debian on 18.05.15.
//

#include "Job.h"

int Job::getJobNumber() {
    return jobNumber;
}

void Job::setJobNumber(int jobNumberValue) {
    this->jobNumber = jobNumberValue;
}

const std::string Job::getCommandName() {
    return commandName;
}

void Job::setCommandName(const std::string commandNameValue) {
    commandName = commandNameValue;
}

int Job::getState() const {
    return state;
}

void Job::setState(int stateValue) {
    state = stateValue;
}

pid_t Job::getPid() const {
    return pid;
}

void Job::setPid(pid_t pidValue) {
    pid = pidValue;
}

Job::Job(std::string commandNameV, pid_t pidV) {
    commandName = commandNameV;
    pid = pidV;
    jobNumber = 1;
}
