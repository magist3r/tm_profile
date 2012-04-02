#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include "profile.h"

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(Profile &profile, QObject *parent = 0);
    ~MyThread();
    void setProfile(Profile &profile);
    Profile &getProfile() { return m_profile; }
private:
    Profile &m_profile;
protected:
    void run();
signals:
    
public slots:
    
};

#endif // MYTHREAD_H
