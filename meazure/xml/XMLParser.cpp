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

#include "XMLParser.h"
#include <QApplication>
#include <QByteArray>
#include <utility>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>


//*************************************************************************
// XMLAttributes
//*************************************************************************


XMLAttributes::XMLAttributes(const xercesc::AttributeList& atts) {
    for (XMLSize_t i = 0; i < atts.getLength(); i++) {
        const QString name = QString::fromUtf16(atts.getName(i));
        const QString value = QString::fromUtf16(atts.getValue(i));
        m_attributeMap[name] = value;
    }
}

bool XMLAttributes::getValueStr(const QString& name, QString& value) const {
    return getValueStr(name, [&value](const QString& v) { value = v; });
}

bool XMLAttributes::getValueStr(const QString& name, const std::function<void (const QString&)>& valueFunc) const {
    auto iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        valueFunc((*iter).second);
        return true;
    }
    return false;
}

bool XMLAttributes::getValueInt(const QString& name, int& value) const {
    return getValueInt(name, [&value](int v) { value = v; });
}

bool XMLAttributes::getValueInt(const QString& name, const std::function<void (int)>& valueFunc) const {
    auto iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        bool success = false;
        valueFunc(((*iter).second).toInt(&success));
        return success;
    }
    return false;
}

bool XMLAttributes::getValueUInt(const QString& name, unsigned int& value) const {
    return getValueUInt(name, [&value](unsigned int v) { value = v; });
}

bool XMLAttributes::getValueUInt(const QString& name, const std::function<void (unsigned int)>& valueFunc) const {
    auto iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        bool success = false;
        valueFunc(((*iter).second).toUInt(&success));
        return success;
    }
    return false;
}

bool XMLAttributes::getValueDbl(const QString& name, double& value) const {
    return getValueDbl(name, [&value](double v) { value = v; });
}

bool XMLAttributes::getValueDbl(const QString& name, const std::function<void (double)>& valueFunc) const {
    auto iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        bool success = false;
        valueFunc((*iter).second.toDouble(&success));
        return success;
    }
    return false;
}

bool XMLAttributes::getValueBool(const QString& name, bool& value) const {
    return getValueBool(name, [&value](bool v) { value = v; });
}

bool XMLAttributes::getValueBool(const QString& name, const std::function<void (bool)>& valueFunc) const {
    auto iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        const QString& vstr = (*iter).second;
        valueFunc(vstr == "true" || vstr == "1");
        return true;
    }
    return false;
}


//*************************************************************************
// XMLNode
//*************************************************************************


XMLNode::XMLNode() :
        m_type(Type::Unknown),
        m_parent(nullptr) {}

XMLNode::XMLNode(QString elementName, XMLAttributes attrs) :
        m_type(Type::Element),
        m_data(std::move(elementName)),
        m_attributes(std::move(attrs)),
        m_parent(nullptr) {}

XMLNode::XMLNode(QString data) :
        m_type(Type::Data),
        m_data(std::move(data)),
        m_parent(nullptr) {}

XMLNode::~XMLNode() {
    try {
        for (auto* child : m_children) {
            delete child;
        }
        m_children.clear();
    } catch (...) {
        assert(false);
    }
}

QString XMLNode::getChildData() const {
    QString data;

    for (const XMLNode* node : m_children) {
        if (node->getType() == XMLNode::Type::Data) {
            data += node->getData();
        }
    }

    return data;
}


std::ostream& operator<<(std::ostream& os, const XMLNode::Type& type){
    switch (type) {
        case XMLNode::Type::Element:
            os << "Element";
            break;
        case XMLNode::Type::Data:
            os << "Data";
            break;
        default:
            os << "Unknown";
            break;
    }

    return os;
}


std::ostream& operator<<(std::ostream& os, const XMLNode& node){
    static int indent = 0;

    const char* indentStr = QString(indent, ' ').toUtf8().constData();
    const XMLNode::Type type = node.getType();

    os << indentStr << "Node Type: " << type << '\n';
    if (type == XMLNode::Type::Element) {
        os << indentStr << "Name: " << node.getData().toUtf8().constData() << '\n';
    } else if (type == XMLNode::Type::Data) {
        os << indentStr << "Data: " << node.getData().toUtf8().constData() << '\n';
    }

    for (auto* child : node.m_children) {
        indent += 4;
        os << child;
        indent -= 4;
    }

    return os;
}


//*************************************************************************
// XMLParserHandler
//*************************************************************************


void XMLParserHandler::startElement(const QString&, const QString&, const XMLAttributes&) {}

void XMLParserHandler::endElement(const QString&, const QString&) {}

void XMLParserHandler::characterData(const QString&, const QString&) {}

xercesc::InputSource* XMLParserHandler::resolveEntity(const QString&) {
    return nullptr;
}

QString XMLParserHandler::getFilePathname() {
    return "";
}


//*************************************************************************
// XMLParser
//*************************************************************************


XMLParserHandler XMLParser::m_noopHandler;


XMLParser::XMLParser() : XMLParser(&m_noopHandler, true) {}

XMLParser::XMLParser(XMLParserHandler* handler, bool buildDOM) :
        m_handler(handler),
        m_buildDOM(buildDOM),
        m_dom(nullptr) {

    xercesc::XMLPlatformUtils::Initialize();

    m_parser = new xercesc::SAXParser();    // NOLINT(cppcoreguidelines-prefer-member-initializer)
    m_parser->setValidationScheme(xercesc::SAXParser::ValSchemes::Val_Auto);
    m_parser->setDocumentHandler(this);
    m_parser->setEntityResolver(this);
    m_parser->setErrorHandler(this);
}

XMLParser::~XMLParser() {
    try {
        delete m_parser;
        delete m_dom;

        xercesc::XMLPlatformUtils::Terminate();
    } catch (...) {
        assert(false);
    }
}

void XMLParser::parseFile(const QString& pathname) {
    m_parser->parse(pathname.toUtf8().constData());
}

void XMLParser::parseString(const QString& content) {
    const QByteArray data = content.toUtf8();
    const xercesc::MemBufInputSource source(reinterpret_cast<const XMLByte*>(data.constData()), data.size(), "XMLBuf");
    m_parser->parse(source);
}

void XMLParser::startDocument() {
}

void XMLParser::setDocumentLocator(const xercesc::Locator* const /*locator*/) {
}

void XMLParser::processingInstruction(const XMLCh* const /*target*/, const XMLCh* const /*data*/) {
}

void XMLParser::startElement(const XMLCh* const elementName, xercesc::AttributeList& attrs) {
    const XMLAttributes attributes(attrs);
    const QString name = QString::fromUtf16(elementName);
    QString container;

    if (!m_elementStack.empty()) {
        container = m_elementStack.top();
    }
    m_handler->startElement(container, name, attributes);
    m_elementStack.push(name);

    if (m_buildDOM) {
        auto* node = new XMLNode(name, attributes);
        if (m_nodeStack.empty()) {
            assert(m_dom == nullptr);
            m_dom = node;
        } else {
            m_nodeStack.top()->addChild(node);
        }
        m_nodeStack.push(node);
    }
}

void XMLParser::endElement(const XMLCh* const elementName) {
    const QString name = QString::fromUtf16(elementName);

    assert(!m_elementStack.empty());
    m_elementStack.pop();

    QString container;
    if (!m_elementStack.empty()) {
        container = m_elementStack.top();
    }
    m_handler->endElement(container, name);

    if (m_buildDOM && !m_nodeStack.empty()) {
        m_nodeStack.pop();
    }
}

void XMLParser::characters(const XMLCh* const chars, const XMLSize_t length) {
    QString container;
    if (!m_elementStack.empty()) {
        container = m_elementStack.top();
    }
    if (!container.isEmpty()) {
        const QString data = QString::fromUtf16(chars, static_cast<qsizetype>(length));
        m_handler->characterData(container, data);

        if (m_buildDOM && !m_nodeStack.empty()) {
            m_nodeStack.top()->addChild(new XMLNode(data));
        }
    }
}

void XMLParser::ignorableWhitespace(const XMLCh* const /*chars*/, const XMLSize_t /*length*/) {
    // Ignore the whitespace.
}

void XMLParser::endDocument() {
}

void XMLParser::resetDocument() {
    delete m_dom;
    m_dom = nullptr;

    while (!m_elementStack.empty()) {
        m_elementStack.pop();
    }

    while (!m_nodeStack.empty()) {
        m_nodeStack.pop();
    }

    while (!m_pathnameStack.empty()) {
        m_pathnameStack.pop();
    }
}

xercesc::InputSource* XMLParser::resolveEntity(const XMLCh* const /*publicId*/, const XMLCh* const systemId) {
    if (systemId != nullptr) {
        const QString sysId = QString::fromUtf16(systemId);

        m_pathnameStack.push(sysId);
        xercesc::InputSource* source = m_handler->resolveEntity(sysId);
        m_pathnameStack.pop();

        return source;
    }

    return nullptr;
}

void XMLParser::warning(const xercesc::SAXParseException& /*exc*/) {
    // Ignore warnings
}

void XMLParser::error(const xercesc::SAXParseException& exc) {
    const QString msg = QString::fromUtf16(exc.getMessage());
    const QString pathname(m_pathnameStack.empty() ? m_handler->getFilePathname() : m_pathnameStack.top());
    const int line = static_cast<int>(exc.getLineNumber());
    const int column = static_cast<int>(exc.getColumnNumber());

    throw XMLParsingException(msg, pathname, line, column);
}

void XMLParser::fatalError(const xercesc::SAXParseException& exc) {
    const QString msg = QString::fromUtf16(exc.getMessage());
    const QString pathname(m_pathnameStack.empty() ? m_handler->getFilePathname() : m_pathnameStack.top());
    const int line = static_cast<int>(exc.getLineNumber());
    const int column = static_cast<int>(exc.getColumnNumber());

    throw XMLParsingException(msg, pathname, line, column);
}

void XMLParser::resetErrors() {
}
