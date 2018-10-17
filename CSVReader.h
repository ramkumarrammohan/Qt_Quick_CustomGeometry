#ifndef CSVREADER_H
#define CSVREADER_H

#include <QString>
#include <QVector>

class CSVReader
{
public:
    CSVReader();
    ~CSVReader();

    int getData(int index);
    int maxData();

private:
    QString filePath();
    void readCSVFiles();

    int m_dataCount;
    QVector<int> m_data;
};

#endif // CSVREADER_H
