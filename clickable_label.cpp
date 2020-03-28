#include "clickable_label.h"

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent)
{
    QObject::connect(this, &ClickableLabel::DoubleClicked,
                     this, &ClickableLabel::RecieveDoubleClick);
}


ClickableLabel::~ClickableLabel() {}


void
ClickableLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit DoubleClicked();
}


void
ClickableLabel::RecieveDoubleClick()
{
    std::cout << "recieved double click" << std::endl;
}
