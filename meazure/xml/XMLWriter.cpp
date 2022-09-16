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

#include "XMLWriter.h"
#include <meazure/utils/StringUtils.h>


const char* XMLWriter::kIndent = "    ";


void XMLWriter::flush() {
    m_out.flush();
}

XMLWriter& XMLWriter::startDocument() {
    handleEvent(Event::StartDocument);

    writeLiteral(u8"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    writeNewline();
    return *this;
}

XMLWriter& XMLWriter::endDocument() {
    handleEvent(Event::EndDocument);

    writeNewline();
    flush();

    return *this;
}

XMLWriter& XMLWriter::doctype(const QString& name, const QString& systemId) {
    writeLiteral(u8"<!DOCTYPE ");
    writeLiteral(name);
    writeLiteral(u8" SYSTEM \"");
    writeLiteral(systemId);
    writeLiteral(u8"\">");
    writeNewline();

    return *this;
}

XMLWriter& XMLWriter::startElement(const QString& name) {
    State previousState = handleEvent(Event::StartElement);

    ElementPtr element = std::make_shared<Element>(name, previousState);
    m_elementStack.push(element);

    return *this;
}

XMLWriter& XMLWriter::endElement() {
    handleEvent(Event::EndElement);

    return *this;
}

XMLWriter& XMLWriter::addAttribute(const QString& name, const QString& value) {
    handleEvent(Event::Attribute);

    AttributePtr attribute = std::make_shared<Attribute>(name, value);
    m_elementStack.top()->m_attributes.push_back(attribute);

    return *this;
}

XMLWriter& XMLWriter::addAttribute(const QString& name, int value) {
    addAttribute(name, QString::number(value));

    return *this;
}

XMLWriter& XMLWriter::addAttribute(const QString& name, unsigned int value) {
    addAttribute(name, QString::number(value));

    return *this;
}

XMLWriter& XMLWriter::addAttribute(const QString& name, double value) {
    addAttribute(name, StringUtils::dblToStr(value));

    return *this;
}

XMLWriter& XMLWriter::characters(const QString& str) {
    handleEvent(Event::Characters);

    writeEscaped(str);

    return *this;
}

XMLWriter::State XMLWriter::handleEvent(Event event) {
    State previousState = m_currentState;
    bool invalidEvent = false;

    switch (m_currentState) {
        case State::BeforeDoc:
            if (event == Event::StartDocument) {
                m_currentState = State::BeforeRoot;
            } else {
                invalidEvent = true;
            }
            break;
        case State::BeforeRoot:
            switch (event) {
                case Event::Characters:
                    break;
                case Event::StartElement:
                    m_currentState = State::InStartTag;
                    break;
                case Event::EndDocument:
                    m_currentState = State::AfterDoc;
                    break;
                default:
                    invalidEvent = true;
                    break;
            }
            break;
        case State::InStartTag:
            switch (event) {
                case Event::Attribute:
                    break;
                case Event::Characters:
                    writeStartElement(false);
                    m_currentState = State::AfterData;
                    break;
                case Event::StartElement:
                    writeStartElement(false);
                    m_currentState = State::InStartTag;
                    break;
                case Event::EndElement:
                    writeStartElement(true);
                    m_currentState = (m_elementStack.size() == 0) ? State::AfterRoot : State::AfterTag;
                    break;
                case Event::EndDocument:
                    writeStartElement(true);
                    m_currentState = State::AfterDoc;
                    break;
                default:
                    invalidEvent = true;
                    break;
            }
            break;
        case State::AfterTag:
            switch (event) {
                case Event::Characters:
                    m_currentState = State::AfterData;
                    break;
                case Event::StartElement:
                    m_currentState = State::InStartTag;
                    break;
                case Event::EndElement:
                    writeEndElement();
                    m_currentState = (m_elementStack.size() == 0) ? State::AfterRoot : State::AfterTag;
                    break;
                default:
                    invalidEvent = true;
                    break;
            }
            break;
        case State::AfterData:
            switch (event) {
                case Event::Characters:
                    break;
                case Event::StartElement:
                    m_currentState = State::InStartTag;
                    break;
                case Event::EndElement:
                    writeEndElement();
                    m_currentState = (m_elementStack.size() == 0) ? State::AfterRoot : State::AfterTag;
                    break;
                default:
                    invalidEvent = true;
                    break;
            }
            break;
        case State::AfterRoot:
            switch (event) {
                case Event::Characters:
                    break;
                case Event::EndDocument:
                    m_currentState = State::AfterDoc;
                    break;
                default:
                    invalidEvent = true;
                    break;
            }
            break;
        case State::AfterDoc:
            invalidEvent = true;
            break;
        default:
            QString msg("Unrecognized state " + getStateName(m_currentState));
            throw std::ios::failure(msg.toStdString());
    }

    if (invalidEvent) {
        QString msg = QString("Event %1 not allowed in state %2")
                .arg(getEventName(event))
                .arg(getStateName(m_currentState));
        throw std::ios::failure(msg.toStdString());
    }

    return previousState;
}

QString XMLWriter::getStateName(State state) {
    switch (state) {
        case State::BeforeDoc:
            return "BeforeDoc";
        case State::BeforeRoot:
            return "BeforeRoot";
        case State::InStartTag:
            return "InStartTag";
        case State::AfterTag:
            return "AfterTag";
        case State::AfterData:
            return "AfterData";
        case State::AfterRoot:
            return "AfterRoot";
        case State::AfterDoc:
            return "AfterDoc";
        default:
            assert(false);
            return "";
    }
}

QString XMLWriter::getEventName(Event event) {
    switch (event) {
        case Event::Attribute:
            return "Attribute";
        case Event::Characters:
            return "Characters";
        case Event::EndDocument:
            return "EndDocument";
        case Event::EndElement:
            return "EndElement";
        case Event::StartDocument:
            return "StartDocument";
        case Event::StartElement:
            return "StartElement";
        default:
            assert(false);
            return "";
    }
}

void XMLWriter::writeStartElement(bool isEmpty) {
    ElementPtr element = m_elementStack.top();

    if ((element->m_state != State::AfterData) && (m_elementStack.size() > 1)) {
        writeNewline();
        writeIndent();
    }

    writeLiteral('<');
    writeLiteral(element->m_name);
    writeAttributes(element->m_attributes);
    writeLiteral(isEmpty ? u8"/>" : u8">");

    // If this is an empty tag, act like an end tag has been specified.
    if (isEmpty) {
        m_elementStack.pop();
    }
}

void XMLWriter::writeEndElement() {
    if (m_currentState != State::AfterData) {
        writeNewline();
        writeIndent();
    }

    ElementPtr element = m_elementStack.top();

    writeLiteral(u8"</");
    writeLiteral(element->m_name);
    writeLiteral('>');

    m_elementStack.pop();
}

void XMLWriter::writeAttributes(Attributes attributes) {   // NOLINT(performance-unnecessary-value-param)
    for (AttributePtr attribute : attributes) {            // NOLINT(performance-for-range-copy)
        writeLiteral(' ');
        writeLiteral(attribute->m_name);
        writeLiteral('=');
        writeQuoted(attribute->m_value);
    }
}

void XMLWriter::writeIndent() {
    std::size_t level = m_elementStack.size() - 1;
    while (level-- > 0) {
        writeLiteral(kIndent);
    }
}

void XMLWriter::writeQuoted(const QString& str) {
    writeLiteral(u'"');
    writeEscaped(str);
    writeLiteral(u'"');
}

void XMLWriter::writeEscaped(const QString& str) {
    if (str != nullptr) {
        qsizetype len = str.size();
        for (qsizetype i = 0; i < len; i++) {
            writeEscaped(str[i]);
        }
    }
}

void XMLWriter::writeEscaped(const QChar& ch) {
    switch (ch.unicode()) {
        case u'&':
            writeLiteral(u8"&amp;");
            break;
        case u'<':
            writeLiteral(u8"&lt;");
            break;
        case u'>':
            writeLiteral(u8"&gt;");
            break;
        case u'\'':
            writeLiteral(u8"&apos;");
            break;
        case u'\"':
            writeLiteral(u8"&quot;");
            break;
        case u'\n':
            writeNewline();
            break;
        case u'\t':
            writeLiteral('\t');
            break;
        case u'\r':
            writeLiteral('\r');
            break;
        default:
            if (ch > u'\u001F' && ch < u'\u007F') {
                writeLiteral(ch);
            } else {
                if ((ch >= u'\u007F' && ch <= u'\uD7FF') || (ch >= u'\uE000' && ch <= u'\uFFFD')) {
                    writeLiteral("&#");
                    m_out << ch.unicode();
                    writeLiteral(';');
                } else {
                    writeLiteral("ctrl-");
                    m_out << ch.unicode();
                }
            }
            break;
    }
}

void XMLWriter::writeNewline() {
    writeLiteral('\n');
}

void XMLWriter::writeLiteral(const QString& str) {
    writeLiteral(str.toUtf8().constData());
}

void XMLWriter::writeLiteral(const char* str) {
    m_out << str;
}

void XMLWriter::writeLiteral(char ch) {
    m_out << ch;
}
