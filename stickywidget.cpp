#include "stickywidget.h"

StickyWidget::StickyWidget(QObject *parent)
{
    setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    initWidgets();
    setMinimumSize(QSize(400,400));
    move(QCursor::pos());
}
void StickyWidget::initWidgets()
{
    auto mainLayout=new QStackedLayout(this);
    mainLayout->setStackingMode(QStackedLayout::StackAll);
    auto topLayer=new TransparentWidget(this);
    auto topLayout=new QVBoxLayout(topLayer);
    auto bottomLayer=new QWidget(this);
    auto bottomLayout=new QVBoxLayout(bottomLayer);

    editor=new ZTextEdit(this,false);
    {//init editor
        editor->viewport()->setAttribute(Qt::WA_TranslucentBackground);
//        editor->setAttribute(Qt::WA_TranslucentBackground);
        connect(editor,&ZTextEdit::textChanged,[this](){
            note.setHtml(editor->toHtml());
            note.setOverview(editor->toPlainText());
            emit this->textChanged(editor->toPlainText());
        });
    }

    auto titleBarLayout=new QHBoxLayout(topLayer);
    {//init titlebar
        auto attachButton=new DIconButton(this);
        attachButton->setFlat(true);
        attachButton->setIcon(QIcon(":/images/attach"));
        attachButton->setIconSize(QSize(30,35));
        connect(attachButton,&DIconButton::clicked,[this](){
            note.toggleAttach();
            emit this->attach();
//            this->deleteLater();
            this->hide();
        });
        titleBarLayout->addStretch();
        titleBarLayout->addWidget(attachButton);
    }

    auto toolBarLayout=new QHBoxLayout(topLayer);
    {//init toolbar
        toolBarLayout->addStretch();
        toolBarLayout->addWidget(initToolBar(topLayer,editor));
        toolBarLayout->addStretch();
    }

    topLayout->setContentsMargins(0,0,0,0);
    topLayout->addLayout(titleBarLayout);
    topLayout->addStretch();
    topLayout->addLayout(toolBarLayout);
    topLayer->setLayout(topLayout);

    bottomLayout->addSpacing(10);
    bottomLayout->addWidget(editor);
    bottomLayer->setLayout(bottomLayout);

    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(topLayer);
    mainLayout->addWidget(bottomLayer);
    setLayout(mainLayout);
    setContentsMargins(0,0,0,0);
}
QWidget* StickyWidget::initToolBar(QWidget *parent,ZTextEdit *textEditor)
{
    auto bar=new RoundedWidget(parent,15,RoundedWidget::TopTwo,QPoint(0,5));
    bar->setShadow(5);
    auto layout=new QHBoxLayout(bar);
    auto bold=new CircleButton(QIcon(":/images/font-bold"),bar),
            italic=new CircleButton(QIcon(":/images/font-italic"),bar),
            underline=new CircleButton(QIcon(":/images/font-underline"),bar),
            linethrough=new CircleButton(QIcon(":/images/font-linethrough"),bar),
            image=new CircleButton(QIcon(":/images/insimage"),bar);
    for(auto i:{bold,italic,underline,linethrough})
    {
        i->setCheckable(true);
    }
    connect(bold,&QPushButton::toggled,textEditor,&ZTextEdit::fBold);
    connect(textEditor,&ZTextEdit::fBoldState,bold,&QPushButton::setChecked);
    connect(italic,&QPushButton::toggled,textEditor,&ZTextEdit::fItalic);
    connect(textEditor,&ZTextEdit::fItalicState,italic,&QPushButton::setChecked);
    connect(underline,&QPushButton::toggled,textEditor,&ZTextEdit::fUnderline);
    connect(textEditor,&ZTextEdit::fUnderlineState,underline,&QPushButton::setChecked);
    connect(linethrough,&QPushButton::toggled,textEditor,&ZTextEdit::fLinethrough);
    connect(textEditor,&ZTextEdit::fLinethroughState,linethrough,&QPushButton::setChecked);
    connect(image,&QPushButton::clicked,textEditor,&ZTextEdit::pInsImage);

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

    layout->addWidget(bold),layout->addWidget(italic),layout->addWidget(underline),layout->addWidget(linethrough),layout->addWidget(image);

    bar->setLayout(layout);
    return bar;
}
/*void StickyWidget::focusOutEvent(QFocusEvent *e)
{
    commitChange(note);
}*/
void StickyWidget::setNote(ZNote &d)
{
    note=d;
    editor->setHtml(d.getHtml());
}
ZNote StickyWidget::getNote()
{
    return note;
}
