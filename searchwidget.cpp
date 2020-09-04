#include "searchwidget.h"

SearchWidget::SearchWidget(QWidget *parent) : QWidget(parent), active(false)
{
    input=new DSearchEdit(this);
    result=new SearchResult(this);
    auto layout=new QVBoxLayout(this);
    layout->addWidget(input);
    layout->addWidget(result);
    layout->setMargin(0);
    result->setVisible(false);
    setLayout(layout);
    setAttribute(Qt::WA_TranslucentBackground);

    connect(input,&DSearchEdit::textEdited,this,&SearchWidget::changeSearchResult);
    connect(input,&DSearchEdit::focusChanged,this,&SearchWidget::reset);
    connect(result,&SearchResult::widgetMoving,this,&SearchWidget::keepPopupPosition);
    connect(result,&SearchResult::changeCurrent,[this](const QModelIndex &cur){emit changeCurrent(cur);});
}
void SearchWidget::changeSearchResult(const QString &text)
{
    if(!active)
    {
        result->setFixedWidth(input->width());
        result->move(mapToGlobal(QPoint(0,input->height())));
        result->setVisible(true);
        active = true;
    }
    if(text.length()) result->filter(text);
}
void SearchWidget::reset(bool b)
{
    if(active && b) return ;
    else if(b) return changeSearchResult("");
    result->setVisible(false);
    active = false;
}
void SearchWidget::moveEvent(QMoveEvent *event)
{
    keepPopupPosition();
    QWidget::moveEvent(event);
}
void SearchWidget::resizeEvent(QResizeEvent *event)
{
    if(active)
    {
        result->setFixedWidth(input->width());
    }
    QWidget::resizeEvent(event);
}
void SearchWidget::keepPopupPosition()
{
    if(!active)return ;
    result->blockSignals(true);
    result->move(mapToGlobal(QPoint(0,input->height())));
    result->blockSignals(false);
}
void SearchWidget::setSourceModel(QAbstractItemModel *m)
{
    result->setSrcModel(m);
}
