/*
 * Copyright 2023 C Thing Software
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <QTest>
#include <QtPlugin>
#include <QFileInfo>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <meazure/xml/XMLParser.h>
#include <test/meazure/testing/TestHelpers.h>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class XMLParserTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testParserHandlerNoop();
    [[maybe_unused]] void testParserHandlerNoValidation();
    [[maybe_unused]] void testDOMNoValidation();
    [[maybe_unused]] void testValidationInternalDTD();
    [[maybe_unused]] void testValidationExternalDTD();
    [[maybe_unused]] void testParsingError();
    [[maybe_unused]] void testValidationError();
};


QString xml1(R"|(<?xml version="1.0" encoding="UTF-8"?>
<elem1>
    <elem2>
        <elem3>Test XML Data Meazure&#x2122;</elem3>
        <elem4 attr1="abc" attr2="1" attr3="2.5" attr4="true"/>
    </elem2>
</elem1>
)|");

QString xml2 = R"|(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE elem1 [
  <!ELEMENT elem1 (#PCDATA)>
]>
<elem1/>
)|";

QString xml3 = R"|(<?xml version="1.0" encoding="UTF-8"?>
<elem1>
    <elem2>
        <elem3>Test XML Data</elem3>
        <elem4 attr1="abc" attr2="1" attr3="2.5" attr4="true"/>
</elem1>
)|";

QString xml4 = R"|(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE elem1 [
  <!ELEMENT elem1 (#PCDATA)>
]>
<elem1>
    <elem2/>
</elem1>
)|";

QString xml5 = R"|(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE elem1 SYSTEM "https://www.cthing.com/test.dtd">
<elem1/>
)|";

QString xml6 = R"|(<?xml version="1.0" encoding="UTF-8"?>
<elem1>
    <elem2>
        <elem3>Test XML Data Meazure&#x2122;</elem3>
        <elem4 attr1="abc" attr2="1" attr3="2.5" attr4="true"/>
        <elem4 attr1="def"/>
    </elem2>
</elem1>
)|";


[[maybe_unused]] void XMLParserTest::testParserHandlerNoop() {

    XMLParserHandler testHandler;
    XMLParser parser(&testHandler);
    parser.parseString(xml1);

    if (QTest::currentTestFailed()) {
        return;
    }
}

[[maybe_unused]] void XMLParserTest::testParserHandlerNoValidation() {

    struct TestHandler : public XMLParserHandler {
        QString charData;
        bool elem1Start = false;
        bool elem1End = false;
        bool elem2Start = false;
        bool elem2End = false;
        bool elem3Start = false;
        bool elem3End = false;
        bool elem4Start = false;
        bool elem4End = false;

        void startElement(const QString& container, const QString& elementName, const XMLAttributes& attrs) override {
            if (elementName == "elem1") {
                elem1Start = true;
                QVERIFY(container.isEmpty());
            } else if (elementName == "elem2") {
                elem2Start = true;
                QCOMPARE(container, "elem1");
            } else if(elementName == "elem3") {
                elem3Start = true;
                QCOMPARE(container, "elem2");
            } else if(elementName == "elem4") {
                elem4Start = true;
                QCOMPARE(container, "elem2");

                QString value1;
                QVERIFY(attrs.getValueStr("attr1", value1));
                QCOMPARE(value1, "abc");

                int value2 = 0;
                QVERIFY(attrs.getValueInt("attr2", value2));
                QCOMPARE(value2, 1);

                double value3 = 0.0;
                QVERIFY(attrs.getValueDbl("attr3", value3));
                QCOMPARE(value3, 2.5);
            } else {
                QFAIL(QString("Unexpected start element %1").arg(elementName).toUtf8().constData());
            }
        }

        void endElement(const QString& container, const QString& elementName) override {
            if (elementName == "elem1") {
                elem1End = true;
                QVERIFY(container.isEmpty());
            } else if (elementName == "elem2") {
                elem2End = true;
                QCOMPARE(container, "elem1");
            } else if (elementName == "elem3") {
                elem3End = true;
                QCOMPARE(container, "elem2");
            } else if (elementName == "elem4") {
                elem4End = true;
                QCOMPARE(container, "elem2");
            } else {
                QFAIL(QString("Unexpected end element %1").arg(elementName).toUtf8().constData());
            }
        }

        void characterData(const QString& container, const QString& data) override {
            QVERIFY(!container.isEmpty());
            QVERIFY(!data.isEmpty());

            const QString cleanStr(data.trimmed());
            if (!cleanStr.isEmpty()) {
                charData += cleanStr;
            }
        }

        xercesc::InputSource* resolveEntity(const QString& pathname) override {
            MEA_VERIFY(pathname.isEmpty());
            return nullptr;
        }
    } testHandler;

    XMLParser parser(&testHandler);
    parser.parseString(xml1);

    if (QTest::currentTestFailed()) {
        return;
    }

    QCOMPARE(testHandler.charData, "Test XML Data Meazure\u2122");
    QVERIFY(testHandler.elem1Start);
    QVERIFY(testHandler.elem1End);
    QVERIFY(testHandler.elem2Start);
    QVERIFY(testHandler.elem2End);
    QVERIFY(testHandler.elem3Start);
    QVERIFY(testHandler.elem3End);
    QVERIFY(testHandler.elem4Start);
    QVERIFY(testHandler.elem4End);
}

[[maybe_unused]] void XMLParserTest::testDOMNoValidation() {
    XMLParser parser;
    parser.parseString(xml6);

    const XMLNode* elem1 = parser.getDOM();
    QVERIFY(elem1);

    // elem1
    QCOMPARE(elem1->getType(), XMLNode::Type::Element);
    QCOMPARE(elem1->getData(), "elem1");
    QVERIFY(!elem1->hasAttributes());

    auto children = elem1->getChildIter();
    QVERIFY(!elem1->atEnd(children));

    // elem2
    while ((*children)->getType() != XMLNode::Type::Element) {
        children++;
        QVERIFY(!elem1->atEnd(children));
    }

    const XMLNode* elem2 = *children++;
    QCOMPARE(elem2->getData(), "elem2");
    QVERIFY(!elem2->hasAttributes());

    QCOMPARE(elem2->findChildElements("elem4").size(), 2);

    children = elem2->getChildIter();
    QVERIFY(!elem2->atEnd(children));

    // elem3
    while ((*children)->getType() != XMLNode::Type::Element) {
        children++;
        QVERIFY(!elem1->atEnd(children));
    }

    const XMLNode* elem3 = *children;
    QCOMPARE(elem3->getData(), "elem3");
    QVERIFY(!elem3->hasAttributes());
    QCOMPARE(elem3->getChildData(), "Test XML Data Meazure\u2122");

    // elem4
    const XMLNode* elem4 = elem2->findChildElement("elem4");
    QCOMPARE(elem4->getData(), "elem4");
    QVERIFY(elem4->hasAttributes());

    const XMLAttributes& attrs = elem4->getAttributes();

    QString value1;
    QVERIFY(attrs.getValueStr("attr1", value1));
    QCOMPARE(value1, "abc");

    int value2 = 0;
    QVERIFY(attrs.getValueInt("attr2", value2));
    QCOMPARE(value2, 1);

    double value3 = 0.0;
    QVERIFY(attrs.getValueDbl("attr3", value3));
    QCOMPARE(value3, 2.5);
}

[[maybe_unused]] void XMLParserTest::testValidationInternalDTD() {

    struct TestHandler : public XMLParserHandler {
        bool elem1Start = false;
        bool elem1End = false;

        void startElement(const QString& container, const QString& elementName, const XMLAttributes&) override {
            if (elementName == "elem1") {
                elem1Start = true;
                QVERIFY(container.isEmpty());
            } else {
                QFAIL(QString("Unexpected start element %1").arg(elementName).toUtf8().constData());
            }
        }

        void endElement(const QString& container, const QString& elementName) override {
            if (elementName == "elem1") {
                elem1End = true;
                QVERIFY(container.isEmpty());
            } else {
                QFAIL(QString("Unexpected end element %1").arg(elementName).toUtf8().constData());
            }
        }

        xercesc::InputSource* resolveEntity(const QString& pathname) override {
            MEA_VERIFY(pathname.isEmpty());
            return nullptr;
        }
    } testHandler;

    XMLParser parser(&testHandler);
    parser.parseString(xml2);

    if (QTest::currentTestFailed()) {
        return;
    }

    QVERIFY(testHandler.elem1Start);
    QVERIFY(testHandler.elem1End);
}

[[maybe_unused]] void XMLParserTest::testValidationExternalDTD() {

    struct TestHandler : public XMLParserHandler {
        bool hadEntity = false;

        xercesc::InputSource* resolveEntity(const QString& pathname) override {
            hadEntity = true;

            const QFileInfo fileInfo(pathname);
            MEA_COMPARE(fileInfo.baseName(), "test");
            MEA_COMPARE(fileInfo.suffix(), "dtd");

            const QString dtd("<!ELEMENT elem1 (#PCDATA)>");
            auto* source = new xercesc::MemBufInputSource(reinterpret_cast<const XMLByte*>(dtd.toUtf8().constData()),
                                                          dtd.toUtf8().size(), "DTD");

            return source;
        }
    } testHandler;

    XMLParser parser(&testHandler);
    parser.parseString(xml5);

    if (QTest::currentTestFailed()) {
        return;
    }

    QVERIFY(testHandler.hadEntity);
}

[[maybe_unused]] void XMLParserTest::testParsingError() {
    XMLParser parser;

    try {
        parser.parseString(xml3);
        QFAIL("Expected XMLParsingException to be thrown");
    } catch (const XMLParsingException& ex) {
        QCOMPARE(ex.getMessage(), "expected end of tag 'elem2'");
        QVERIFY(ex.getPathname().isEmpty());
        QCOMPARE(ex.getLine(), 6);
        QCOMPARE(ex.getColumn(), 3);
    }
}

[[maybe_unused]] void XMLParserTest::testValidationError() {
    XMLParser parser;

    try {
        parser.parseString(xml4);
        QFAIL("Expected XMLParsingException to be thrown");
    } catch (const XMLParsingException& ex) {
        QCOMPARE(ex.getMessage(), "no declaration found for element 'elem2'");
        QVERIFY(ex.getPathname().isEmpty());
        QCOMPARE(ex.getLine(), 6);
        QCOMPARE(ex.getColumn(), 11);
    }
}


QTEST_GUILESS_MAIN(XMLParserTest)

#include "XMLParserTest.moc"
