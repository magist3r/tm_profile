#include "calcthread.h"

CalcThread::CalcThread(Profile &profile, QObject *parent) :
    QThread(parent), m_profile(profile)
{

}

void CalcThread::run()
{
    m_profile.calculate();
}

CalcThread::~CalcThread()
{
    wait();
}


