#ifndef QCUSTOMPROCESS_H
#define QCUSTOMPROCESS_H
#include<QObject>
#include<QProcess>

enum class ProcessKey
{
    IDE_PROCESS_INSTALL_KOMODO,
    IDE_PROCESS_UNINSTALL_KOMODO,
    IDE_PROCESS_CLEAN_KOMODO,
    IDE_PROCESS_KILL_KOMODO,
    IDE_PROCESS_KOMODOD_START,
    IDE_PROCESS_KOMODOD_STOP,
    IDE_PROCESS_GET_INFO,
    IDE_PROCESS_GET_PRIVATE_KEY,
    IDE_PROCESS_GET_PUBLIC_KEY,
    IDE_PROCESS_INSERT_PRIVATE_KEY,
    IDE_PROCESS_MONITORING_UP_LEFT,
    IDE_PROCESS_MONITORING_UP_RIGHT,
    IDE_PROCESS_MONITORING_DOWN_LEFT,
    IDE_PROCESS_MONITORING_DOWN_RIGHT,
    IDE_PROCESS_MONITORING_PERFORMANCE,
    IDE_PROCESS_ISSUER_START_ISSUING,
    IDE_PROCESS_ISSUER_IS_KOMODO_RUNNING

};
/*
constexpr size_t MONITORING_PROCESS_SIZE = 5;
constexpr ProcessKey MonitoringProc[MONITORING_PROCESS_SIZE] = {  ProcessKey::IDE_PROCESS_MONITORING_UP_LEFT,
                                                                   ProcessKey::IDE_PROCESS_MONITORING_UP_RIGHT,
                                                                   ProcessKey::IDE_PROCESS_MONITORING_DOWN_LEFT,
                                                                   ProcessKey::IDE_PROCESS_MONITORING_DOWN_RIGHT,
                                                                  ProcessKey::IDE_PROCESS_MONITORING_PERFORMANCE };
                                                                  */
class CustomProcess : public QProcess
{
    Q_OBJECT

public:
    explicit CustomProcess(QObject *parent = Q_NULLPTR);
    virtual ~CustomProcess();
    ProcessKey getProcessKey() const;
    void setProcessKey(ProcessKey key);

private:
    ProcessKey Pkey;
};

#endif // QCUSTOMPROCESS_H
