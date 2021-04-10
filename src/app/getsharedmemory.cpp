#include "getsharedmemory.h"

#include "audiomanager.h"

void GetSharedMemory::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void GetSharedMemory::run() {
    debug::out(3, "Running shared memory thread...");

    readSemaphore = new QSystemSemaphore("HorizonReadSemaphore");
    writeSemaphore = new QSystemSemaphore("HorizonWriteSemaphore");
    sharedMemory = new QSharedMemory("HorizonSharedData");

    while (true) {

        writeSemaphore->acquire();

        if (!sharedMemory->isAttached()) {
            sharedMemory->attach();
        }

        QPair<double, QList<QList<int>>> horizonData;

        QBuffer buffer;

        QDataStream in;
        in.setDevice(&buffer);
        buffer.setData((char*)sharedMemory->constData(), sharedMemory->size());
        buffer.open(QBuffer::ReadOnly);
        in >> horizonData;
        readSemaphore->release();

        audioManager->setCurrentGridTime(horizonData.first);

        for (int i = 0; i < horizonData.second.size(); i++) {
            Track *track = audioManager->getTrack(i);

            track->lMeterData = {horizonData.second.at(i).at(0), horizonData.second.at(i).at(1)};
            track->rMeterData = {horizonData.second.at(i).at(2), horizonData.second.at(i).at(3)};

            track->uiUpdate();
        }
        delay(50);
    }

}
