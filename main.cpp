#include <iostream>
#include <QCoreApplication>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QDebug>

namespace G {

class Handler:public QXmlDefaultHandler
{
    bool fatalError(const QXmlParseException &exception)
    {
        qWarning() << "Fatal Error on Line: " << exception.lineNumber()
                   << ", column" << exception.columnNumber() << " : "
                   << exception.message();
        return false;
    }

};

}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::cout << "Hello Qt 5.10.0" << std::endl;

    QXmlSimpleReader xmlReader;
    QFile file{"/Users/julian.guarin/Code/ColladaViewer/collada-octo-singularity/resources/monkey.dae"};
    QXmlInputSource* pSource = new QXmlInputSource(&file);

    G::Handler* pHandler = new G::Handler();
    xmlReader.setContentHandler(pHandler);
    xmlReader.setErrorHandler(pHandler);

    bool ok = xmlReader.parse(pSource);


    return a.exec();
}
