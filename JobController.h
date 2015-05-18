#include <vector>

#include "Job.h"

#ifndef LESSHELL_JOBCONTROLLER_H
#define LESSHELL_JOBCONTROLLER_H



class JobController {
private:
    std::vector<Job> joblist;
    int getJobNumber();
    int JobNumber;
public:
    JobController();
    void addJob(Job *job);
    pid_t getFgJob();
    pid_t stopFgJob();
    pid_t contStoppedJob(int state);
    Job *getBgJob();


};


#endif //LESSHELL_JOBCONTROLLER_H
