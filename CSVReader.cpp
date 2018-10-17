#include "CSVReader.h"

#include <QDebug>
#include <QFile>

static const char* SignalResPath = "SIGNAL_RESOURCES";
static const char* MockFile = "80_BPM_500HZ.csv";

CSVReader::CSVReader()
{
    m_dataCount = 0;
    m_data.clear();
    readCSVFiles();
}

CSVReader::~CSVReader()
{
}

int CSVReader::getData(int index)
{
    if (index >= m_data.count())
        return 0;

    return m_data.at(index);
}

int CSVReader::maxData()
{
    return m_dataCount;
}

QString CSVReader::filePath()
{
    QString csvResPath = getenv(SignalResPath);
    if (csvResPath.isEmpty() || csvResPath.isNull())
        return QString();

    return QString("%1/%2").arg(csvResPath, MockFile);
}

void CSVReader::readCSVFiles()
{
    QFile file(filePath());
    if (!file.exists())
        return;

    if (!file.open(QIODevice::ReadOnly))
        return;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        m_data.append(line.split(',').first().toInt());
    }
    m_dataCount = m_data.count();
    qDebug() << "data count: " << m_data.count();
}
