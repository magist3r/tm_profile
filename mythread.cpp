#include "mythread.h"

MyThread::MyThread(Profile &profile, QObject *parent) :
    QThread(parent), m_profile(profile)
{

}

void MyThread::run()
{
    m_profile.calculate();
}

MyThread::~MyThread()
{
    wait();
}
