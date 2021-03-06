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

#ifndef GRAPHICSBOX_H
#define GRAPHICSBOX_H

#include <QGraphicsObject>

class GraphicsBox : public QGraphicsObject {
    Q_OBJECT

public :
    explicit GraphicsBox(const QString &title = QString());
    virtual ~GraphicsBox();

    static void paintGraphicsBoxStyle(QPainter *painter, const QString &title, const QRectF &rect);
    static void stylize(QGraphicsObject *target);
    static void moveToCenter(QGraphicsObject *target);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const = 0;

    void moveToCenter();

    QString title;
};

#endif // GRAPHICSBOX_H
