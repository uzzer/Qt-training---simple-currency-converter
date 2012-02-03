#include "qtcurrconv.h"

qtCurrconv::qtCurrconv(QWidget *parent)
    : QWidget(parent)
{
    QDoubleValidator* validator = new QDoubleValidator();
    QVBoxLayout* layout = new QVBoxLayout();

    QLabel *firstLabel = new QLabel("EUR");
    layout->addWidget(firstLabel);
    firstCurrencyField = new QLineEdit();
    firstCurrencyField->setValidator(validator);
    firstCurrencyField->setMinimumHeight(40);
    firstCurrencyField->setMaximumWidth(200);
    firstCurrencyField->setStyleSheet("font-weight: bold; font-size: 24px;");
    connect(firstCurrencyField,SIGNAL(editingFinished()),this,SLOT(onFirstFieldEdit()));
    layout->addWidget(firstCurrencyField);

    QLabel *secondLabel = new QLabel("USD");
    layout->addWidget(secondLabel);
    secondCurrencyField = new QLineEdit();
    secondCurrencyField->setValidator(validator);
    secondCurrencyField->setMinimumHeight(40);
    secondCurrencyField->setMaximumWidth(200);
    secondCurrencyField->setStyleSheet("font-weight: bold; font-size: 24px;");
    layout->addWidget(secondCurrencyField);
    connect(secondCurrencyField,SIGNAL(editingFinished()),this,SLOT(onSecondFieldEdit()));
    this->setLayout(layout);



    rate = 1.3094;
    dataurl = "http://www.ecb.europa.eu/stats/eurofxref/eurofxref-daily.xml";

}

qtCurrconv::~qtCurrconv()
{

}

void qtCurrconv::onFirstFieldEdit(){
    secondCurrencyField->setText("");
    updateRate();
}

void qtCurrconv::onSecondFieldEdit(){
    firstCurrencyField->setText("");
    updateRate();
}

void qtCurrconv::updateRate(){
    qDebug()<<"updating rates";
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(processNetworkReply(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(dataurl)));
    qDebug()<<"GET "<<dataurl;
}

void qtCurrconv::processNetworkReply(QNetworkReply* reply){
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        //QString string(bytes);
        //qDebug() << string;
        parseXml(bytes);
        processCurrencyConversion();
    }
    else
    {

        qDebug() << reply->errorString();
    }
    //delete reply;
}

void qtCurrconv::processCurrencyConversion(){
    qDebug()<<"Updating fields based on rate "<<rate<<" EUR to USD";
    float result;
    if(firstCurrencyField->text() == ""){
        result = secondCurrencyField->text().toFloat() * rate;
        firstCurrencyField->setText(QString::number(result));
        qDebug()<<"updating first field";
    }
    if(secondCurrencyField->text() == ""){
        result = firstCurrencyField->text().toFloat() / rate;
        secondCurrencyField->setText(QString::number(result));
        qDebug()<<"updating second field";
    }
}

void qtCurrconv::parseXml(QByteArray bytes)
{
        QDomDocument doc("mydocument");
        if (!doc.setContent(bytes)) {
            return;
        }

        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild();

        bool stopflag = false;

            while(!n.isNull() || stopflag==true) {
                    QDomElement e = n.toElement();
                    if(!e.isNull()) {
                             qDebug()<<e.tagName();
                            if(e.tagName() == "Cube"){
                                rate =  e.childNodes().item(0).childNodes().item(0).toElement().attribute("rate").toFloat();
                                qDebug()<< e.childNodes().item(0).childNodes().item(0).toElement().attribute("rate");
                                qDebug()<<"currency is" << e.childNodes().item(0).childNodes().item(0).toElement().attribute("currency");
                             }
                    }
                    n = n.nextSibling();
            }

}
