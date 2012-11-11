#ifndef CALCTHREAD_H
#define CALCTHREAD_H

#include <QThread>
#include "profile.h"

class CalcThread : public QThread
{
    Q_OBJECT
    Q_PROPERTY(Profile profile READ profile WRITE setProfile)
public:
    explicit CalcThread(Profile &profile, QObject *parent = 0);
    ~CalcThread();
    void setProfile(Profile &profile) { m_profile = profile; }

    Profile &profile() const
    {
        return m_profile;
    }

private:
    Profile &m_profile;
protected:
    void run();
signals:
    
public slots:
    
};

#endif // MYTHREAD_H
