/********************************************************************
    Copyright (c) 2013-2014 - QSanguosha-Hegemony Team

    This file is part of QSanguosha-Hegemony.

    This game is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3.0 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    See the LICENSE file for more details.

    QSanguosha-Hegemony Team
    *********************************************************************/

#ifndef FREECHOOSEDIALOG_H
#define FREECHOOSEDIALOG_H

#include <QDialog>
#include <QButtonGroup>

class General;

class FreeChooseDialog : public QDialog {
    Q_OBJECT
    Q_ENUMS(ButtonGroupType)

public:
    enum ButtonGroupType { Exclusive, Pair, Multi };

    explicit FreeChooseDialog(QWidget *parent, ButtonGroupType type = Exclusive);

private:
    QButtonGroup *group;
    ButtonGroupType type;
    QWidget *createTab(const QList<const General *> &generals);

private slots:
    void chooseGeneral();
    void disableButtons(QAbstractButton *button);

signals:
    void general_chosen(const QString &name);
    void pair_chosen(const QString &first, const QString &second);
};

#endif // FREECHOOSEDIALOG_H

