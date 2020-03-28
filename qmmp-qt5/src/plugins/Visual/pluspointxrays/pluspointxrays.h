/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (C) 2015 - 2020 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#ifndef PLUSPOINTXRAYS_H
#define PLUSPOINTXRAYS_H

#include <qmmp/visual.h>

class QTimer;
class QPainter;
class QPaintEvent;
class QHideEvent;
class QShowEvent;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class PlusPointXRays : public Visual
{
    Q_OBJECT
public:
    explicit PlusPointXRays(QWidget *parent = nullptr);
    virtual ~PlusPointXRays();

public slots:
    virtual void start() override;
    virtual void stop() override;

private slots:
    void timeout();
    void readSettings();
    void writeSettings();
    void changeColor();
    void changeGridState(bool state);

private:
    void clear();
    virtual void hideEvent(QHideEvent *e) override;
    virtual void showEvent(QShowEvent *e) override;
    virtual void paintEvent(QPaintEvent *) override;
    virtual void contextMenuEvent(QContextMenuEvent *e) override;

    void process();
    void draw(QPainter *p);

    QList<QColor> m_colors;
    QTimer *m_timer;
    float m_left_buffer[QMMP_VISUAL_NODE_SIZE];
    float m_right_buffer[QMMP_VISUAL_NODE_SIZE];
    bool m_running;
    int *m_intern_vis_data, m_cols, m_rows;
    QAction *m_screenAction;
    QAction *m_gridAction;

};

#endif