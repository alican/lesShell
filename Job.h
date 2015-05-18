#include <string>

#ifndef LESSHELL_JOB_H
#define LESSHELL_JOB_H


class Job {
private:
    int jobNumber;
    std::string commandName;
    int state;
    pid_t pid;

public:
    Job(std::string commandName, pid_t pid );

    enum state{ FG, BG, Three };

    int getJobNumber();

    void setJobNumber(int jobNumber);

    const std::string getCommandName();

    void setCommandName(const std::string commandName);

    int getState() const;

    void setState(int state);

    pid_t getPid() const;

    void setPid(pid_t pid);
};


#endif //LESSHELL_JOB_H
