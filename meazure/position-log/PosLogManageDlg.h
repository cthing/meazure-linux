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

#pragma once

#include "PosLogMgr.h"
#include <meazure/tools/ToolMgr.h>
#include <meazure/ui/fields/TextEdit.h>
#include <QDialog>
#include <QLineEdit>
#include <QScrollBar>
#include <QLabel>
#include <QPushButton>


class PosLogManageDlg : public QDialog {

    Q_OBJECT

public:
    PosLogManageDlg(PosLogMgr* posLogMgr, QWidget* parent);

private slots:
    void positionsLoaded();
    void positionsChanged(unsigned int numPositions);
    void positionAdded(unsigned int positionIndex);
    void positionSelected(unsigned int positionIndex);

private:
    static constexpr int k_numDescRows { 3 };

    void createUI();
    void configure();

    void updatePositionInfo();

    PosLogMgr* m_posLogMgr;
    QLineEdit* m_logTitleField;
    TextEdit* m_logDescField;
    QScrollBar* m_positionSelector;
    QLabel* m_positionLabel;
    QLabel* m_positionNumberLabel;
    QLabel* m_recordedLabel;
    QLabel* m_recordedTimestampLabel;
    QLabel* m_positionDescLabel;
    TextEdit* m_positionDescField;
    QPushButton* m_addButton;
    QPushButton* m_insertButton;
    QPushButton* m_deleteButton;
    QPushButton* m_deleteAllButton;
    QPushButton* m_loadButton;
    QPushButton* m_saveButton;
    QPushButton* m_saveAsButton;
    QPushButton* m_closeButton;
    QPushButton* m_helpButton;
    unsigned int m_currentPositionIndex { 0 };
};
