/*
 * Copyright 2022 C Thing Software
 *
 * This file is part of Meazure.
 *
 * Meazure is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Meazure is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Meazure.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QTest>
#include <QtPlugin>
#include <sstream>
#include <memory>
#include <meazure/xml/XMLWriter.h>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


struct TestXMLWriter : public XMLWriter {

    explicit TestXMLWriter(std::ostream& out) : XMLWriter(out) {}

    void writeQuoted(const QString& str) override { XMLWriter::writeQuoted(str); }

    void writeEscaped(const QString& str) override { XMLWriter::writeEscaped(str); };

    void writeEscaped(const QChar& ch) override { XMLWriter::writeEscaped(ch); };

    void writeNewline() override { XMLWriter::writeNewline(); }

    void writeLiteral(const QString& str) override { XMLWriter::writeLiteral(str); }

    void writeLiteral(const char* str) override { XMLWriter::writeLiteral(str); }

    void writeLiteral(char ch) override { XMLWriter::writeLiteral(ch); }
};


struct TestFixture {
    TestFixture() : writer(stream) {
        stream.exceptions(std::ios::failbit | std::ios::badbit);
    }

    void clear() { stream.str(""); }

    std::ostringstream stream;
    TestXMLWriter writer;
};


class XMLWriterTest : public QObject {

Q_OBJECT

private slots:
    void init();
    [[maybe_unused]] void testWriteLiteralChar();
    [[maybe_unused]] void testWriteLiteralString();
    [[maybe_unused]] void testWriteLiteralQString();
    [[maybe_unused]] void testWriteNewline();
    [[maybe_unused]] void testWriteEscapedChar();
    [[maybe_unused]] void testWriteEscapedString();
    [[maybe_unused]] void testWriteQuoted();
    [[maybe_unused]] void testStartEndDocument();
    [[maybe_unused]] void testElements();
    [[maybe_unused]] void testAttributes();
    [[maybe_unused]] void testDoctype();
    [[maybe_unused]] void testCharacters();

private:
    std::unique_ptr<TestFixture> fixture;
};


void XMLWriterTest::init()
{
    fixture.reset(new TestFixture());     // NOLINT(modernize-make-unique)
}


void XMLWriterTest::testWriteLiteralChar() {
    fixture->clear();
    fixture->writer.writeLiteral('a');
    QCOMPARE(fixture->stream.str().data(), "a");

    fixture->clear();
    fixture->writer.writeLiteral(' ');
    QCOMPARE(fixture->stream.str().data(), " ");

    fixture->clear();
    fixture->writer.writeLiteral('<');
    QCOMPARE(fixture->stream.str().data(), "<");

    fixture->clear();
    fixture->writer.writeLiteral('>');
    QCOMPARE(fixture->stream.str().data(), ">");

    fixture->clear();
    fixture->writer.writeLiteral('\x99');
    QCOMPARE(fixture->stream.str().data(), "\x99");
}

void XMLWriterTest::testWriteLiteralString() {
    fixture->clear();
    fixture->writer.writeLiteral("a");
    QCOMPARE(fixture->stream.str().data(), "a");

    fixture->clear();
    fixture->writer.writeLiteral(" ");
    QCOMPARE(fixture->stream.str().data(), " ");

    fixture->clear();
    fixture->writer.writeLiteral("<");
    QCOMPARE(fixture->stream.str().data(), "<");

    fixture->clear();
    fixture->writer.writeLiteral(">");
    QCOMPARE(fixture->stream.str().data(), ">");

    fixture->clear();
    fixture->writer.writeLiteral("A bc<>&\"'");
    QCOMPARE(fixture->stream.str().data(), "A bc<>&\"'");

    fixture->clear();
    fixture->writer.writeLiteral("\u2122\u2026");
    QCOMPARE(fixture->stream.str().data(), "\u2122\u2026");
}

void XMLWriterTest::testWriteLiteralQString() {
    fixture->clear();
    fixture->writer.writeLiteral(QString("a"));
    QCOMPARE(fixture->stream.str().data(), "a");

    fixture->clear();
    fixture->writer.writeLiteral(QString(" "));
    QCOMPARE(fixture->stream.str().data(), " ");

    fixture->clear();
    fixture->writer.writeLiteral(QString("<"));
    QCOMPARE(fixture->stream.str().data(), "<");

    fixture->clear();
    fixture->writer.writeLiteral(QString(">"));
    QCOMPARE(fixture->stream.str().data(), ">");

    fixture->clear();
    fixture->writer.writeLiteral(QString("A bc<>&\"'"));
    QCOMPARE(fixture->stream.str().data(), "A bc<>&\"'");

    fixture->clear();
    fixture->writer.writeLiteral(QString("\u2122\u2026"));
    QCOMPARE(fixture->stream.str().data(), "\u2122\u2026");
}

void XMLWriterTest::testWriteNewline() {
    fixture->writer.writeNewline();
    QCOMPARE(fixture->stream.str().data(), "\n");
}

void XMLWriterTest::testWriteEscapedChar() {
    fixture->writer.writeEscaped('a');
    QCOMPARE(fixture->stream.str().data(), "a");

    fixture->clear();
    fixture->writer.writeEscaped('\n');
    QCOMPARE(fixture->stream.str().data(), "\n");

    fixture->clear();
    fixture->writer.writeEscaped('\t');
    QCOMPARE(fixture->stream.str().data(), "\t");

    fixture->clear();
    fixture->writer.writeEscaped('\r');
    QCOMPARE(fixture->stream.str().data(), "\r");

    fixture->clear();
    fixture->writer.writeEscaped('&');
    QCOMPARE(fixture->stream.str().data(), "&amp;");

    fixture->clear();
    fixture->writer.writeEscaped('<');
    QCOMPARE(fixture->stream.str().data(), "&lt;");

    fixture->clear();
    fixture->writer.writeEscaped('>');
    QCOMPARE(fixture->stream.str().data(), "&gt;");

    fixture->clear();
    fixture->writer.writeEscaped('"');
    QCOMPARE(fixture->stream.str().data(), "&quot;");

    fixture->clear();
    fixture->writer.writeEscaped('\'');
    QCOMPARE(fixture->stream.str().data(), "&apos;");

    fixture->clear();
    fixture->writer.writeEscaped('\x99');
    QCOMPARE(fixture->stream.str().data(), "&#153;");

    fixture->clear();
    fixture->writer.writeEscaped('\x1A');
    QCOMPARE(fixture->stream.str().data(), "ctrl-26");

    fixture->clear();
    fixture->writer.writeEscaped(u'\uE010');
    QCOMPARE(fixture->stream.str().data(), "&#57360;");
}

void XMLWriterTest::testWriteEscapedString() {
    fixture->writer.writeEscaped("a\n\t\r&<>\"'");
    QCOMPARE(fixture->stream.str().data(), "a\n\t\r&amp;&lt;&gt;&quot;&apos;");

    fixture->clear();
    fixture->writer.writeEscaped("\u2122\u2026");
    QCOMPARE(fixture->stream.str().data(), "&#8482;&#8230;");
}

void XMLWriterTest::testWriteQuoted() {
    fixture->writer.writeQuoted("abcd &efg");
    QCOMPARE(fixture->stream.str().data(), "\"abcd &amp;efg\"");
}

void XMLWriterTest::testStartEndDocument() {
    fixture->writer.startDocument().endDocument();
    QCOMPARE(fixture->stream.str().data(), "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n");
}

void XMLWriterTest::testElements() {
    fixture->writer.startDocument()
                   .startElement("elem1")
                   .startElement("elem2")
                   .endElement()
                   .endElement()
                   .endDocument();
    QCOMPARE(fixture->stream.str().data(), R"|(<?xml version="1.0" encoding="UTF-8"?>
<elem1>
    <elem2/>
</elem1>
)|");
}

void XMLWriterTest::testAttributes() {
    fixture->writer.startDocument()
                   .startElement("elem")
                   .addAttribute("attr1", "abc")
                   .addAttribute("attr2", 2)
                   .addAttribute("attr3", 3.5)
                   .endElement()
                   .endDocument();
    QCOMPARE(fixture->stream.str().data(), R"|(<?xml version="1.0" encoding="UTF-8"?>
<elem attr1="abc" attr2="2" attr3="3.5"/>
)|");
}

void XMLWriterTest::testDoctype() {
    fixture->writer.startDocument()
                   .doctype("elem", "http://www.cthing.com/dtd/PositionLog1.dtd")
                   .startElement("elem")
                   .endElement()
                   .endDocument();
    QCOMPARE(fixture->stream.str().data(), R"|(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE elem SYSTEM "http://www.cthing.com/dtd/PositionLog1.dtd">
<elem/>
)|");
}

void XMLWriterTest::testCharacters() {
    fixture->writer.startDocument()
                   .startElement("elem")
                   .characters("Hello\n\nWorld")
                   .endElement()
                   .endDocument();
    QCOMPARE(fixture->stream.str().data(), "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<elem>Hello\n\nWorld</elem>\n");
}


QTEST_MAIN(XMLWriterTest)

#include "XMLWriterTest.moc"
