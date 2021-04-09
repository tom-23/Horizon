#include "getsharedmemory.h"

#include "audiomanager.h"

GetSharedMemory::GetSharedMemory() :
    readSemaphore("HorizonReadSemaphore"),
    writeSemaphore("HorizonWriteSemaphore"),
    sharedMemory("HorizonSharedData")
{

}

void GetSharedMemory::run() {
    debug::out(3, "Running shared memory thread...");
    updateTimer = new QTimer(this);

    sharedMemory.attach();
    sharedMemory.detach();

    connect(updateTimer, &QTimer::timeout, this, [=] () {
        if (!sharedMemory.isAttached()) {
            sharedMemory.attach();
        }
        writeSemaphore.acquire();

        QPair<double, QList<QList<int>>> horizonData;

        QBuffer buffer;

        QDataStream in;
        in.setDevice(&buffer);
        buffer.setData((char*)sharedMemory.constData(), sharedMemory.size());
        buffer.open(QBuffer::ReadOnly);
        in >> horizonData;
        readSemaphore.release();

        audioManager->setCurrentGridTime(horizonData.first);

        for (int i = 0; i < horizonData.second.size(); i++) {
            Track *track = audioManager->getTrack(i);

            track->lMeterData = {horizonData.second.at(i).at(0), horizonData.second.at(i).at(1)};
            track->rMeterData = {horizonData.second.at(i).at(2), horizonData.second.at(i).at(3)};

            track->uiUpdate();
        }
    });

    updateTimer->start(10);
}
