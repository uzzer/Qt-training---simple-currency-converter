#ifndef QTCURRCONV_H
#define QTCURRCONV_H

#include <QtGui/QWidget>
#include <QtDebug>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDoubleValidator>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>

#include <QFile>
#include <QIODevice>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtXml/QXmlStreamReader>
#include <QtXmlPatterns/QXmlQuery>
#include <QtXmlPatterns/QXmlResultItems>


class qtCurrconv : public QWidget
{
    Q_OBJECT

    QLineEdit* firstCurrencyField;
    QLineEdit* secondCurrencyField;

    float rate;
    QString dataurl;
    QXmlStreamReader xml;

    QNetworkAccessManager *manager;
    QNetworkReply* reply;

public:
    qtCurrconv(QWidget *parent = 0);
    ~qtCurrconv();

private:
    void processCurrencyConversion();
    void updateRate();
    void parseXml(QByteArray bytes);

public slots:
    void onFirstFieldEdit();
    void onSecondFieldEdit();
    void processNetworkReply(QNetworkReply*);

};

#endif // QTCURRCONV_H
