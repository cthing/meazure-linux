/*
 * Copyright 2023 C Thing Software
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

#include "PosLogManageDlg.h"
#include <meazure/utils/LayoutUtils.h>
#include <meazure/utils/HelpUtils.h>
#include <QGridLayout>
#include <QSignalBlocker>
#include <QWhatsThis>


PosLogManageDlg::PosLogManageDlg(PosLogMgr* posLogMgr, QWidget* parent) :     // NOLINT(cppcoreguidelines-pro-type-member-init)
        QDialog(parent),
        m_posLogMgr(posLogMgr) {
    setWindowTitle(tr("Positions"));

    createUI();
    configure();

    posLogMgr->refresh();
    positionSelected(m_currentPositionIndex);
}

void PosLogManageDlg::createUI() {
    using namespace LayoutUtils;        // NOLINT(google-build-using-namespace)

    auto* logLabel = new QLabel(tr("<b>Log</b>"));
    auto* logTitleLabel = new QLabel(tr("Title:"));
    m_logTitleField = new QLineEdit();
    m_logTitleField->setWhatsThis(tr("Title for the position log."));
    auto* logDescLabel = new QLabel(tr("Description:"));
    m_logDescField = new TextEdit();
    m_logDescField->setRows(k_numDescRows);
    m_logDescField->setWhatsThis(tr("Description for the position log."));

    auto* positionLabel = new QLabel(tr("<b>Positions</b>"));
    m_positionSelector = new QScrollBar(Qt::Horizontal);
    m_positionSelector->setSingleStep(1);
    m_positionSelector->setPageStep(5);
    m_positionSelector->setRange(0, 0);
    m_positionSelector->setStyleSheet("QScrollBar:horizontal { border: 1px solid darkGrey; }");
    m_positionSelector->setToolTip(tr("Replay a logged position."));
    m_positionSelector->setWhatsThis(tr("Replays a logged position."));
    m_positionLabel = new QLabel(tr("Position:"));
    m_positionNumberLabel = new QLabel();
    m_recordedLabel = new QLabel(tr("Recorded:"));
    m_recordedTimestampLabel = new QLabel();
    m_positionDescLabel = new QLabel(tr("Description:"));
    m_positionDescField = new TextEdit();
    m_positionDescField->setRows(k_numDescRows);
    m_positionDescField->setWhatsThis(tr("Description for the position."));

    m_addButton = new QPushButton(tr("Add"));
    m_addButton->setToolTip(tr("Add new position"));
    m_addButton->setWhatsThis(tr("Records the active position and adds it to the end of the recorded positions."));

    m_insertButton = new QPushButton(tr("Insert"));
    m_insertButton->setToolTip(tr("Insert new position"));
    m_insertButton->setWhatsThis(tr("Records the active position and inserts it before the current log position."));

    m_deleteButton = new QPushButton(tr("Delete"));
    m_deleteButton->setToolTip(tr("Delete position"));
    m_deleteButton->setWhatsThis(tr("Deletes the current log position."));

    m_deleteAllButton = new QPushButton(tr("Delete All"));
    m_deleteAllButton->setToolTip(tr("Delete all positions"));
    m_deleteAllButton->setWhatsThis(tr("Deletes all log positions."));

    m_loadButton = new QPushButton(tr("Load..."));
    m_loadButton->setToolTip(tr("Load a position log file"));
    m_loadButton->setWhatsThis(tr("Loads a position log file."));

    m_saveButton = new QPushButton(tr("Save"));
    m_saveButton->setToolTip(tr("Save position log to file"));
    m_saveButton->setWhatsThis(tr("Saves the position log to a file."));

    m_saveAsButton = new QPushButton(tr("Save As..."));
    m_saveAsButton->setToolTip(tr("Save position log to file"));
    m_saveAsButton->setWhatsThis(tr("Saves the position log to a file."));

    m_closeButton = new QPushButton(tr("Close"));
    m_closeButton->setToolTip(tr("Close the dialog"));
    m_closeButton->setWhatsThis(tr("Closes the dialog."));

    m_helpButton = new QPushButton(HelpUtils::getHelpIcon(style()), "");

    auto* logLayout = new QGridLayout();
    logLayout->addWidget(logLabel,        k_row0, k_col0, k_rowspan1, k_colspan2);
    logLayout->addWidget(logTitleLabel,   k_row1, k_col0, Qt::AlignRight);
    logLayout->addWidget(m_logTitleField, k_row1, k_col1);
    logLayout->addWidget(logDescLabel,    k_row2, k_col0, Qt::AlignRight | Qt::AlignTop);
    logLayout->addWidget(m_logDescField,  k_row2, k_col1);

    auto* positionLayout = new QGridLayout();
    positionLayout->addWidget(positionLabel,            k_row0, k_col0, k_rowspan1, k_colspan2);
    positionLayout->addWidget(m_positionSelector,       k_row1, k_col0, k_rowspan1, k_colspan2);
    positionLayout->addWidget(m_positionLabel,          k_row2, k_col0, Qt::AlignRight);
    positionLayout->addWidget(m_positionNumberLabel,    k_row2, k_col1);
    positionLayout->addWidget(m_recordedLabel,          k_row3, k_col0, Qt::AlignRight);
    positionLayout->addWidget(m_recordedTimestampLabel, k_row3, k_col1);
    positionLayout->addWidget(m_positionDescLabel,      k_row4, k_col0, Qt::AlignRight | Qt::AlignTop);
    positionLayout->addWidget(m_positionDescField,      k_row4, k_col1);

    auto* buttonLayout = new QGridLayout();
    buttonLayout->addWidget(m_helpButton,      k_row0, k_col0, k_rowspan2, k_colspan1);
    buttonLayout->addWidget(m_addButton,       k_row0, k_col1);
    buttonLayout->addWidget(m_insertButton,    k_row0, k_col2);
    buttonLayout->addWidget(m_deleteButton,    k_row0, k_col3);
    buttonLayout->addWidget(m_deleteAllButton, k_row0, k_col4);
    buttonLayout->addWidget(m_loadButton,      k_row1, k_col1);
    buttonLayout->addWidget(m_saveButton,      k_row1, k_col2);
    buttonLayout->addWidget(m_saveAsButton,    k_row1, k_col3);
    buttonLayout->addWidget(m_closeButton,     k_row1, k_col4);

    auto* layout = new QVBoxLayout();
    layout->addLayout(logLayout);
    layout->addSpacing(10);
    layout->addLayout(positionLayout);
    layout->addSpacing(15);
    layout->addLayout(buttonLayout);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);
}

void PosLogManageDlg::configure() {
    connect(m_posLogMgr, &PosLogMgr::positionsLoaded, this, &PosLogManageDlg::positionsLoaded);
    connect(m_posLogMgr, &PosLogMgr::positionsChanged, this, &PosLogManageDlg::positionsChanged);
    connect(m_posLogMgr, &PosLogMgr::positionAdded, this, &PosLogManageDlg::positionAdded);

    connect(m_logTitleField, &QLineEdit::textEdited, m_posLogMgr, &PosLogMgr::changeTitle);
    connect(m_logDescField, &QPlainTextEdit::textChanged, this, [this]() {
        m_posLogMgr->changeDescription(m_logDescField->toPlainText());
    });

    connect(m_positionSelector, &QScrollBar::valueChanged, this, &PosLogManageDlg::positionSelected);
    connect(m_positionDescField, &QPlainTextEdit::textChanged, this, [this]() {
        m_posLogMgr->changePositionDescription(m_currentPositionIndex, m_positionDescField->toPlainText());
    });

    connect(m_addButton, &QPushButton::clicked, m_posLogMgr, &PosLogMgr::addPosition);
    connect(m_insertButton, &QPushButton::clicked, this, [this]() {
        m_posLogMgr->insertPosition(m_currentPositionIndex);
    });
    connect(m_deleteButton, &QPushButton::clicked, this, [this]() {
        m_posLogMgr->deletePosition(m_currentPositionIndex);
    });
    connect(m_deleteAllButton, &QPushButton::clicked, m_posLogMgr, &PosLogMgr::deletePositions);
    connect(m_loadButton, &QPushButton::clicked, m_posLogMgr, &PosLogMgr::loadPositions);
    connect(m_saveButton, &QPushButton::clicked, m_posLogMgr, &PosLogMgr::savePositions);
    connect(m_saveAsButton, &QPushButton::clicked, m_posLogMgr, &PosLogMgr::saveAsPositions);
    connect(m_closeButton, &QPushButton::clicked, this, &PosLogManageDlg::hide);
    connect(m_helpButton, &QPushButton::clicked, this, []() {
        QWhatsThis::enterWhatsThisMode();
    });
}

void PosLogManageDlg::positionsChanged(unsigned int numPositions) {
    const QSignalBlocker sliderBlocker(m_positionSelector);
    const bool havePositions = numPositions > 0;

    m_positionSelector->setEnabled(havePositions);
    m_positionLabel->setEnabled(havePositions);
    m_positionNumberLabel->setEnabled(havePositions);
    m_recordedLabel->setEnabled(havePositions);
    m_recordedTimestampLabel->setEnabled(havePositions);
    m_positionDescLabel->setEnabled(havePositions);
    m_positionDescField->setEnabled(havePositions);

    m_deleteButton->setEnabled(havePositions);
    m_deleteAllButton->setEnabled(havePositions);
    m_saveButton->setEnabled(havePositions);
    m_saveAsButton->setEnabled(havePositions);

    const unsigned int maxIndex = numPositions == 0 ? 0 : (numPositions - 1);

    m_positionSelector->setRange(0, static_cast<int>(maxIndex));

    if (m_currentPositionIndex > maxIndex) {
        positionSelected(maxIndex);
    } else {
        updatePositionInfo();
    }
}

void PosLogManageDlg::positionsLoaded() {
    const QSignalBlocker titleBlocker(m_logTitleField);
    const QSignalBlocker descBlocker(m_logDescField);

    m_logTitleField->setText(m_posLogMgr->getTitle());
    m_logDescField->setPlainText(m_posLogMgr->getDescription());

    positionAdded(0);
    positionSelected(0);
}

void PosLogManageDlg::positionAdded(unsigned int positionIndex) {
    const QSignalBlocker sliderBlocker(m_positionSelector);

    m_currentPositionIndex = positionIndex;

    m_positionSelector->setValue(static_cast<int>(m_currentPositionIndex));

    updatePositionInfo();
}

void PosLogManageDlg::positionSelected(unsigned int positionIndex) {
    m_currentPositionIndex = positionIndex;

    if (positionIndex < m_posLogMgr->getNumPositions()) {
        m_posLogMgr->showPosition(m_currentPositionIndex);
    }

    updatePositionInfo();
}

void PosLogManageDlg::updatePositionInfo() {
    const QSignalBlocker descBlocker(m_positionDescField);

    const unsigned int numPositions = m_posLogMgr->getNumPositions();
    if (m_currentPositionIndex < numPositions) {
        m_positionNumberLabel->setText(tr("%1 of %2").arg(m_currentPositionIndex + 1).arg(numPositions));
        const QDateTime recorded(m_posLogMgr->getPositionRecorded(m_currentPositionIndex));
        m_recordedTimestampLabel->setText(recorded.toLocalTime().toString(Qt::TextDate));
        m_positionDescField->setPlainText(m_posLogMgr->getPositionDescription(m_currentPositionIndex));
    } else {
        m_positionNumberLabel->clear();
        m_recordedTimestampLabel->clear();
        m_positionDescField->clear();
    }
}
