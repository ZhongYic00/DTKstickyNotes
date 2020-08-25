#include "editor.h"

Editor::Editor(QWidget *parent):QWidget(parent)
{
    textEditor=new ZTextEdit(this);
    textEditor->setObjectName("textEditor");
    connect(textEditor,&ZTextEdit::textChanged,this,&Editor::sendContentChanged);

    toolBar=initToolBar();

    QStackedLayout *layout=new QStackedLayout(this);
    layout->setStackingMode(QStackedLayout::StackAll);
    layout->addWidget(toolBar);
    layout->setAlignment(toolBar,Qt::AlignBottom);
    layout->addWidget(textEditor);
    textEditor->setFocus();
    setLayout(layout);
}
QWidget* Editor::initToolBar()
{
    auto *layer=new TransparentWidget(this);
    auto toolBox=initToolButtons(layer);
    layer->setObjectName("toolBarLayer");
    QVBoxLayout *layout=new QVBoxLayout(layer);
    layer->setLayout(layout);
    layout->addStretch();

    QHBoxLayout *bar=new QHBoxLayout(layer);
    layout->addLayout(bar);
    layout->addSpacing(5);

    bar->addStretch();
    bar->addWidget(toolBox);
    bar->addStretch();

    return layer;
}
QWidget* Editor::initToolButtons(QWidget *parent)
{
    QWidget *box=new QWidget(parent);
    box->setObjectName("toolBarBox");
    box->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    QHBoxLayout *layout=new QHBoxLayout(box);
    box->setLayout(layout);

    layout->setMargin(15);

    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(box);
    effect->setOffset(0,0);
    effect->setColor(QColor(0,0,0,50));
    effect->setBlurRadius(15);
    box->setGraphicsEffect(effect);

    auto bold=new QPushButton(QIcon(":/images/font-bold"),"",box),
            italic=new QPushButton(QIcon(":/images/font-italic"),"",box),
            underline=new QPushButton(QIcon(":/images/font-underline"),"",box),
            linethrough=new QPushButton(QIcon(":/images/font-linethrough"),"",box),
            splitline=new QPushButton(QIcon(":/images/splitline"),"",box),
            image=new QPushButton(QIcon(":/images/insimage"),"",box),
            url=new QPushButton(QIcon(":images/insurl"),"",box),
            list=new QPushButton(QIcon(":images/inslist"),"",box),
            olist=new QPushButton(QIcon(":images/insorderedlist"),"",box),
            code=new QPushButton(QIcon(":images/inscode"),"",box);
    for(auto i:{bold,italic,underline,linethrough,url})
        i->setCheckable(true);
    connect(bold,&QPushButton::toggled,textEditor,&ZTextEdit::fBold);
    connect(textEditor,&ZTextEdit::fBoldState,bold,&QPushButton::setChecked);
    connect(italic,&QPushButton::toggled,textEditor,&ZTextEdit::fItalic);
    connect(textEditor,&ZTextEdit::fItalicState,italic,&QPushButton::setChecked);
    connect(underline,&QPushButton::toggled,textEditor,&ZTextEdit::fUnderline);
    connect(textEditor,&ZTextEdit::fUnderlineState,underline,&QPushButton::setChecked);
    connect(linethrough,&QPushButton::toggled,textEditor,&ZTextEdit::fLinethrough);
    connect(textEditor,&ZTextEdit::fLinethroughState,linethrough,&QPushButton::setChecked);
    connect(image,&QPushButton::clicked,textEditor,&ZTextEdit::pInsImage);
    connect(url,&QPushButton::clicked,textEditor,&ZTextEdit::pInsUrl);
    connect(url,&QPushButton::toggled,textEditor,&ZTextEdit::fUrl);
    connect(textEditor,&ZTextEdit::pIsUrl,url,&QPushButton::setChecked);
    connect(list,&QPushButton::clicked,textEditor,&ZTextEdit::pInsList);
    connect(olist,&QPushButton::clicked,textEditor,&ZTextEdit::pInsOrderedList);
    connect(code,&QPushButton::clicked,textEditor,&ZTextEdit::pInsCode);
    connect(splitline,&QPushButton::clicked,textEditor,&ZTextEdit::pSplitline);

    auto newAction=[this](QKeySequence shortcut,bool b=true){
        auto rt=new QAction;
        rt->setCheckable(b);
        rt->setShortcut(shortcut);
        this->addAction(rt);
        return rt;
    };

    connect(newAction(QKeySequence::Bold),&QAction::toggled,textEditor,&ZTextEdit::fBold);
    connect(newAction(QKeySequence::Underline),&QAction::toggled,textEditor,&ZTextEdit::fUnderline);
    connect(newAction(QKeySequence::Italic),&QAction::toggled,textEditor,&ZTextEdit::fItalic);

    layout->addWidget(bold),layout->addWidget(italic),layout->addWidget(underline),layout->addWidget(linethrough),layout->addWidget(splitline),layout->addWidget(image),layout->addWidget(url),layout->addWidget(list),layout->addWidget(olist),layout->addWidget(code);
    return box;
}
void Editor::display(const QString &html)
{
    textEditor->setHtml(html);
    textEditor->setFocus();
    this->setEnabled(true);
}
void Editor::reset()
{
//    qDebug()<<"call Editor::reset()"<<textEditor;
    textEditor->clear();
    this->setEnabled(false);
//    qDebug()<<"Editor::reset() return";
}
void Editor::sendContentChanged()
{
    emit contentChanged(textEditor->toPlainText().left(100));
}
QString Editor::getContentRich() const
{
    return textEditor->toHtml();
}

TransparentWidget::TransparentWidget(QWidget *p):QWidget (p){}
void TransparentWidget::updateMask()
{
    setMask(QRegion(childrenRegion()));
}
void TransparentWidget::resizeEvent(QResizeEvent *e)
{
    updateMask();
    return QWidget::resizeEvent(e);
}
bool TransparentWidget::event(QEvent *e)
{
    if(e->type()==QEvent::WindowActivate)updateMask();
    return QWidget::event(e);
}
