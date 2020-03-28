#ifndef CLICKABLE_LABEL_H
#define CLICKABLE_LABEL_H

#include <QWidget>
#include <QLabel>

#include <iostream>


class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickableLabel();

signals:
    void DoubleClicked();

public slots:
    void RecieveDoubleClick();

protected:
    void mouseDoubleClickEvent(QMouseEvent* event);
};

#endif // CLICKABLE_LABEL_H
