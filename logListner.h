#ifndef LOGLISTNER_H
#define LOGLISTNER_H

#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/log_msg.h>
#include <spdlog/details/null_mutex.h>
#include <QLabel>
#include <QMetaObject>
#include <QString>
#include <QStringList>
#include <mutex>

#include "securitycamera.h"
#include "camcontainer.h"

template<typename Mutex>
class logListner : public spdlog::sinks::base_sink<Mutex>
{

private:
    QStringList logBuffer;
    int maxShownLogs = 4;

public:
    QLabel* label;
    explicit logListner(QLabel* l) : label(l) {}

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        // Only show info, warning and error messages
        if (msg.level != spdlog::level::info &&
            msg.level != spdlog::level::warn &&
            msg.level != spdlog::level::err)
        {
            return;
        }

        // Use the raw payload directly – no timestamp, logger name or level prefix
        QString logMessage = QString::fromUtf8(msg.payload.data(), msg.payload.size());

        QMetaObject::invokeMethod(label, [this, logMessage]() {
            if (label == nullptr) {
                return;
            }

            QString message = logMessage.isEmpty()
                                  ? QString("error without description occurred")
                                  : logMessage;

            if (logBuffer.size() < maxShownLogs) {
                logBuffer.append(message);
            } else {
                logBuffer.removeFirst();
                logBuffer.append(message);
            }

            label->setText(logBuffer.join("\n"));
        }, Qt::AutoConnection);
    }

    void flush_() override {}
};

using logListner_mt = logListner<std::mutex>;
using logListner_st = logListner<spdlog::details::null_mutex>;

#endif // LOGLISTNER_H
